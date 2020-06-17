// 1. Create instance
// 2. Create device
//	a. find GPU
//	b. Find gpu family with graphics support

#include "Ventile.h"

namespace Ventile {
	extern System::Logger* logger;

	namespace System {
		Renderer::Renderer(const char* appName) {
			DEBUG(logger, "Creating Vulkan Renderer...");

			_CreateInstance(appName);
			_CreateDevice();
		}

		Renderer::~Renderer() {
			DEBUG(logger, "Destroying vulkan renderer...");

			_RemoveDevice();
			_RemoveInstance();
		}

		void Renderer::_CreateInstance(const char* appName) {
			// Create application info
			VkApplicationInfo appinfo{};
			appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appinfo.apiVersion = VK_API_VERSION_1_2;
			appinfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
			appinfo.pApplicationName = appName;

			// create Instance information
			VkInstanceCreateInfo instanceinfo{};
			instanceinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			instanceinfo.pApplicationInfo = &appinfo;

			if (vkCreateInstance(&instanceinfo, nullptr, &_instance) != VK_SUCCESS)
				ERRLQ(logger, "Failed to create VK Instance!");
		}

		void Renderer::_RemoveInstance() {
			vkDestroyInstance(_instance, nullptr);
			_instance = nullptr;
		}

		void Renderer::_CreateDevice() {
			DEBUG(logger, "Finding gpu...");
			{
				unsigned int gpu_count = 0;
				vkEnumeratePhysicalDevices(_instance, &gpu_count, nullptr);

				if (gpu_count <= 0)
					ERRLQ(logger, "Failed to find a gpu with vulkan support...");

				DEBUG(logger, "Found %u gpus", gpu_count);

				VkPhysicalDevice* gpu_list = (VkPhysicalDevice *)ec_malloc(sizeof(VkPhysicalDevice) * gpu_count);

				vkEnumeratePhysicalDevices(_instance, &gpu_count, gpu_list);
				_gpu = gpu_list[0];

				free(gpu_list);
			}

			DEBUG(logger, "Looping family queue");
			{
				uint32_t family_count = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(_gpu, &family_count, nullptr);

				DEBUG(logger, "Found %u families", family_count);

				VkQueueFamilyProperties* family_property_list = (VkQueueFamilyProperties*)ec_malloc(sizeof(VkQueueFamilyProperties) * family_count);

				vkGetPhysicalDeviceQueueFamilyProperties(_gpu, &family_count, family_property_list);

				bool found = false;
				for (unsigned int i = 0; i < family_count; i++) {
					if (family_property_list[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						found = true;
						_graphics_family_index = i;
					}
				}

				if (!found)
					ERRLQ(logger, "Failed to get family with graphics support");

				free(family_property_list);
			}

			DEBUG(logger, "Creating device...");
			float queue_priorities[] = { 1.0f };
			VkDeviceQueueCreateInfo queueinfo{};
			queueinfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueinfo.queueFamilyIndex = _graphics_family_index;
			queueinfo.queueCount = 1;
			queueinfo.pQueuePriorities = queue_priorities;

			VkDeviceCreateInfo deviceinfo{};
			deviceinfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			deviceinfo.queueCreateInfoCount = 1;
			deviceinfo.pQueueCreateInfos = &queueinfo;

			if (vkCreateDevice(_gpu, &deviceinfo, nullptr, &_device) != VK_SUCCESS)
				ERRLQ(logger, "Failed to create Vk Device!");

		}

		void Renderer::_RemoveDevice() {
			vkDestroyDevice(_device, nullptr);
			_device = nullptr;
		}
	}
}