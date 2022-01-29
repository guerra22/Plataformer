#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Defs.h"
#include "Log.h"
#include "Enemy.h"
#include "ModulePhysics.h"
#include "External/Box2D/Box2D/Box2D.h"
#include "Entities.h"
#include "Animation.h"
#include "Gui.h"

Gui::Gui() : Module()
{
	name.Create("Gui");
}

// Destructor
Gui::~Gui()
{}

bool Gui::Awake()
{
	return true;
}

bool Gui::Start()
{
	app->audio->LoadFx("Assets/audio/fx/buttonSound.wav");
	GuiState = 0;

	//title screen
	titleScreen = app->tex->Load("Assets/textures/guiEntities/title/titleScreen.png");
	startSelected = app->tex->Load("Assets/textures/guiEntities/title/startSelected.png");
	settingsSelected = app->tex->Load("Assets/textures/guiEntities/title/settingsSelected.png");
	exitSelected = app->tex->Load("Assets/textures/guiEntities/title/exitSelected.png");
	creditsSelected = app->tex->Load("Assets/textures/guiEntities/title/creditsSelected.png");
	continueSelected = app->tex->Load("Assets/textures/guiEntities/title/continueSelected.png");
	//settings screen
	settingsScreen = app->tex->Load("Assets/textures/guiEntities/settings/settingsScreen.png");
	backSettingsSelected = app->tex->Load("Assets/textures/guiEntities/settings/backSettingsSelected.png");
	musicSelected = app->tex->Load("Assets/textures/guiEntities/settings/musicSelected.png");
	settingsOff = app->tex->Load("Assets/textures/guiEntities/settings/settingsOff.png");
	settingsOn = app->tex->Load("Assets/textures/guiEntities/settings/settingsOn.png");
	//credits screen
	creditsScreen = app->tex->Load("Assets/textures/guiEntities/credits/creditsScreen.png");
	backCreditsSelected = app->tex->Load("Assets/textures/guiEntities/credits/backCreditsSelected.png");

	return true;
}

bool Gui::PreUpdate()
{
	return true;
}

bool Gui::Update(float dt)
{
	SDL_GetMouseState(&mouseX, &mouseY);

	if (app->gameState == 0) {
		if (GuiState == 0) {
			if ( mouseX > 555 && mouseX < 726 && mouseY > 130 && mouseY < 200) {
				app->render->DrawTexture(startSelected, 0, 0, NULL, 0.0f, 0);
			}
			else if (mouseX > 555 && mouseX < 726 && mouseY > 222 && mouseY < 289) {
				app->render->DrawTexture(continueSelected, 0, 0, NULL, 0.0f, 0);
			}
			else if (mouseX > 555 && mouseX < 726 && mouseY > 300 && mouseY < 370) {
				app->render->DrawTexture(settingsSelected, 0, 0, NULL, 0.0f, 0);
			}
			else if (mouseX > 580 && mouseX < 700 && mouseY > 380 && mouseY < 430) {
				app->render->DrawTexture(exitSelected, 0, 0, NULL, 0.0f, 0);
			}
			else if (mouseX > 40 && mouseX < 215 && mouseY > 370 && mouseY < 440) {
				app->render->DrawTexture(creditsSelected, 0, 0, NULL, 0.0f, 0);
			}
			else {
				app->render->DrawTexture(titleScreen, 0, 0, NULL, 0.0f, 0);
			}
		}
	}
	return true;
}

bool Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool Gui::CleanUp()
{
	return true;
}
