// Basic includes
#include <GL/glew.h> 

#include <iostream>
#include <chrono>
#include <stack>
#include <random>
#include <sstream>

// OpenCV ? GL independent
#include <opencv2/opencv.hpp>

// OpenGL Extension Wrangler: allow all multiplatform GL functions
// WGLEW = Windows GL Extension Wrangler (change for different platform) 
// platform specific functions (in this case Windows)
#include <GL/wglew.h> 

// GLFW toolkit
// Uses GL calls to open GL context, i.e. GLEW must be first.
#include <GLFW/glfw3.h>

// OpenGL math
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Our app
#include "App.h"
#include "gl_err_callback.h"
#include "Shader.h"

//´------------------------------------------------------------------------------------------
// Freetype library - needs different shaders -> finalPlusText.xxx
#include <ft2build.h>
#include FT_FREETYPE_H
#include "text_fonts_glyphs.h"
//´------------------------------------------------------------------------------------------

#define print(x) std::cout << x << "\n"

bool App::is_vsync_on = false;
bool App::is_fullscreen_on = false;
GLFWmonitor* App::monitor;
const GLFWvidmode* App::mode;
int App::window_xcor{};
int App::window_ycor{};
int App::window_width = 800;
int App::window_height = 600;
int App::window_width_return_from_fullscreen{};
int App::window_height_return_from_fullscreen{};

Camera App::camera = Camera(glm::vec3(0, 0, 1000));
double App::last_cursor_xpos{};
double App::last_cursor_ypos{};

Projectile App::projectile; // Definice statického členského proměnného mimo třídu


App::App()
{
	// default constructor
	// nothing to do here (for now...)
	std::cout << "Constructed...\n--------------\n";
}

// App initialization, if returns true then run run()
bool App::Init()
{
	try {
		// Set GLFW error callback
		glfwSetErrorCallback(error_callback);

		// Init GLFW :: https://www.glfw.org/documentation.html
		if (!glfwInit()) {
			return false;
		}

		// Set OpenGL version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		// Set OpenGL profile
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core, comment this line for Compatible

		// Open window (GL canvas) with no special properties :: https://www.glfw.org/docs/latest/quick.html#quick_create_window
		window = glfwCreateWindow(window_width, window_height, "OpenGL 3D Game", NULL, NULL);
		if (!window) {
			glfwTerminate();
			return false;
		}
		glfwSetWindowUserPointer(window, this);

		// Fullscreen On/Off
		monitor = glfwGetPrimaryMonitor(); // Get primary monitor
		mode = glfwGetVideoMode(monitor); // Get resolution of the monitor

		// Setup callbacks
		glfwMakeContextCurrent(window);
		glfwSetKeyCallback(window, key_callback);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetScrollCallback(window, scroll_callback);

		// Set V-Sync OFF.
		//glfwSwapInterval(0);

		// Set V-Sync ON.
		///*
		glfwSwapInterval(1);
		is_vsync_on = true;
		/**/

		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Init GLEW :: http://glew.sourceforge.net/basic.html
		GLenum err = glewInit();
		if (GLEW_OK != err) {
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err)); /* Problem: glewInit failed, something is seriously wrong. */
		}
		wglewInit();

		//...after ALL GLFW & GLEW init ...
		if (GLEW_ARB_debug_output)
		{
			glDebugMessageCallback(MessageCallback, 0);
			glEnable(GL_DEBUG_OUTPUT);

			//default is asynchronous debug output, use this to simulate glGetError() functionality
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

			std::cout << "GL_DEBUG enabled.\n";
		}
		else
			std::cout << "GL_DEBUG NOT SUPPORTED!\n";

		// set GL params
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);

		glEnable(GL_CULL_FACE); // assuming ALL objects are non-transparent 

		// Transparency blending function
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// first init OpenGL, THAN init assets: valid context MUST exist
		InitAssets();
	}
	catch (std::exception const& e) {
		std::cerr << "Init failed : " << e.what() << "\n";
		//throw;
		exit(-1);
	}
	std::cout << "--------------\nInitialized...\n";
	return true;
}

