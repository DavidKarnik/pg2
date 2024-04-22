#include "App.h"

void App::CreateModel(std::string name, std::string obj, std::string tex, bool is_opaque, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation)
{
	std::filesystem::path modelpath("./resources/objects/" + obj);
	std::filesystem::path texturepath("./resources/textures/" + tex);
	auto model = Model(modelpath, texturepath);

	model.position = position;
	model.scale = scale;
	model.rotation = rotation;

	if (is_opaque) {
		scene_opaque.insert({ name, model });
	}
	else {
		scene_transparent.insert({ name, model });
	}
}

// Load models, load textures, load shaders, initialize level, etc.
void App::InitAssets()
{
	// == SHADERS ==
	// Load shaders and create ShaderProgram
	///*
	std::filesystem::path VS_path("./resources/bruh.vert");
	std::filesystem::path FS_path("./resources/bruh.frag");
	/**/
	/*
	std::filesystem::path VS_path("./resources/all.vert");
	std::filesystem::path FS_path("./resources/all.frag");
	/**/
	my_shader = ShaderProgram(VS_path, FS_path);

	// == MODELS ==
	glm::vec3 position{};
	glm::vec3 scale{};
	glm::vec4 rotation{};

	//CreateModel("cube_triangles.obj", "box_rgb888.png", true);
	//CreateModel("cube_triangles_normals_tex.obj", "TextureDouble_A.png", true);
	//CreateModel("sphere_tri_vnt.obj", "box_rgb888.png", true);
	//CreateModel("bunny_tri_vn.obj", "box_rgb888.png", true); // big
	///*

	// BUNNY
	position = glm::vec3(1.0f, 1.0f, 1.0f);
	scale = glm::vec3(0.5f, 0.5f, 0.5f);
	rotation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	CreateModel("obj_bunny", "bunny_tri_vnt.obj", "TextureDouble_A.png", true, position, scale, rotation);

	// TEAPOT
	position = glm::vec3(2.0f, 2.0f, 2.0f);
	scale = glm::vec3(0.2f, 0.2f, 0.2f);
	rotation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	CreateModel("obj_teapot", "teapot_tri_vnt.obj", "Glass.png", false, position, scale, rotation);
	/**/

	// == MAZE ==
	///*
	cv::Mat maze = cv::Mat(10, 25, CV_8U);
	MazeGenerate(maze);
	/**/

	// == HEIGHTMAP ==
	std::filesystem::path heightspath("./resources/textures/heights.png");
	std::filesystem::path texturepath("./resources/textures/tex_256.png");
	auto model = Model(heightspath, texturepath, true);
	model.position = glm::vec3(1.0f, 1.0f, 1.0f);
	model.scale = glm::vec3(0.1f, 0.1f, 0.1f);
	model.rotation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	scene_opaque.insert({ "hightmap", model });
}

void App::UpdateModels()
{
	glm::vec3 position{};
	glm::vec3 scale{};
	glm::vec4 rotation{};

	rotation = glm::vec4(0.0f, 1.0f, 0.0f, 45 * glfwGetTime());
	scene_transparent.find("obj_teapot")->second.rotation = rotation;
}