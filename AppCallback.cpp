#include <iostream>

#include "App.h"

void App::error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        case GLFW_KEY_F:
            std::cout << "F clicked!\n";
            is_fullscreen_on = !is_fullscreen_on;
            if (is_fullscreen_on) {
                glfwGetWindowPos(window, &window_xcor, &window_ycor);
                glfwGetWindowSize(window, &window_width_return_from_fullscreen, &window_height_return_from_fullscreen);
                if (window_height_return_from_fullscreen == 0) window_height_return_from_fullscreen++;
                glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            }
            else {
                glfwSetWindowMonitor(window, nullptr, window_xcor, window_ycor, window_width_return_from_fullscreen, window_height_return_from_fullscreen, 0);
            }
            break;
        case GLFW_KEY_V:
            std::cout << "V clicked!\n";
            is_vsync_on = !is_vsync_on;
            glfwSwapInterval(is_vsync_on);
            if (is_vsync_on) {
                std::cout << "VSync is ON!" << "\n";
            }
            else {
                std::cout << "VSync is OFF!" << "\n";
            }
            break;
        case GLFW_KEY_E:
            std::cout << "E clicked!\n";
            //std::cout << "holdItem: " << holdItem << "\n";
            // static x no static - dumb ... later repair logic for pick up
            holdItem = !holdItem;
            break;
        case GLFW_KEY_G:
            std::cout << "G clicked!\n";
            isFlashlightOn = !isFlashlightOn;
            break;
        case GLFW_KEY_L:
            std::cout << "L clicked!\n";
            isLampOn = !isLampOn;
            break;
        }
    }

    // Sprint
    if (action == GLFW_PRESS && key == GLFW_KEY_LEFT_SHIFT) {
        std::cout << "LEFT_SHIFT clicked!\n";
        camera.toggleSprint();
    }
}

void App::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Zoooooom
    auto this_inst = static_cast<App*>(glfwGetWindowUserPointer(window));
    this_inst->FOV += 10.0f * static_cast<float>(yoffset);
    this_inst->FOV = std::clamp(this_inst->FOV, 20.0f, 170.0f); // limit FOV to reasonable values...
    this_inst->UpdateProjectionMatrix();
}

void App::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    auto this_inst = static_cast<App*>(glfwGetWindowUserPointer(window));
    this_inst->window_width = width;
    this_inst->window_height = height;
    // set viewport
    glViewport(0, 0, width, height);
    //now your canvas has [0,0] in bottom left corner, and its size is [width x height] 
    this_inst->UpdateProjectionMatrix();
}

void App::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        std::cout << "Right click!\n";
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "Left click!\n";
        projectile.onKeyboardEvent(window, camera.getPosition(), button, action, mods);
    }
}