int App::Run(void)
{
	try {

		//´------------------------------------------------------------------------------------------
		// Freetype Text
		//
		FT_Library free_type;
		FT_Error error_code = FT_Init_FreeType(&free_type);
		if (error_code)
		{
			std::cout << "\n   Error code: " << error_code << " --- " << "An error occurred during initialising the FT_Library";
			int keep_console_open;
			std::cin >> keep_console_open;
		}

		//Text text_object2(free_type, window_width, window_height, "01234567890Get Rady.Timr:owns&ClBgfb"); // Declare a new text object, passing in your chosen alphabet.	
		//text_object2.create_text_message("Get Ready... Timer: 000", 100, 50, "./assets/Text Fonts/arialbi.ttf", 130, false); // True indicates that the message will be modified.

		//int num_replace = 3;
		//size_t vec_size = text_object2.messages[0].characters_quads.size();
		//float start_pos = text_object2.messages[0].start_x_current[vec_size - num_replace];

		////glUniform1i(glGetUniformLocation(text_shader.ID, "alphabet_texture"), 31);
		//shader.setUniform("alphabet_texture", 31);
		//shader.setUniform("font_colour", glm::vec3(10.0f, 120.0f, 105.0f));
		////shader.setUniform("font_colour", glm::vec3(1.0f, 1.0f, 1.0f));
		//
		//
		//´------------------------------------------------------------------------------------------

		double fps_counter_seconds = 0;
		int fps_counter_frames = 0;

		UpdateProjectionMatrix();
		glViewport(0, 0, window_width, window_height);
		camera.position = glm::vec3(0, 0, 0);
		double last_frame_time = glfwGetTime();
		glm::vec3 camera_movement{};

		glm::vec3 rgb_orange = { 1.0f, 0.5f, 0.0f };
		glm::vec3 rgb_white = { 1.0f, 1.0f, 1.0f };
		glm::vec4 rgba_white = { 1.0f, 1.0f, 1.0f, 1.0f };

		// Set camera position
		camera.position.y = 2.0f;
		camera.position.z = 5.0f;

		bool isResettingCursor = false;
		int width, height, centerX, centerY;

		// Set light position
		glm::vec3 light_position(-100000, 0, 100000);

		while (!glfwWindowShouldClose(window)) {
			// Time/FPS measure start
			auto fps_frame_start_timestamp = std::chrono::steady_clock::now();

			// Clear OpenGL canvas, both color buffer and Z-buffer
			glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// === After clearing the canvas ===

			// React to user :: Create View Matrix according to camera settings
			double delta_time = glfwGetTime() - last_frame_time;
			last_frame_time = glfwGetTime();
			//camera_movement = camera.ProcessInput(window, static_cast<float>(delta_time));
			camera.onKeyboardEvent(window, static_cast<float>(delta_time)); // process keys etc
			camera.position += camera_movement;
			glm::mat4 mx_view = camera.getViewMatrix();

			POINT p;
			if (GetCursorPos(&p)) {
				// Získat rozmìry okna
				glfwGetWindowSize(window, &width, &height);
				centerX = width / 2;
				centerY = height / 2;
				if (isResettingCursor) {
					isResettingCursor = false;
					last_cursor_xpos = centerX;
					last_cursor_ypos = centerY;
				}
				float deltaX = static_cast<float>(p.x - last_cursor_xpos);
				float deltaY = static_cast<float>(p.y - last_cursor_ypos);
				bool isCursorOutOfCenter = p.x != 400 || p.y != 300;

				// Aktualizovat pøedchozí pozici pro další iteraci
				last_cursor_xpos = p.x;
				last_cursor_ypos = p.y;

				// Pokud došlo ke zmìnì pozice, volat onMouseEvent s relativní zmìnou
				if ((deltaX != 0 || deltaY != 0) && isCursorOutOfCenter) {
					camera.onMouseEvent(deltaX, deltaY, true);
					isResettingCursor = true;
					glfwSetCursorPos(window, centerX, centerY);
				}
			}

			// Set Model Matrix
			UpdateModels();

			// Activate shader, set uniform vars
			shader.activate();
			//my_shader.SetUniform("uMx_model", mx_model); // Object local coor space -> World space
			shader.setUniform("uMx_view", mx_view); // World space -> Camera space
			shader.setUniform("uMx_projection", mx_projection); // Camera space -> Screen

			///*
			shader.setUniform("ambient_material", rgb_white);
			shader.setUniform("diffuse_material", rgb_white);
			shader.setUniform("specular_material", rgb_white);
			shader.setUniform("specular_shinines", 5.0f);
			shader.setUniform("light_position", light_position);
			/**/

			//´------------------------------------------------------------------------------------------
			// Freetype Text
			//
			//shader.setUniform("alphabet_texture", 31);
			//shader.setUniform("font_colour", glm::vec3(10.0f, 120.0f, 105.0f));

			//// std::cout << "\n   display_counting: " << display_counting << " ---  display_counting % 10: " << display_counting % 10 << " ---  display_counting / 10 % 10: "
			//	// << display_counting / 10 % 10 << " --- display_counting / 100 % 10: " << display_counting / 100 % 10;

			//unsigned num1 = 333 / 100 % 10; // Left digit.
			//unsigned num2 = 333 / 10 % 10;
			//unsigned num3 = 333 % 10;

			//float advance1 = text_object2.messages[0].alphabet_vec[num1].glyph_advance_x;
			//float advance2 = advance1 + (text_object2.messages[0].alphabet_vec[num2].glyph_advance_x);

			//text_object2.messages[0].characters_quads.resize(vec_size - num_replace);
			//text_object2.messages[0].text_start_x = start_pos;

			//text_object2.process_text_index(text_object2.messages[0], num1, 0); // Important: the number of calls to: process_text_index(...) must = "num_replace_characters"
			//text_object2.process_text_index(text_object2.messages[0], num2, advance1);
			//text_object2.process_text_index(text_object2.messages[0], num3, advance2);

			//text_object2.update_buffer_data_message(text_object2.messages[0], (int)(vec_size - num_replace));

			//text_object2.draw_messages(0);
			//text_object2.draw_alphabets();
			//
			//´------------------------------------------------------------------------------------------


			// Draw the scene
			// - Draw opaque objects
			for (auto& [key, value] : scene_opaque) {
				value.Draw(shader);
			}
			// - Draw transparent objects
			glEnable(GL_BLEND);         // enable blending
			glDisable(GL_CULL_FACE);    // no polygon removal
			glDepthMask(GL_FALSE);      // set Z to read-only
			//// TODO: sort by distance from camera, from far to near
			//for (auto& [key, value] : scene_transparent) {
			//	value.Draw(shader);
			//}
			// Calculate distace from camera for all transparent objects
			for (auto& transparent_pair : scene_transparent_pairs) {
				transparent_pair->second._distance_from_camera = glm::length(camera.position - transparent_pair->second.position);
			}
			// Sort all transparent objects in vector by their distance from camera
			// near to far ... first draw nearest obj ofcourse <<
			std::sort(scene_transparent_pairs.begin(), scene_transparent_pairs.end(), [](std::pair<const std::string, Model>*& a, std::pair<const std::string, Model>*& b) {
				return a->second._distance_from_camera < b->second._distance_from_camera;
				});
			// Draw all transparent objects in sorted order
			for (auto& transparent_pair : scene_transparent_pairs) {
				transparent_pair->second.Draw(shader);
			}
			glDisable(GL_BLEND);
			glEnable(GL_CULL_FACE);
			glDepthMask(GL_TRUE);

			//projectile.drawAllProjectiles(2.0f);

			// === End of frame ===
			// Swap front and back buffers
			glfwSwapBuffers(window);

			// Poll for and process events
			glfwPollEvents();

			// Time/FPS measure end
			auto fps_frame_end_timestamp = std::chrono::steady_clock::now();
			std::chrono::duration<double> fps_elapsed_seconds = fps_frame_end_timestamp - fps_frame_start_timestamp;
			fps_counter_seconds += fps_elapsed_seconds.count();
			fps_counter_frames++;
			if (fps_counter_seconds >= 1) {
				//std::cout << fps_counter_frames << " FPS\n";
				std::stringstream ss;
				ss << fps_counter_frames << " FPS";
				glfwSetWindowTitle(window, ss.str().c_str());
				fps_counter_seconds = 0;
				fps_counter_frames = 0;
			}
		}
	}
	catch (std::exception const& e) {
		std::cerr << "App failed : " << e.what() << "\n";
		return EXIT_FAILURE;
	}

	GetInformation();
	std::cout << "Finished OK...\n";
	return EXIT_SUCCESS;
}

