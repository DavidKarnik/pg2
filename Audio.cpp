#include <iostream>

#include "Audio.h"

#define VOLUME 0.5f

Audio::Audio()
{
	engine = irrklang::createIrrKlangDevice();
	if (!engine) {
		std::cerr << "Sound Engine error\n";
	}

	// Init SFXs
	irrklang::ISoundSource* snd_shoot = engine->addSoundSourceFromFile("resources/sfx/bow.ogg");
	snd_shoot->setDefaultVolume(0.45f);
	sounds.insert({ "snd_shoot", snd_shoot });

	irrklang::ISoundSource* snd_hit = engine->addSoundSourceFromFile("resources/sfx/hit.mp3");
	sounds.insert({ "snd_hit", snd_hit });

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

void Audio::PlayMusic3D()
{
	music = engine->play3D("assets/sounds/teapot.wav", irrklang::vec3df(0, 0, 0), true, true);
	if (music) {
		music->setMinDistance(1.0f);
		music->setIsPaused(false);
		music->setVolume(VOLUME);
	}
}

void Audio::UpdateMusicPosition(glm::vec3 position)
{
	irrklang::vec3df _position(position.x, position.y, position.z);
	music->setPosition(_position);
}

void Audio::UpdateMusicVolume(float amount)
{
	music->setVolume(amount * VOLUME);
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
