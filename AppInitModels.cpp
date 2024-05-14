#include "App.h"

bool lastStateOfholdItem;

Model* App::CreateModel(std::string name, std::string obj, std::string tex, bool is_opaque, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation)
{
	std::filesystem::path modelpath("./assets/obj/" + obj);
	std::filesystem::path texturepath("./assets/textures/" + tex);
	//float _scale = HEGHTMAP_SCALE;
	bool use_aabb = false;
	auto model = new Model(name, modelpath, texturepath, position, scale, rotation, false, use_aabb);
	/*auto model = Model(modelpath, texturepath);

	model.position = position;
	model.scale = scale;
	model.rotation = rotation;*/

	if (is_opaque) {
		scene_opaque.insert({ name, model });
	}
	else {
		scene_transparent.insert({ name, model });
	}

	return model;
}

// Load models, load textures, load shaders, initialize level, etc.
void App::InitAssets()
{

	//std::filesystem::path VS_path("./assets/shaders/final.vert");
	//std::filesystem::path FS_path("./assets/shaders/final.frag");
	
	//std::filesystem::path FS_path("./assets/shaders/finalFlash.frag");

	std::filesystem::path VS_path("./assets/shaders/redo.vert");
	std::filesystem::path FS_path("./assets/shaders/redo.frag");

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

	// Megaphone
	position = glm::vec3(4.0f, 25.0f, 2.0f);
	scale = glm::vec3(0.5f, 0.5f, 0.5f);
	rotation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	sound_model = CreateModel("obj_megaphone", "MegaPhone.obj", "MegaPhone_basecolor.png", true, position, scale, rotation);
	//CreateModel("obj_megaphone", "MegaPhone.obj", "Glass.png", true, position, scale, rotation);
	//scene_opaque.find("obj_megaphone")->second.canBeHold = true;
	scene_opaque.find("obj_megaphone")->second->canBeHold = true;

	// TEAPOT
	position = glm::vec3(2.0f, 25.0f, 2.0f);
	scale = glm::vec3(0.1f, 0.1f, 0.1f);
	rotation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	CreateModel("obj_teapot", "teapot_tri_vnt.obj", "Glass.png", false, position, scale, rotation);
	//scene_transparent.find("obj_teapot")->second.canBeHold = true;
	scene_transparent.find("obj_teapot")->second->canBeHold = true;

	position = glm::vec3(-1.0f, 1.0f, 1.0f);
	//scale = glm::vec3(0.005f, 0.005f, 0.005f);
	scale = glm::vec3(0.01f, 0.01f, 0.01f);
	rotation = glm::vec4(1.0f, 0.0f, 10.0f, 0.0f);
	//CreateModel("obj_gun", "Beretta_M9.obj", "Beretta_M9.mtl", false, position, scale, rotation);
	//CreateModel("obj_gun", "Beretta_M9.obj", "Grey.png", false, position, scale, rotation);
	CreateModel("obj_gun", "Beretta_M9.obj", "Grey.png", false, position, scale, rotation);
	// right init rotation
	/*scene_transparent.find("obj_gun")->second.rotation = glm::vec4(0.0f, 0.0f, 1.0f, -90);
	scene_transparent.find("obj_gun")->second.canBeHold = true;
	scene_transparent.find("obj_gun")->second.isItemHeld = true;*/
	scene_transparent.find("obj_gun")->second->rotation = glm::vec4(0.0f, 0.0f, 1.0f, -90);
	scene_transparent.find("obj_gun")->second->canBeHold = true;
	scene_transparent.find("obj_gun")->second->isItemHeld = true;

	holdItem = true;
	lastStateOfholdItem = holdItem;

	// Generate MAZE from boxes 
	/*cv::Mat maze = cv::Mat(10, 25, CV_8U);
	MazeGenerate(maze);*/

	// HEIGHTMAP
	//std::filesystem::path heightspath("./assets/textures/heights.png");
	//std::filesystem::path heightspath("./assets/textures/heightmap2.jpeg");
	std::filesystem::path heightspath("./assets/textures/heightmap3.png");
	std::filesystem::path texturepath("./assets/textures/tex_256.png");
	/*auto model = Model(heightspath, texturepath, true);
	model.position = glm::vec3(-60.0f, 0.0f, -60.0f);
	model.scale = glm::vec3(0.1f, 0.1f, 0.1f);
	model.rotation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	scene_opaque.insert({ "hightmap", model });*/


	position = glm::vec3(-HEIGHTMAP_SHIFT, 0.0f, -HEIGHTMAP_SHIFT);
	scale = glm::vec3(0.1f, 0.1f, 0.1f);
	//float scale2 = HEGHTMAP_SCALE;
	rotation = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	auto obj_heightmap = new Model("heightmap", heightspath, texturepath, position, scale, rotation, true, false);
	scene_opaque.insert({ "obj_heightmap", obj_heightmap });

	//std::cout << "Drop item!\n";

	_heights = &obj_heightmap->_heights;
	//_heights = &model._heights;

	// == for TRANSPARENT OBJECTS sorting ==	
	for (auto i = scene_transparent.begin(); i != scene_transparent.end(); i++) {
		scene_transparent_pairs.push_back(&*i);
	}

}

