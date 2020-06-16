#pragma once

namespace Ventile {
	namespace System {
		class VENTILEAPI WindowHandler {
		public:
			GLFWwindow* window;

			WindowHandler(int height = 800, int width = 600, const char* title = "Ventile");
			~WindowHandler();
		private:
			int _width;
			int _height;
			char* _title;
		};
	}
}