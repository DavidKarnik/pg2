#include <iostream>

#include "Audio.h"

#define VOLUME 0.5f

Audio::Audio()
{
	engine = irrklang::createIrrKlangDevice();
	if (!engine) {
		std::cerr << "Sound Engine error\n";
	}

}

void Audio::UpdateListenerPosition(glm::vec3 position, glm::vec3 front, glm::vec3 world_up)
{
	// position of the listener
	irrklang::vec3df _position(position.x, position.y, position.z);
	// the direction the listener looks into
	irrklang::vec3df _look_direction(-front.x, front.y, -front.z);
	// only relevant for doppler effects
	irrklang::vec3df _vel_per_second(0, 0, 0);
	// where 'up' is in your 3D scene
	irrklang::vec3df _up_vector(world_up.x, world_up.y, world_up.z);

	engine->setListenerPosition(_position, _look_direction, _vel_per_second, _up_vector);
}

void Audio::PlayMusic3D(const std::string& soundFile, float volume, bool repeat)
{

	std::string fullPath = "assets/sounds/" + soundFile;
	music = engine->play3D(fullPath.c_str(), irrklang::vec3df(0, 0, 0), repeat, true);
	if (music) {
		music->setMinDistance(1.0f);
		music->setIsPaused(false);
		music->setVolume(volume);
	}
}

void Audio::UpdateMusicPosition(float posOnCircle, float radius)
{
	irrklang::vec3df pos3d(radius * cosf(posOnCircle), 0, radius * sinf(posOnCircle * 0.5f));
	engine->setListenerPosition(irrklang::vec3df(0, 0, 0), irrklang::vec3df(0, 0, 1));
	if (music)
		music->setPosition(pos3d);
}

void Audio::UpdateMusicVolume(float amount)
{
	music->setVolume(amount * VOLUME);
}

void Audio::Walk()
{
	engine->play2D(walk_sound ? "./assets/sounds/step1.wav" : "./assets/sounds/step2.wav");
	walk_sound = !walk_sound;
}

Audio::~Audio()
{
	if (music) {
		music->stop();
		music->drop();
	}
	if (engine) {
		engine->drop();
	}
}
