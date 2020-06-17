#pragma once

namespace Ventile {
	namespace System {
		class VENTILEAPI WindowHandler {
		public:
			GLFWwindow* window;

			WindowHandler(int height = 800, int width = 600, const char* title = "Ventile " ENGINE_VERSION);
			~WindowHandler();

			const char* GetApplicationName();
		private:
			int _width;
			int _height;
			char* _title;
		};
	}
}