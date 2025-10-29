#pragma once

#include "FallEngine/Core/Layer.h"

namespace FallEngine {
	class  ImGuiLayer : public Layer {
	
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();
		
		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& event);

		void Begin();
		void End();

		void OnImGuiRender() override;
	private:
		float m_Time = 0.0f;
	};
}