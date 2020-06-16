#include "Ventile.h"

namespace Ventile {
	extern System::Logger* logger;

	namespace System {

		WindowHandler::WindowHandler(int height, int width, const char* title) {
			DEBUG(logger, "Creating application window!");

			_height = height; _width = width;

			_title = (char*)System::ec_malloc(strlen(title) + 1);
			strcpy(_title, title);

			if (!glfwInit())
				ERRLQ(logger, "Failed to init glew");

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

			if ((window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr)) == NULL)
				ERRLQ(logger, "Failed to create glfw window!");

		}

		WindowHandler::~WindowHandler() {
			DEBUG(logger, "Destroying application window!");

			glfwDestroyWindow(window);
			glfwTerminate();

			free(_title);

			DEBUG(logger, "Application window destroyed!");
		}
	}
}