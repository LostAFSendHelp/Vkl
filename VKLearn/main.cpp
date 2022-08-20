#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <iostream>

#include "VKTriangleApplication.h"

int main() {
	Vkl::VkTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::exception& exc) {
		std::cout << exc.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}