App::~App()
{
	// clean-up
	shader.clear();

	if (window) {
		glfwDestroyWindow(window);
	}
	glfwTerminate();

	exit(EXIT_SUCCESS);
	std::cout << "Bye...\n";
}

void App::UpdateProjectionMatrix(void)
{
	if (window_height < 1) window_height = 1; // avoid division by 0

	float ratio = static_cast<float>(window_width) / window_height;

	mx_projection = glm::perspective(
		glm::radians(FOV),   // The vertical Field of View
		ratio,               // Aspect Ratio. Depends on the size of your window.
		0.1f,                // Near clipping plane. Keep as big as possible, or you'll get precision issues.
		20000.0f             // Far clipping plane. Keep as little as possible.
	);
}

void App::GetInformation()
{
	std::cout << "\n=================== :: GL Info :: ===================\n";
	std::cout << "GL Vendor:\t" << glGetString(GL_VENDOR) << "\n";
	std::cout << "GL Renderer:\t" << glGetString(GL_RENDERER) << "\n";
	std::cout << "GL Version:\t" << glGetString(GL_VERSION) << "\n";
	std::cout << "GL Shading ver:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n\n";

	GLint profile;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
	if (const auto errorCode = glGetError()) {
		std::cout << "[!] Pending GL error while obtaining profile: " << errorCode << "\n";
		//return;
	}
	if (profile & GL_CONTEXT_CORE_PROFILE_BIT) {
		std::cout << "Core profile" << "\n";
	}
	else {
		std::cout << "Compatibility profile" << "\n";
	}
	std::cout << "=====================================================\n\n";
}