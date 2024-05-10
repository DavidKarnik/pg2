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

	//std::filesystem::path VS_path("./assets/shaders/finalPlusText.vert");
	//std::filesystem::path FS_path("./assets/shaders/finalPlusText.frag");
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
	position = glm::vec3(4.0f, 4.0f, 2.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	rotation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	CreateModel("obj_megaphone", "MegaPhone.obj", "MegaPhone_basecolor.png", true, position, scale, rotation);
	//CreateModel("obj_megaphone", "MegaPhone.obj", "Glass.png", true, position, scale, rotation);

	// Megaphone
	position = glm::vec3(2.0f, 2.0f, 2.0f);
	scale = glm::vec3(0.2f, 0.2f, 0.2f);
	rotation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	CreateModel("obj_teapot", "teapot_tri_vnt.obj", "Glass.png", false, position, scale, rotation);

	position = glm::vec3(-1.0f, 1.0f, 1.0f);
	scale = glm::vec3(0.005f, 0.005f, 0.005f);
	rotation = glm::vec4(1.0f, 0.0f, 10.0f, 0.0f);
	//CreateModel("obj_gun", "Beretta_M9.obj", "Beretta_M9.mtl", false, position, scale, rotation);
	//CreateModel("obj_gun", "Beretta_M9.obj", "Grey.png", false, position, scale, rotation);
	CreateModel("obj_gun", "Beretta_M9.obj", "Grey.png", false, position, scale, rotation);
	// right init rotation
	scene_transparent.find("obj_gun")->second.rotation = glm::vec4(0.0f, 0.0f, 1.0f, -90);

	// Generate MAZE from boxes 
	/*cv::Mat maze = cv::Mat(10, 25, CV_8U);
	MazeGenerate(maze);*/

	// HEIGHTMAP
	//std::filesystem::path heightspath("./assets/textures/heights.png");
	//std::filesystem::path heightspath("./assets/textures/heightmap2.jpeg");
	std::filesystem::path heightspath("./assets/textures/heightmap3.png");
	std::filesystem::path texturepath("./assets/textures/tex_256.png");
	auto model = Model(heightspath, texturepath, true);
	model.position = glm::vec3(1.0f, 1.0f, 1.0f);
	model.scale = glm::vec3(0.1f, 0.1f, 0.1f);
	model.rotation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	scene_opaque.insert({ "hightmap", model });

	// == for TRANSPARENT OBJECTS sorting ==	
	for (auto i = scene_transparent.begin(); i != scene_transparent.end(); i++) {
		scene_transparent_pairs.push_back(&*i);
	}

}

glm::vec3 updateGunPosition(Camera& camera)
{
	// Offset pozice zbran� od pozice kamery pod�l sm�ru pohledu kamery
	glm::vec3 gunPosition = camera.getPosition() + camera.getFront() * 0.5f + glm::vec3(0.4f, 0.0f, 0.0f);

	// Aktualizace pozice zbran�
	return gunPosition;
}

glm::vec3 updateGunPositionToMiddleOfScreen(Camera& camera)
{
	// Fixn� vzd�lenost zbran� od hr��e
	float gunDistance = 1.0f;

	// Vypo��t�n� pozice zbran� ve st�edu obrazovky ve fixn� vzd�lenosti od hr��e
	glm::vec3 gunPosition = camera.getPosition() + camera.getFront() * gunDistance;

	// Aktualizace pozice zbran�
	return gunPosition;
}


glm::vec3 updateGunPosition2(Camera& camera)
{
	glm::vec3 handOffset = glm::vec3(0.4f, 0.1f, 0.1f);
	// Offset pozice zbran� od pozice kamery pod�l sm�ru pohledu kamery
	glm::vec3 gunOffset = camera.getFront() * 0.5f; // Vzd�lenost od kamery
	glm::vec3 gunPosition = camera.getPosition() + gunOffset - handOffset;

	// Aktualizace pozice zbran�
	return gunPosition;
}

glm::vec4 updateGunRotation2(Camera& camera)
{
	// Vytvo�en� rotace zbran� na z�klad� rotace kamery
	// M��ete experimentovat s �hlem rotace, aby zbra� vypadala, �e je dr�ena ve spr�vn� pozici
	//glm::vec4 gunRotation = glm::vec4(0.0f, 0.0f, 1.0f, -90.0f);
	glm::vec4 gunRotation = glm::vec4(camera.getFront(), glm::radians(-90.0f));

	// Aktualizace rotace zbran�
	return gunRotation;
}



void App::UpdateModels()
{
	glm::vec3 position{};
	glm::vec3 scale{};
	glm::vec4 rotation{};

	rotation = glm::vec4(0.0f, 1.0f, 0.0f, 45 * glfwGetTime());
	scene_transparent.find("obj_teapot")->second.rotation = rotation;

	// Nastaven� nov� pozice pro zbra�
	scene_transparent.find("obj_gun")->second.position = updateGunPositionToMiddleOfScreen(camera);
	//scene_transparent.find("obj_gun")->second.position = updateGunPosition2(camera);
	//scene_transparent.find("obj_gun")->second.rotation = updateGunRotation2(camera);

	// TODO -> OPRAVA:
	// kdy� oto��m kamerou doleva - zbra� se nakl�p� nahodu,
	// kdy� kamera (my�) doprava  - zbra� se nakl�p� dol�,
	// kdy� kamera nahoru		  - zbra� se ot��� doprava,
	// kdy� kamera dol�			  - zbra� se nakl�p� doleva
	//scene_transparent.find("obj_gun")->second.rotation = glm::vec4(cameraFront.x, cameraFront.y, cameraFront.z, -90.0f);
	// Upraven� sm�rov�ho vektoru kamery
	glm::vec3 adjustedCameraFront = camera.getFront();
	adjustedCameraFront.y *= -1.0f; // Oto�en� osy Y (zm�na znam�nka) pro obr�cenou rotaci zbran�
	scene_transparent.find("obj_gun")->second.rotation = glm::vec4(adjustedCameraFront, -90.0f);

	//scene_transparent.find("obj_gun")->second.rotation = glm::vec4(cameraFront.x, cameraFront.y, cameraFront.z, -90.0f + camera.getYaw());
	//scene_transparent.find("obj_gun")->second.rotation = glm::vec4(camera.getYaw(), camera.getPitch(), 1.0f, -90.0f);

	/*glm::vec3 viewDirection = camera.getFront();
	float angle = glm::degrees(atan2(viewDirection.y, viewDirection.x));
	scene_transparent.find("obj_gun")->second.rotation = glm::vec4(0.0f, 0.0f, 1.0f, angle);*/

	//RemoveModel("obj_teapot");
}

void App::RemoveModel(std::string name) {
	// Odeber model z mapy scene_opaque
	auto it_opaque = scene_opaque.find(name);
	if (it_opaque != scene_opaque.end()) {
		scene_opaque.erase(it_opaque);
		std::cout << "\n Nepruhledny model smazan! \n";
	}

	// Odeber model z mapy scene_transparent
	auto it_transparent = scene_transparent.find(name);
	if (it_transparent != scene_transparent.end()) {
		scene_transparent.erase(it_transparent);
		std::cout << "\n Pruhledny model smazan! \n";
	}
}



