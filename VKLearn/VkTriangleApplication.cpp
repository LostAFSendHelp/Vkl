#define GLFW_INCLUDE_VULKAN
#include <stdexcept>
#include "VkTriangleApplication.h"

namespace Vkl {
	const int VkTriangleApplication::WINDOW_INIT_WIDTH{ 800 };
	const int VkTriangleApplication::WINDOW_INIT_HEIGHT{ 600 };
	const char* VkTriangleApplication::WINDOW_INIT_NAME{ "VK Triangle Application" };

	void VkTriangleApplication::run() { 
		initWindow();
		initVulkan();
		mainLoop();
		cleanup(); 
	}

	void VkTriangleApplication::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		mWindow = glfwCreateWindow(
			WINDOW_INIT_WIDTH,
			WINDOW_INIT_HEIGHT,
			WINDOW_INIT_NAME,
			nullptr,
			nullptr
		);
	}

	void VkTriangleApplication::initVulkan() {
		createVkInstance();
	}

	void VkTriangleApplication::createVkInstance() {
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
		createInfo.enabledLayerCount = 0u;
		createInfo.flags = 0;
		uint32_t glfwExtCount;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtCount);
		createInfo.enabledExtensionCount = glfwExtCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		auto result = vkCreateInstance(
			&createInfo,
			nullptr,
			&mVkInstance
		);

		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to create Vulkan instance");
		}
	}

	void VkTriangleApplication::mainLoop() {
		while (!glfwWindowShouldClose(mWindow)) {
			glfwPollEvents();
		}
	}

	void VkTriangleApplication::cleanup() {
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}
};  // namespace VKL