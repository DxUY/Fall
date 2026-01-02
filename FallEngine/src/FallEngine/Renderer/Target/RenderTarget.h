#pragma once
#include <cstdint>

namespace FallEngine {

	enum class LoadOp {
		Load,
		Clear
	};

	enum class StoreOp {
		Store,
		Discard
	};

	struct ClearColor {
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 1.0f;
	};

	class RenderTarget {
	public:
		static RenderTarget Backbuffer(
			ClearColor clear = {}
		) {
			RenderTarget rt;
			rt.m_IsBackbuffer = true;
			rt.m_LoadOp = LoadOp::Clear;
			rt.m_StoreOp = StoreOp::Store;
			rt.m_Clear = clear;
			return rt;
		}

		bool IsBackbuffer() const { return m_IsBackbuffer; }
		LoadOp GetLoadOp() const { return m_LoadOp; }
		StoreOp GetStoreOp() const { return m_StoreOp; }
		const ClearColor& GetClearColor() const { return m_Clear; }

	private:
		bool m_IsBackbuffer = false;
		LoadOp m_LoadOp = LoadOp::Load;
		StoreOp m_StoreOp = StoreOp::Store;
		ClearColor m_Clear{};
	};

}