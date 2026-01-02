#pragma once

#include <cstdint>
#include <vector>

struct SDL_GPUTexture;

namespace FallEngine {

	class GPUCommand;

	class FrameContext {
	public:
		FrameContext() = default;

		void Reset(uint64_t newFrameIndex);

		uint64_t GetFrameIndex() const { return m_FrameIndex; }
		GPUCommand* GetCommand() const { return m_Command; }

		SDL_GPUTexture* GetBackbuffer() const { return m_Backbuffer; }
		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

	private:
		friend class Renderer;

		void SetCommand(GPUCommand* command) { m_Command = command; }
		void SetBackbuffer(SDL_GPUTexture* tex, uint32_t w, uint32_t h);

	private:
		uint64_t m_FrameIndex = 0;
		GPUCommand* m_Command = nullptr;

		SDL_GPUTexture* m_Backbuffer = nullptr;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;

		std::vector<void*> m_TransientResources;
	};

}