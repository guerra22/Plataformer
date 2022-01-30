#ifndef __GUI__
#define __GUI__

#include "Module.h"
#include "ModulePhysics.h"
#include "Box2D/Box2D/Box2D.h"
#include "Module.h"
#include "Globals.h"
#include "Point.h"
#include "Animation.h"
#include "Textures.h"
#include "Render.h"


class Gui : public Module
{
public:
	Gui();

	// Destructor
	virtual ~Gui();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// GUI State  0-Title 1-Settings 2-Credits 3-PauseGameplay 4-Pause 5-Finish
	int GuiState;
	// restart progress
	int restartProgress;

	//mosue position
	int mouseX;
	int mouseY;

	// game closer
	bool ret;

	// title Screen
	SDL_Texture* titleScreen;
	SDL_Texture* startSelected;
	SDL_Texture* settingsSelected;
	SDL_Texture* exitSelected;
	SDL_Texture* creditsSelected;
	SDL_Texture* continueSelected;
	// Settings screen
	SDL_Texture* settingsScreen;
	SDL_Texture* backSettingsSelected;
	SDL_Texture* musicSelected;
	SDL_Texture* settingsOff;
	SDL_Texture* settingsOn;
	// Credits Screen
	SDL_Texture* creditsScreen;
	SDL_Texture* backCreditsSelected;
	// Pause Screen
	SDL_Texture* pauseGameplay;
	SDL_Texture* pauseGameplaySelected;
	SDL_Texture* backPauseSelected;
	SDL_Texture* exitPauseSelected;
	SDL_Texture* pauseScreen;
	SDL_Texture* resumePauseSelected;
	SDL_Texture* settingsPauseSelected;
	// Finish Screen
	SDL_Texture* exitFinishSelected;
	SDL_Texture* finishScreen;
	SDL_Texture* loadFinishScreen;
	SDL_Texture* restartFinishScreen;

};

#endif