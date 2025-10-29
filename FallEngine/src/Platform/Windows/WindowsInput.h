#pragma once

#include "FallEngine/Core/Input.h"

namespace FallEngine {

	class WindowsInput : public Input {
	protected:
		virtual bool IsKeyPressedImpl(KeyCode keycode) override;

		virtual bool IsMouseButtonPressedImpl(MouseCode button) override;
		/*virtual glm::vec2 GetMousePositionImpl() override;*/
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};

}