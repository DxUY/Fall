#include <Fall.h>
#include "Core/EntryPoint.h"

#include "Layers/EditorLayer.h"


class Editor : public Fall::Application {
public:
	Editor() {
		
	}

	~Editor() {}
};

Fall::Application* Fall::CreateApplication() {
	return new Editor();
}