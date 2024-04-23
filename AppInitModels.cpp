#include "App.h"

void App::CreateModel(std::string name, std::string obj, std::string tex, bool is_opaque, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation)
{
	std::filesystem::path modelpath("./assets/obj/" + obj);
	std::filesystem::path texturepath("./assets/textures/" + tex);
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

	std::filesystem::path VS_path("./assets/shaders/final.vert");
	std::filesystem::path FS_path("./assets/shaders/final.frag");
	shader = Shader(VS_path, FS_path);

	// MODELS
	glm::vec3 position{};
	glm::vec3 scale{};
	glm::vec4 rotation{};


	camera = Camera{ glm::vec3(0.0f, 0.0f, 0.0f) };

	// BUNNY
	/*position = glm::vec3(1.0f, 1.0f, 1.0f);
	scale = glm::vec3(0.5f, 0.5f, 0.5f);
	rotation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	CreateModel("obj_bunny", "bunny_tri_vnt.obj", "TextureDouble_A.png", true, position, scale, rotation);*/

	// TEAPOT
	position = glm::vec3(2.0f, 2.0f, 2.0f);
	scale = glm::vec3(0.2f, 0.2f, 0.2f);
	rotation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	CreateModel("obj_teapot", "teapot_tri_vnt.obj", "Glass.png", false, position, scale, rotation);

	position = glm::vec3(-1.0f, 1.0f, 1.0f);
	scale = glm::vec3(0.005f, 0.005f, 0.005f);
	rotation = glm::vec4(1.0f, 0.0f, 10.0f, 0.0f);
	//CreateModel("obj_gun", "Beretta_M9.obj", "Beretta_M9.mtl", false, position, scale, rotation);
	CreateModel("obj_gun", "Beretta_M9.obj", "Glass.png", false, position, scale, rotation);

	// Generate MAZE from boxes 
	/*cv::Mat maze = cv::Mat(10, 25, CV_8U);
	MazeGenerate(maze);*/

	// HEIGHTMAP
	std::filesystem::path heightspath("./assets/textures/heights.png");
	std::filesystem::path texturepath("./assets/textures/tex_256.png");
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

	//scene_transparent.find("obj_gun")->second.position = camera.getPosition() + glm::vec3(0.5f, 0.0f, 0.0f);

	// Získání aktuální rotace kamery
	glm::vec3 cameraFront = camera.getFront();

	// Výpoèet pozice zbranì relativnì k pozici kamery
	glm::vec3 relativeWeaponPosition = glm::vec3(0.5f, 0.0f, 1.0f); // Pøíklad relativní pozice zbranì
	glm::vec3 weaponPosition = camera.getPosition() + relativeWeaponPosition;

	// Nastavení pozice zbranì
	//scene_transparent.find("obj_gun")->second.position = weaponPosition;

	// Nastavení rotace zbranì
	// Výpoèet rotace pro postavení zbranì
	float pitch = atan2(cameraFront.y, sqrt(cameraFront.x * cameraFront.x + cameraFront.z * cameraFront.z));
	float yaw = atan2(-cameraFront.x, -cameraFront.z);

	// Nastavení rotace zbranì
	//scene_transparent.find("obj_gun")->second.rotation = glm::vec4(pitch, yaw, 0.0f, 1.0f);
	scene_transparent.find("obj_gun")->second.rotation = glm::vec4(0.0f, 0.0f, 1.0f, -90);


	//RemoveModel("obj_teapot");
}

void App::RemoveModel(std::string name) {
	// Odeber model z mapy scene_opaque
	auto it_opaque = scene_opaque.find(name);
	if (it_opaque != scene_opaque.end()) {
		scene_opaque.erase(it_opaque);
	}

	// Odeber model z mapy scene_transparent
	auto it_transparent = scene_transparent.find(name);
	if (it_transparent != scene_transparent.end()) {
		scene_transparent.erase(it_transparent);
	}
}