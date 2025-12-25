#include <FallEngine.h>

#include "Layers/EditorLayer.h"

class Editor : public FallEngine::Application
{
public:
	Editor() 
	{

	}

	~Editor() {}
};

FallEngine::Application* FallEngine::CreateApplication() {
	return new Editor();
}