#pragma once

namespace Ventile {
	namespace System {
		class VENTILEAPI Renderer {
		public:
			Renderer(const char* appName);
			~Renderer();

		private:
			void _CreateInstance(const char* appName);
			void _RemoveInstance();

			void _CreateDevice();
			void _RemoveDevice();

			VkInstance	_instance = nullptr;
			VkDevice	_device = nullptr;
			VkPhysicalDevice _gpu = nullptr;
			unsigned int _graphics_family_index = 0;

		};
	}
}