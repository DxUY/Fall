#include "FallEnginePCH.h"
#include "WindowsInput.h"

#include "FallEngine/Core/Application.h"
#include <SDL3/SDL.h>

namespace FallEngine {

    Input* Input::s_Instance = new WindowsInput();

    bool WindowsInput::IsKeyPressedImpl(KeyCode keycode)
    {
		auto window = static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());
        const bool* state = SDL_GetKeyboardState(nullptr);
        return state[keycode] != 0;  
        return true;
    }

    bool WindowsInput::IsMouseButtonPressedImpl(MouseCode button)
    {
        auto window = static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());
        Uint32 buttons = SDL_GetMouseState(nullptr, nullptr);
        return (buttons & SDL_BUTTON_MASK(static_cast<int>(button))) != 0;
        return true;
    }

    /*glm::vec2 WindowsInput::GetMousePositionImpl()
    {
        auto window = static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());
        float x = 0.0f, y = 0.0f;
        SDL_GetMouseState(&x, &y);
        return { x, y };
    }*/

    float WindowsInput::GetMouseXImpl()
    {
        return /*GetMousePositionImpl().x*/ 1.0;
    }

    float WindowsInput::GetMouseYImpl()
    {
        return /*GetMousePositionImpl().y*/ 1.0;
    }
}