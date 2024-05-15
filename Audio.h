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

	void PlayMusic3D(const std::string& soundFile, float, bool);
	void UpdateMusicPosition(float, float);
	void UpdateMusicVolume(float amount);

	void Walk();

	~Audio();
private:
	irrklang::ISoundEngine* engine = nullptr;

	irrklang::ISound* music = nullptr;

	bool walkSwitch = false;
};
