#include <FallEngine.h>

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