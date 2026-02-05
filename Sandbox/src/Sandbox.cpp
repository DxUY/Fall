#include <Fall.h>
#include "Core/EntryPoint.h"

#include "Layers/EditorLayer.h"

class Sandbox : public Fall::Application {
public:
	Sandbox() {
		
	}

	~Sandbox() {}
};

Fall::Application* Fall::CreateApplication() {
	return new Sandbox();
}