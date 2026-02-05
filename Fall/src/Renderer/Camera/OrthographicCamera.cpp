#include "FallPCH.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Fall {
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearP, float farP) {
        m_Projection = glm::ortho(left, right, bottom, top, nearP, farP);

        RecalculateViewMatrix();
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top, float nearP, float farP) {
        m_Projection = glm::ortho(left, right, bottom, top, nearP, farP);
        m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
    }

    void OrthographicCamera::RecalculateViewMatrix() {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

        m_ViewMatrix = glm::inverse(transform);

        m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
    }

}