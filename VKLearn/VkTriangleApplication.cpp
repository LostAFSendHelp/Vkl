#include <stdexcept>
#include <iostream>
#include <cstring>
#include <fmt/format.h>
#include "VkTriangleApplication.h"

namespace Vkl {
	const int VkTriangleApplication::WINDOW_INIT_WIDTH{ 800 };
	const int VkTriangleApplication::WINDOW_INIT_HEIGHT{ 600 };
	const char* VkTriangleApplication::WINDOW_INIT_NAME{ "VK Triangle Application" };

#ifdef NDEBUG
	const bool VkTriangleApplication::VALIDATION_LAYER_ENABLED{ false };
#else
	const bool VkTriangleApplication::VALIDATION_LAYER_ENABLED{ true };
#endif // NDEBUG

	void VkTriangleApplication::run() { 
		_initWindow();
		_initVulkan();
		_mainLoop();
		_cleanup(); 
	}

	void VkTriangleApplication::_initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		_pWindow = glfwCreateWindow(
			WINDOW_INIT_WIDTH,
			WINDOW_INIT_HEIGHT,
			WINDOW_INIT_NAME,
			nullptr,
			nullptr
		);
	}

	std::vector<const char*> VkTriangleApplication::_requestInstanceExtensions(uint32_t* extCount) const {
		// enumerate available extensions
		uint32_t availableExtCount;

		auto result = vkEnumerateInstanceExtensionProperties(
			nullptr,
			&availableExtCount,
			nullptr
		);

		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to enumerate VK instance extensions");
		}

		std::vector<VkExtensionProperties> props{ availableExtCount };

		vkEnumerateInstanceExtensionProperties(
			nullptr,
			&availableExtCount,
			props.data()
		);

		std::cout << "Supported Instance extensions:" << std::endl;

		for (const auto& prop : props) {
			std::cout << "\t" << prop.extensionName << std::endl;
		}

		// get required extensions
		uint32_t glfwRequiredExtCount;
		auto glfwRequiredExtensions = glfwGetRequiredInstanceExtensions(&glfwRequiredExtCount);
		auto requiredExtCount = glfwRequiredExtCount;

		std::vector<const char*> requiredExtensions{ 
			glfwRequiredExtensions, 
			glfwRequiredExtensions + glfwRequiredExtCount
		};

		if (VALIDATION_LAYER_ENABLED) {
			// require one more extension if validation layer is enabled
			++requiredExtCount;
			requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		// check for extension support
		auto allRequiredExtensionsFound = true;

		for (const auto& extension : requiredExtensions) {
			auto extensionFound = false;

			for (const auto& prop : props) {
				if (!strcmp(extension, prop.extensionName)) {
					std::cout << "Required extension " << extension << " FOUND" << std::endl;
					extensionFound = true;
					break;
				}
			}

			if (!extensionFound) {
				std::cout << "Required extension " << extension << " MISSING" << std::endl;
				allRequiredExtensionsFound = false;
				break;
			}
		}

		if (!allRequiredExtensionsFound) {
			throw std::runtime_error("Required extension(s) MISSING");
		}

		*extCount = requiredExtCount;
		return requiredExtensions;
	}

	std::vector<const char*> VkTriangleApplication::_requestValidationLayers(uint32_t* layerCount) const {
		// return nothing if validation layers are disabled
		if (!VALIDATION_LAYER_ENABLED) {
			std::cout << "Validation layer DISABLED" << std::endl;
			*layerCount = 0u;
			return {};
		}

		std::cout << "Validation layer ENABLED" << std::endl;

		// enumerate available layers
		uint32_t availableLayerCount;
		vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr);
		
		if (availableLayerCount == 0u) {
			throw std::runtime_error("No Vulkan Instance Layers found");
		}

		std::vector<VkLayerProperties> availableLayers{ availableLayerCount };
		vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers.data());

		std::cout << availableLayerCount << " Instance Layers found:" << std::endl;
		for (const auto& layer : availableLayers) {
			std::cout << '\t' << layer.layerName << std::endl;
		}

		// check for support for requested layers
		std::vector<const char*> requestedLayers{ "VK_LAYER_KHRONOS_validation" };
		auto allLayersFound = true;

		for (auto requestedLayer : requestedLayers) {
			auto layerFound = false;

			for (const auto& availableLayer : availableLayers) {
				if (!strcmp(requestedLayer, availableLayer.layerName)) {
					std::cout << "Requested extension " << requestedLayer << " FOUND" << std::endl;
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				std::cout << "Requested extension " << requestedLayer << " MISSING" << std::endl;
				allLayersFound = false;
				break;
			}
		}

		if (!allLayersFound) {
			throw std::runtime_error("Required instance layer(s) MISSING");
		}

		*layerCount = static_cast<uint32_t>(requestedLayers.size());
		return requestedLayers;
	}

	void VkTriangleApplication::_initVulkan() {
		_createVkInstance();
		_createDebugMessenger();
	}

	void VkTriangleApplication::_createVkInstance() {
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "VK TRIANGLE APPLICATION";
		appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
		appInfo.pEngineName = "VKL";
		appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
		appInfo.apiVersion = VK_API_VERSION_1_0;
		
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.flags = 0;
		uint32_t enabledExtensionCount;
		auto enabledExtensionNames = _requestInstanceExtensions(&enabledExtensionCount);
		createInfo.enabledExtensionCount = enabledExtensionCount;
		createInfo.ppEnabledExtensionNames = enabledExtensionNames.data();
		uint32_t enabledLayerCount;
		auto enabledLayerNames = _requestValidationLayers(&enabledLayerCount);
		createInfo.enabledLayerCount = enabledLayerCount;
		createInfo.ppEnabledLayerNames = enabledLayerNames.data();

		if (VALIDATION_LAYER_ENABLED) {
			auto debugMsgCreateInfo = _getDebugMessengerCreateInfo();
			createInfo.pNext = &debugMsgCreateInfo;
		}

		auto result = vkCreateInstance(
			&createInfo,
			nullptr,
			&_vkInstance
		);

		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to create Vulkan instance");
		}
	}

	void VkTriangleApplication::_createDebugMessenger() {
		if (!VALIDATION_LAYER_ENABLED) {
			return;
		}

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
			_vkInstance,
			"vkCreateDebugUtilsMessengerEXT"
		);

		if (func) {
			auto msgCreateInfo = _getDebugMessengerCreateInfo();
			
			auto result = func(
				_vkInstance,
				&msgCreateInfo,
				nullptr,
				&_vkDebugMessenger
			);

			if (result != VK_SUCCESS) {
				throw std::runtime_error("Failed to create debug messenger");
			}
		} else {
			throw std::runtime_error("Failed to create debug messenger");
		}
	}

	void VkTriangleApplication::_destroyDebugMessenger() {
		if (!VALIDATION_LAYER_ENABLED) {
			return;
		}

		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
			_vkInstance, 
			"vkDestroyDebugUtilsMessengerEXT"
		);

		if (func) {
			func(
				_vkInstance,
				_vkDebugMessenger,
				nullptr
			);
		} else {
			throw std::runtime_error("Failed to destroy debug messenger");
		}
	}

	VkDebugUtilsMessengerCreateInfoEXT VkTriangleApplication::_getDebugMessengerCreateInfo() const {
		auto msgCreateInfo = VkDebugUtilsMessengerCreateInfoEXT{ };

		msgCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		msgCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		msgCreateInfo.pfnUserCallback = _debugCallback;
		msgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

		return msgCreateInfo;
	}

	void VkTriangleApplication::_mainLoop() {
		while (!glfwWindowShouldClose(_pWindow)) {
			glfwPollEvents();
		}
	}

	void VkTriangleApplication::_cleanup() {
		_destroyDebugMessenger();
		vkDestroyInstance(_vkInstance, nullptr);
		glfwDestroyWindow(_pWindow);
		glfwTerminate();
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VkTriangleApplication::_debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType, 
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
		void* userData
	) {
		if (messageSeverity > VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
			auto message = fmt::format(
				"[{}] Validation: {}",
				messageType,
				pCallbackData->pMessage
			);

			std::cerr << message << std::endl;
		}

		return VK_FALSE;
	}
};  // namespace VKL