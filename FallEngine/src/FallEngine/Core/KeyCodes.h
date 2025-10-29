#pragma once

namespace FallEngine {

	using KeyCode = uint32_t;
	using KeyMask = uint32_t;

	namespace Key {
		enum : KeyCode {
			A = 4,
			B = 5,
			C = 6,
			D = 7,
			E = 8,
			F = 9,
			G = 10,
			H = 11,
			I = 12,
			J = 13,
			K = 14,
			L = 15,
			M = 16,
			N = 17,
			O = 18,
			P = 19,
			Q = 20,
			R = 21,
			S = 22,
			T = 23,
			U = 24,
			V = 25,
			W = 26,
			X = 27,
			Y = 28,
			Z = 29,

			Num1 = 30,
			Num2 = 31,
			Num3 = 32,
			Num4 = 33,
			Num5 = 34,
			Num6 = 35,
			Num7 = 36,
			Num8 = 37,
			Num9 = 38,
			Num0 = 39,

			Return = 40,
			Escape = 41,
			Backspace = 42,
			Tab = 43,
			Space = 44,

			LeftShift = 225,
			LeftCtrl = 224,
			LeftAlt = 226,
			LeftSuper = 227, 

			RightShift = 229,
			RightCtrl = 228,
			RightAlt = 230,
			RightSuper = 231,

			ArrowLeft = 80,
			ArrowRight = 79,
			ArrowUp = 82,
			ArrowDown = 81,
		};

		enum : KeyMask {
			None = 0,
			Shift = 1 << 0,
			Ctrl = 1 << 1,
			Alt = 1 << 2,
			Super = 1 << 3,
		};
	}

}