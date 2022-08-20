#pragma once
#include <GLFW/glfw3.h>

namespace Vkl {
	class VkTriangleApplication {
	public:
		void run();

		static const int WINDOW_INIT_WIDTH;
		static const int WINDOW_INIT_HEIGHT;
		static const char* WINDOW_INIT_NAME;

	private:
		void initWindow();

		void initVulkan();
		void createVkInstance();

		void mainLoop();
		void cleanup();

		GLFWwindow* mWindow;
		VkInstance mVkInstance;
	};
}  // namespace VKL
