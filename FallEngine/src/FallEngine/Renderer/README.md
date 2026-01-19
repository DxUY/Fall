--- Renderer GPU Boundary Rules ---

Purpose
This renderer is designed for async safety, predictable performance, explicit memory ownership, and long-lived sessions typical of Noita-like workloads. These rules are architectural constraints. Violations are bugs.

GPU Ownership Rule (Non-Negotiable)
Only code inside Renderer/GPU may call SDL GPU APIs.
Forbidden outside Renderer/GPU:
SDL_CreateGPU*
SDL_DestroyGPU*
SDL_SubmitGPUCommandBuffer
Accessing raw SDL_GPU* handles

Allowed outside Renderer/GPU:
Requesting work via descriptors or handles
Passing immutable data
Using engine-level abstractions

Reason: Async rendering is impossible if GPU ownership is distributed.

Handle Rule (No Raw GPU Objects)
Raw GPU handles must never appear outside Renderer/GPU.
Forbidden examples:
SDL_GPUTexture*
SDL_GPUBuffer*
SDL_GPUPipeline*
SDL_GPUShader*
SDL_RenderPass*_

Allowed examples:
TextureHandle
BufferHandle
PipelineHandle

All GPU objects must be wrapped and opaque.

Renderer Responsibility Rule
Renderer/Core/Renderer is responsible for:
Frame lifecycle
Swapchain ownership
Explicit render pass execution order

Renderer must never:
Create pipelines
Compile shaders
Touch GPU handles
Wait for async jobs

Renderer orchestrates. It does not execute GPU work.

Pass Execution Rule (No Graph)
Render passes are executed explicitly and in order.
There is no render graph.
There is no dependency solver.
There is no automatic scheduling.

Reason: Predictability and debuggability outweigh abstraction.

Pipeline Rule (Async First)
Pipelines are cached and immutable.
Pipeline creation may be asynchronous.
Renderer must never block waiting for a pipeline.
Fallback pipelines are mandatory.

If a pipeline is not ready, a fallback must be used. Stalling the frame is a failure.

Shader Rule (Data Only)
Shaders contain bytecode and reflection only.
Shaders do not create pipelines.
Shaders do not know render targets.
Shaders do not submit GPU work.

Shaders are descriptors, not executable objects.

Resource Lifetime Rule
GPU resources must explicitly own their lifetime.
Resources must track usage flags.
Resources must support resize and recreation.
Destruction must be deferred safely across frames.

No implicit lifetime extension.
No hidden GPU allocations.

Threading Rule
GPU submission happens only on the GPU thread.
Async jobs may prepare data but must never submit.
Cross-thread state must be immutable or atomic.

If a function might touch GPU state, it belongs in Renderer/GPU.

Performance Rule
This renderer prioritizes minimal allocations, stable memory usage, explicit control, and debuggability under heavy load.
Any abstraction that hides cost must be justified.

Enforcement
Assertions are preferred over comments.
Rule violations should fail fast in debug builds.
Refactors must preserve these boundaries.

Breaking these rules leads to async deadlocks, frame hitches, and unfixable architectural coupling.

--- End of Renderer GPU Boundary Rules ---

--- Naming Conventions (Renderer) ---

This document defines mandatory naming rules for all renderer code. These are not stylistic preferences. They exist to make ownership, lifetime, and threading intent obvious at call-site.

GPU* Prefix (GPU Ownership)

Any type, function, or file that directly touches SDL GPU objects or submits GPU work must be prefixed with GPU.

Examples:
GPUContext
GPUQueue
GPUUtils
GPUCommandBuffer

Rules:
GPU* code may exist only inside Renderer/GPU.
GPU* types may expose opaque accessors but never raw ownership.
If a class name starts with GPU, it is assumed to run on or interact with the GPU thread.

If code touches SDL_GPU* or submission APIs and does not start with GPU, it is a violation.

*Handle Suffix (Opaque References)

Any reference to a GPU object outside Renderer/GPU must be a Handle.

Examples:
TextureHandle
BufferHandle
PipelineHandle
ShaderHandle

Rules:
Handles are lightweight, copyable, and opaque.
Handles do not own GPU memory.
Handles may be invalid or point to fallback resources.
Handles must never expose raw GPU pointers.

If a type crosses subsystem boundaries and represents a GPU object, it must be a Handle.

*Desc Suffix (Creation Intent)

Descriptors define intent for creation or configuration. They are plain data.

Examples:
TextureDesc
BufferDesc
PipelineDesc
RenderTargetDesc

Rules:
Descriptors contain no logic.
Descriptors contain no GPU handles.
Descriptors are immutable after submission.
Descriptors may be hashed or compared.

Descriptors describe what is wanted, not how it is created.

*Key Suffix (Identity & Caching)

Keys uniquely identify cached GPU objects.

Examples:
PipelineKey
ShaderKey

Rules:
Keys must be immutable.
Keys must be hashable and comparable.
Keys must fully define the identity of the cached object.
Keys must not contain pointers.

Keys are used for lookup, never for ownership.

*Pass Suffix (Execution Units)

Explicit execution units must end with Pass.

Examples:
RenderPass
ComputePass
ShadowPass

Rules:
Passes record commands.
Passes do not schedule other passes.
Passes do not create GPU objects.
Passes are executed explicitly by Renderer.

No Suffix (Ownership & Coordination)

Types own lifetime, caching, or coordination logic.

Examples:
Pipeline
Resource

Rules:
Types may launch async jobs.
Types must not submit GPU work directly.
Types must not block the renderer.

General Rules

Class names use PascalCase.
Function names use PascalCase.
Variables use camelCase.
Constants use ALL_CAPS.

If naming does not clearly communicate ownership, lifetime, and thread domain, it is wrong.

Enforcement

Naming violations are architectural violations.
They should be fixed immediately, not documented.
Code review must reject ambiguous naming.

--- End of Naming Conventions (Renderer) ---

--- Frames-in-Flight Decision ---

Rationale
Three frames in flight provide a stable balance between CPU/GPU overlap and latency for an async renderer. This avoids GPU bubbles during heavy compute or pipeline creation while keeping memory usage predictable. For a Noita-like workload with long-running simulations and large transient buffers, two frames is often insufficient under stress.

Implications
Per-frame resources must be triple-buffered.
FrameContext instances are indexed modulo 3.
Deferred destruction must wait at least 3 frames before releasing GPU resources.
Transient allocations must be isolated per frame.
Fences or frame indices must assume a 3-frame delay.

Non-Goals
Dynamic frame count switching is not supported.
One-frame-in-flight mode is not supported.
Reducing to two frames for latency reasons is out of scope unless profiling proves it necessary.

Rule
All renderer systems must assume exactly three frames in flight. Any code that assumes immediate GPU completion is invalid.

This decision is final unless a measured performance or memory issue justifies a change.

--- End of Frames-in-Flight Decision ---

