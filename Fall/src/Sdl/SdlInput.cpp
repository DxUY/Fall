#include "FallPCH.h"

#include "Core/Input.h"

#include <SDL3/SDL.h>

namespace Fall {

    bool Input::IsKeyPressed(KeyCode key) {
        const bool* state = SDL_GetKeyboardState(nullptr);
        return state[key] != 0;
    }

    bool Input::IsMouseButtonPressed(MouseCode button) {
        Uint32 buttons = SDL_GetMouseState(nullptr, nullptr);
        return (buttons & SDL_BUTTON_MASK(static_cast<int>(button))) != 0;
    }

    glm::vec2 Input::GetMousePosition() {
        float x = 0.0f, y = 0.0f;
        SDL_GetMouseState(&x, &y);
        return { x, y };
    }

    float Input::GetMouseX() { return GetMousePosition().x; }
    float Input::GetMouseY() { return GetMousePosition().y; }

}