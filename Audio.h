#pragma once
#include <map>

#include <glm/glm.hpp>
#include <irrKlang.h>
#include <string>

class Audio
{
public:
	Audio();

	void UpdateListenerPosition(glm::vec3 position, glm::vec3 front, glm::vec3 world_up);

	void PlayMusic3D();
	void UpdateMusicPosition(glm::vec3 position);
	void UpdateMusicVolume(float amount);

	~Audio();
private:
	irrklang::ISoundEngine* engine = nullptr;

	std::map<std::string, irrklang::ISoundSource*> sounds;

	irrklang::ISound* music = nullptr;

	bool walkSwitch = false;
};
