#pragma once

namespace FallEngine {

    using MouseCode = uint32_t;
    using MouseMask = uint32_t;

    namespace Mouse {

        enum : MouseCode {
            ButtonLeft = 1,
            ButtonMiddle = 2,
            ButtonRight = 3,
            ButtonX1 = 4,
            ButtonX2 = 5,
            ButtonLast = ButtonX2
        };

        enum : MouseMask {
            LeftMask = (1u << (ButtonLeft - 1)),
            MiddleMask = (1u << (ButtonMiddle - 1)),
            RightMask = (1u << (ButtonRight - 1)),
            X1Mask = (1u << (ButtonX1 - 1)),
            X2Mask = (1u << (ButtonX2 - 1))
        };

    }

}