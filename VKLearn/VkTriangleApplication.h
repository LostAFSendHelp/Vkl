#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "Utils/Log.h"

namespace Vkl {
	class VkTriangleApplication {
	public:
		void run();

		static const int WINDOW_INIT_WIDTH;
		static const int WINDOW_INIT_HEIGHT;
		static const char* WINDOW_INIT_NAME;

	private:
		void _initWindow();

		std::vector<const char*> _requestInstanceExtensions(uint32_t* pExtCount) const;
		std::vector<const char*> _requestValidationLayers(uint32_t* pLayerCount) const;
		void _initVulkan();
		void _createVkInstance();
		void _createDebugMessenger();
		void _destroyDebugMessenger();
		VkDebugUtilsMessengerCreateInfoEXT _getDebugMessengerCreateInfo() const;

		void _mainLoop();
		void _cleanup();

		static VKAPI_ATTR VkBool32 VKAPI_CALL _debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* userData
		);

		GLFWwindow* _pWindow;
		VkInstance _vkInstance;
		VkDebugUtilsMessengerEXT _vkDebugMessenger;

		static const bool _VALIDATION_LAYER_ENABLED;
	};
}  // namespace VKL
