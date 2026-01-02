#include "FallEnginePCH.h"
#include "FrameContext.h"

namespace FallEngine {

	void FrameContext::Reset(uint64_t newFrameIndex)
	{
		m_FrameIndex = newFrameIndex;
		m_Command = nullptr;
		m_Backbuffer = nullptr;
		m_Width = 0;
		m_Height = 0;
		m_TransientResources.clear();
	}

	void FrameContext::SetBackbuffer(SDL_GPUTexture* tex, uint32_t w, uint32_t h)
	{
		m_Backbuffer = tex;
		m_Width = w;
		m_Height = h;
	}

}