glm::vec3 updateGunPosition(Camera& camera)
{
	// Offset pozice zbranì od pozice kamery podél smìru pohledu kamery
	glm::vec3 gunPosition = camera.getPosition() + camera.getFront() * 0.5f + glm::vec3(0.4f, 0.0f, 0.0f);

	return gunPosition;
}

glm::vec3 updateGunPositionToMiddleOfScreen(Camera& camera)
{
	// Fixní vzdálenost zbranì od hráèe
	float gunDistance = 1.0f;
	// Vypoèítání pozice zbranì ve støedu obrazovky ve fixní vzdálenosti od hráèe
	glm::vec3 gunPosition = camera.getPosition() + camera.getFront() * gunDistance;

	return gunPosition;
}


glm::vec3 updateGunPosition2(Camera& camera)
{
	glm::vec3 handOffset = glm::vec3(0.4f, 0.1f, 0.1f);
	// Offset pozice zbranì od pozice kamery podél smìru pohledu kamery
	glm::vec3 gunOffset = camera.getFront() * 0.5f; // Vzdálenost od kamery
	glm::vec3 gunPosition = camera.getPosition() + gunOffset - handOffset;

	// Aktualizace pozice zbranì
	return gunPosition;
}

glm::vec4 updateGunRotation2(Camera& camera)
{
	// Vytvoøení rotace zbranì na základì rotace kamery
	// Mùžete experimentovat s úhlem rotace, aby zbraò vypadala, že je držena ve správné pozici
	//glm::vec4 gunRotation = glm::vec4(0.0f, 0.0f, 1.0f, -90.0f);
	glm::vec4 gunRotation = glm::vec4(camera.getFront(), glm::radians(-90.0f));

	// Aktualizace rotace zbranì
	return gunRotation;
}



