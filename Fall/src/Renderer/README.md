Renderer GPU Boundary Rules (Synchronous Renderer)
Purpose

This renderer is designed for deterministic execution, explicit ownership, predictable performance, and debuggability under long-lived, simulation-heavy workloads (e.g. Noita-like systems).

The renderer is synchronous by design:

GPU work submission is explicit and ordered

CPU/GPU synchronization points are intentional

Stalls are allowed but must be visible and controlled

These rules are architectural constraints.
Violations are bugs, not style issues.

GPU Ownership Rule (Non-Negotiable)

Only code inside Renderer/GPU may call SDL GPU APIs.

Forbidden outside Renderer/GPU

SDL_CreateGPU*

SDL_DestroyGPU*

SDL_SubmitGPUCommandBuffer

Accessing raw SDL_GPU* handles

Waiting on GPU fences or sync primitives

Allowed outside Renderer/GPU

Requesting work via descriptors or handles

Passing immutable data

Using renderer-level abstractions

Reason:
Even in a synchronous renderer, distributed GPU ownership leads to undefined synchronization, hidden stalls, and untraceable performance issues.

Handle Rule (No Raw GPU Objects)

Raw GPU handles must never appear outside Renderer/GPU.

Forbidden

SDL_GPUTexture*

SDL_GPUBuffer*

SDL_GPUPipeline*

SDL_GPUShader*

SDL_RenderPass*

Allowed

TextureHandle

BufferHandle

PipelineHandle

ShaderHandle

All GPU objects must be opaque and wrapped.

Renderer Responsibility Rule

Renderer/Core/Renderer is responsible for:

Frame lifecycle

Swapchain ownership

Explicit render pass execution order

Explicit synchronization points

Renderer must never:

Create GPU resources

Compile shaders

Touch raw GPU handles

Contain GPU backend logic

Renderer orchestrates execution order.
It does not perform GPU work.

Pass Execution Rule (Explicit Order)

Render passes are executed explicitly and synchronously, in a fixed order.

There is no render graph

There is no dependency solver

There is no automatic scheduling

Reason:
In a sync renderer, predictability and step-by-step debugging outweigh automation and abstraction.

Pipeline Rule (Synchronous Creation)

Pipelines are:

Immutable after creation

Created synchronously

Cached and reused

Rules

Pipeline creation may stall, but the stall must be explicit

Pipeline creation must never occur mid-pass

Pipelines must be cached by key

Pipeline creation is allowed only at defined points (e.g. load, warm-up, or explicit rebuild)

If a pipeline is missing at draw time, this is a bug.

Shader Rule (Descriptors Only)

Shaders are data-only descriptors.

Shaders contain:

Bytecode

Reflection metadata

Shaders do not:

Create pipelines

Know render targets

Submit GPU work

Shaders describe capability, not execution.

Resource Lifetime Rule

GPU resources must:

Explicitly own their lifetime

Track usage and access flags

Support resize and recreation

Be destroyed at explicit synchronization points

Rules

Destruction may block, but only at known boundaries

No implicit lifetime extension

No hidden GPU allocations

All lifetime transitions must be visible in code.

Threading Rule (Single-Threaded GPU)

All GPU interaction happens on the render thread.

No background submission

No async GPU jobs

No cross-thread GPU state

If a function touches GPU state, it must live in Renderer/GPU.

Performance Rule

This renderer prioritizes:

Deterministic execution

Explicit stalls

Minimal hidden work

Stable memory usage

Debuggability under heavy simulation load

Any abstraction that hides cost must be justified.

Enforcement

Assertions are preferred over comments

Violations must fail fast in debug builds

Refactors must preserve these boundaries

Breaking these rules leads to:

Hidden stalls

Undefined sync behavior

Unfixable coupling

Naming Conventions (Renderer)

(Mostly unchanged — these rules still apply in a sync renderer)

GPU* Prefix (GPU Ownership)

Any type, function, or file that touches SDL GPU objects or submits GPU work must start with GPU.

Examples:

GPUContext

GPUQueue

GPUCommandBuffer

Rules:

GPU* code exists only in Renderer/GPU

GPU* implies render-thread execution

If it touches SDL_GPU* and is not named GPU*, it is a violation

*Handle Suffix (Opaque References)

Any GPU reference crossing subsystem boundaries must be a handle.

Examples:

TextureHandle

BufferHandle

PipelineHandle

Rules:

Handles are lightweight and copyable

Handles do not own GPU memory

Handles must never expose raw GPU pointers

*Desc Suffix (Creation Intent)

Descriptors define intent only.

Examples:

TextureDesc

PipelineDesc

RenderTargetDesc

Rules:

Plain data only

No GPU handles

Immutable after submission

*Key Suffix (Identity & Caching)

Examples:

PipelineKey

ShaderKey

Rules:

Immutable

Hashable

Fully define identity

Never contain pointers

*Pass Suffix (Execution Units)

Examples:

RenderPass

ComputePass

Rules:

Passes record commands

Passes do not create GPU objects

Passes do not schedule other passes

Passes are executed explicitly by the renderer

*Manager Suffix (Ownership & Coordination)

Examples:

PipelineManager

ResourceManager

Rules:

Managers own lifetime and caching

Managers do not submit GPU work

Managers may block only at explicit sync points

General Naming Rules

Classes: PascalCase

Functions: PascalCase

Variables: camelCase

Constants: ALL_CAPS

If naming does not clearly communicate ownership, lifetime, and thread domain, it is wrong.

Frames-in-Flight Decision (Synchronous)
Decision

This renderer uses one frame in flight.

Rationale

This is a synchronous renderer:

CPU submits work

GPU completes work

Frame advances only after completion

This guarantees:

Deterministic behavior

Simple lifetime management

Straightforward debugging

No implicit buffering or latency hiding

Implications

No frame overlap

No deferred destruction queues

Resource destruction may block

All transient allocations are frame-local

GPU completion is assumed at frame boundaries

Any code assuming GPU work continues after the frame ends is invalid.

Non-Goals

CPU/GPU overlap

Multi-frame latency hiding

Async pipeline creation

Dynamic frame buffering

Rule

All renderer systems must assume immediate GPU completion at frame end.

This decision is final unless the renderer is explicitly redesigned as async.