void App::UpdateModels()
{
	glm::vec3 position{};
	glm::vec3 scale{};
	glm::vec4 rotation{};

	rotation = glm::vec4(0.0f, 1.0f, 0.0f, 45 * glfwGetTime());
	//scene_transparent.find("obj_teapot")->second.rotation = rotation;
	scene_transparent.find("obj_teapot")->second->rotation = rotation;

	// Nastavení nové pozice pro zbraò
	//scene_transparent.find("obj_gun")->second.position = updateGunPositionToMiddleOfScreen(camera);
	//scene_transparent.find("obj_gun")->second.position = updateGunPosition2(camera);
	//scene_transparent.find("obj_gun")->second.rotation = updateGunRotation2(camera);

	// TODO -> OPRAVA:
	// když otoèím kamerou doleva - zbraò se naklápí nahodu,
	// když kamera (myš) doprava  - zbraò se naklápí dolù,
	// když kamera nahoru		  - zbraò se otáèí doprava,
	// když kamera dolù			  - zbraò se naklápí doleva
	//scene_transparent.find("obj_gun")->second.rotation = glm::vec4(cameraFront.x, cameraFront.y, cameraFront.z, -90.0f);
	// Upravení smìrového vektoru kamery
	glm::vec3 adjustedCameraFront = camera.getFront();
	adjustedCameraFront.y *= -1.0f; // Otoèení osy Y (zmìna znaménka) pro obrácenou rotaci zbranì
	scene_transparent.find("obj_gun")->second->rotation = glm::vec4(adjustedCameraFront, -90.0f);

	//scene_transparent.find("obj_gun")->second.rotation = glm::vec4(cameraFront.x, cameraFront.y, cameraFront.z, -90.0f + camera.getYaw());
	//scene_transparent.find("obj_gun")->second.rotation = glm::vec4(camera.getYaw(), camera.getPitch(), 1.0f, -90.0f);

	/*glm::vec3 viewDirection = camera.getFront();
	float angle = glm::degrees(atan2(viewDirection.y, viewDirection.x));
	scene_transparent.find("obj_gun")->second.rotation = glm::vec4(0.0f, 0.0f, 1.0f, angle);*/

	//RemoveModel("obj_teapot");

	// button "e" pressed ... later needs repair this pick up item logic :(
			// changed
	Model* heldItem = findHeldItem();
	if (lastStateOfholdItem != holdItem) {
		if (holdItem == false) {
			//std::cout << "Drop item!\n";
			// check if you are/were holding item
			// if not check -> error -1073741819 -> unauhorized memory write
			if (heldItem != nullptr) {
				heldItem->isItemHeld = false;
			}
		}
		else {
			//std::cout << "Pick up item!\n";
			holdNewItem();
		}
		// current state
		lastStateOfholdItem = holdItem;
	}

	if (heldItem != nullptr) {
		heldItem->position = updateGunPositionToMiddleOfScreen(camera);
	}


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

//void printModels(const std::map<std::string, Model>& models) {
//	for (const auto& pair : models) {
//		const Model& model = pair.second;
//		std::cout << "Position: (" << model.position.x << ", " << model.position.y << ", " << model.position.z << ")\n";
//	}
//}

Model* App::findClosestModel(glm::vec3& cameraPosition) {

	//std::cout << "findClosestModel() called!\n";

	float shortestDistance = std::numeric_limits<float>::max();
	Model* closestModel = nullptr;

	// Projít všechny modely v mapì scene_opaque
	for (auto& pair : scene_opaque) {
		if (pair.second->canBeHold) {
			float distance = glm::length(pair.second->position - cameraPosition);
			if (distance < shortestDistance) {
				shortestDistance = distance;
				closestModel = pair.second;
				//std::cout << "Item founded!\n";
			}
		}
	}

	// Projít všechny modely v mapì scene_transparent
	for (auto& pair : scene_transparent) {
		if (pair.second->canBeHold) {
			float distance = glm::length(pair.second->position - cameraPosition);
			if (distance < shortestDistance) {
				shortestDistance = distance;
				closestModel = pair.second;
				//std::cout << "Item founded!\n";
			}
		}
	}

	return closestModel;
}

Model* App::findClosestModelInItemPickUpRange(glm::vec3& cameraPosition) {

	float shortestDistance = std::numeric_limits<float>::max();
	Model* closestModel = nullptr;
	float distance = 0;

	// std::numeric_limits<float>::epsilon()
	float myEpsilon = 0.001f; // Nastavte vlastní epsilon podle potøeby


	for (auto& pair : scene_opaque) {
		if (pair.second->canBeHold) {
			distance = glm::length(pair.second->position - cameraPosition);
			if (distance < shortestDistance && (distance - itemPickUpRange) < myEpsilon) {
				shortestDistance = distance;
				closestModel = pair.second;
			}
		}
	}

	for (auto& pair : scene_transparent) {
		if (pair.second->canBeHold) {
			distance = glm::length(pair.second->position - cameraPosition);
			if (distance < shortestDistance && (distance - itemPickUpRange) < myEpsilon) {
				shortestDistance = distance;
				closestModel = pair.second;
			}
		}
	}

	return closestModel;
}





Model* App::findHeldItem() {
	for (auto& pair : scene_opaque) {
		if (pair.second->isItemHeld) {
			return pair.second;
		}
	}
	for (auto& pair : scene_transparent) {
		if (pair.second->isItemHeld) {
			return pair.second;
		}
	}
	return nullptr;
}

void App::holdNewItem() {
	//Model* closestModel = findClosestModel(camera.position);
	Model* closestModel = findClosestModelInItemPickUpRange(camera.position);
	// Pokud byl nalezen nejbližší model
	if (closestModel != nullptr) {
		std::cout << "item found!\n";
		// last held obj -> "drop"
		// TODO : Gravity drop item
		/*Model* lastHeldItem = findHeldItem();
		if (lastHeldItem != nullptr) {
			std::cout << "item droped!\n";
			lastHeldItem->isItemHeld = false;
		}*/
		// je držen
		closestModel->isItemHeld = true;
	}
}
