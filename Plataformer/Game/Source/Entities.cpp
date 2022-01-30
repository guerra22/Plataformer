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

Entities::Entities() : Module()
{
	name.Create("Entities");
}

// Destructor
Entities::~Entities()
{}

bool Entities::Awake()
{
	return true;
}

bool Entities::Start()
{
	heart1 = new Heart;
	heart1->heart = app->physics->CreateCircle(497, 270, 7, b2_staticBody);
	heart1->heart->listener = this;
	heart1->heart->type = PhysBody::Type::HEART1;

	heart2 = new Heart;
	heart2->heart = app->physics->CreateCircle(1010, 240, 7, b2_staticBody);
	heart2->heart->listener = this;
	heart2->heart->type = PhysBody::Type::HEART2;

	coin1 = new Coin;
	coin1->coin = app->physics->CreateCircle(400, 240, 7, b2_staticBody);
	coin1->coin->listener = this;
	coin1->coin->type = PhysBody::Type::COIN1;

	coin2 = new Coin;
	coin2->coin = app->physics->CreateCircle(720, 170, 7, b2_staticBody);
	coin2->coin->listener = this;
	coin2->coin->type = PhysBody::Type::COIN2;

	coin3 = new Coin;
	coin3->coin = app->physics->CreateCircle(1120, 230, 7, b2_staticBody);
	coin3->coin->listener = this;
	coin3->coin->type = PhysBody::Type::COIN3;

	checkpoint = new CheckPoint;
	checkpoint->flag = app->physics->CreateCircle(848, 198, 7, b2_staticBody);
	checkpoint->flag->listener = this;
	checkpoint->flag->type = PhysBody::Type::CHECKPOINT;

	heartTexture = app->tex->Load("Assets/textures/Entities/heart.png");
	DoorClosed = app->tex->Load("Assets/textures/Entities/doorClosed.png");
	DoorOpenned = app->tex->Load("Assets/textures/Entities/doorOpenned.png");
	CheckpointTexture = app->tex->Load("Assets/textures/Entities/checkpoint.png");
	coinTexture = app->tex->Load("Assets/textures/Entities/coin.png");
	DamageScreen = app->tex->Load("Assets/textures/DamageScreen.png");

	coinGui = app->tex->Load("Assets/textures/guiEntities/coinGui.png");
	heartGui = app->tex->Load("Assets/textures/guiEntities/heartGui.png");
	emptyCoinGui = app->tex->Load("Assets/textures/guiEntities/coinEmptyGui.png");
	emptyHeartGui = app->tex->Load("Assets/textures/guiEntities/heartEmptyGui.png");

	damageScreenCooldown = 0;
	coinsCollected = 0;

	coin1->isCollected = false;
	coin2->isCollected = false;
	coin3->isCollected = false;

	heart1->isAwake = true;
	heart2->isAwake = true;

	return true;
}

bool Entities::PreUpdate()
{
	return true;
}

bool Entities::Update(float dt)
{

	//Render

	// hearts
	if (heart1->isAwake == true) { 
		if (app->gameState == 1) {
			app->render->DrawTexture(heartTexture, METERS_TO_PIXELS(heart1->heart->body->GetPosition().x - 12), METERS_TO_PIXELS(heart1->heart->body->GetPosition().y - 10),
				NULL, 0.0f, 0);
		}
	}else {
		heart1->heart->body->SetTransform({ 0,0 }, 0.0f);
	}
	if (heart2->isAwake == true) {
		if (app->gameState == 1) {
			app->render->DrawTexture(heartTexture, METERS_TO_PIXELS(heart2->heart->body->GetPosition().x - 12), METERS_TO_PIXELS(heart2->heart->body->GetPosition().y - 10),
				NULL, 0.0f, 0);
		}
	}else {
		heart2->heart->body->SetTransform({ 0,0 }, 0.0f);
	}

	// Door & coins & checkpoint
	if (app->gameState == 1) {
		//door
		if (coinsCollected == 3) { app->render->DrawTexture(DoorOpenned, 1200, 280, NULL, 0.0f, 0); }
		else { app->render->DrawTexture(DoorClosed, 1200, 280, NULL, 0.0f, 0); }
		//checkpoint
		app->render->DrawTexture(CheckpointTexture, 823, 150, NULL, 0.0f, 0);
		//coins
		if (coin1->isCollected == false) { 
			app->render->DrawTexture(coinTexture, 389, 230, NULL, 0.0f, 0); 
		    coin1->coin->body->SetTransform({ PIXEL_TO_METERS(400), PIXEL_TO_METERS(240) }, 0.0f);
		}
		else { coin1->coin->body->SetTransform({ 0, 0 }, 0.0f); }

		if (coin2->isCollected == false) { 
			app->render->DrawTexture(coinTexture, 709, 160, NULL, 0.0f, 0); 
			coin2->coin->body->SetTransform({ PIXEL_TO_METERS(720), PIXEL_TO_METERS(170) }, 0.0f);
		}
		else { coin2->coin->body->SetTransform({ 0, 0 }, 0.0f); }

		if (coin3->isCollected == false) {
			app->render->DrawTexture(coinTexture, 1109, 220, NULL, 0.0f, 0); 
			coin3->coin->body->SetTransform({ PIXEL_TO_METERS(1120), PIXEL_TO_METERS(230) }, 0.0f);
		}
		else { coin3->coin->body->SetTransform({ 0, 0 }, 0.0f); }
		//health GUI
		if (app->player->Health == 1) {
			app->render->DrawTexture(heartGui, 40, 40, NULL, 0.0f, 0);
			app->render->DrawTexture(emptyHeartGui, 70, 40, NULL, 0.0f, 0);
			app->render->DrawTexture(emptyHeartGui, 100, 40, NULL, 0.0f, 0);
		}
		else if (app->player->Health == 2) {
			app->render->DrawTexture(heartGui, 40, 40, NULL, 0.0f, 0);
			app->render->DrawTexture(heartGui, 70, 40, NULL, 0.0f, 0);
			app->render->DrawTexture(emptyHeartGui, 100, 40, NULL, 0.0f, 0);
		}
		else if (app->player->Health == 3) {
			app->render->DrawTexture(heartGui, 40, 40, NULL, 0.0f, 0);
			app->render->DrawTexture(heartGui, 70, 40, NULL, 0.0f, 0);
			app->render->DrawTexture(heartGui, 100, 40, NULL, 0.0f, 0);
		}
		//coins GUI
		if (coinsCollected == 0) {
			app->render->DrawTexture(emptyCoinGui, 1040, 40, NULL, 0.0f, 0);
			app->render->DrawTexture(emptyCoinGui, 1070, 40, NULL, 0.0f, 0);
			app->render->DrawTexture(emptyCoinGui, 1100, 40, NULL, 0.0f, 0);
		}
		else if (coinsCollected == 1) {
			app->render->DrawTexture(coinGui, 1040, 40, NULL, 0.0f, 0);
			app->render->DrawTexture(emptyCoinGui, 1070, 40, NULL, 0.0f, 0);
			app->render->DrawTexture(emptyCoinGui, 1100, 40, NULL, 0.0f, 0);
		}
		else if (coinsCollected == 2) {
			app->render->DrawTexture(coinGui, 1040, 40, NULL, 0.0f, 0);
			app->render->DrawTexture(coinGui, 1070, 40, NULL, 0.0f, 0);
			app->render->DrawTexture(emptyCoinGui, 1100, 40, NULL, 0.0f, 0);
		}
		else if (coinsCollected == 3) {
			app->render->DrawTexture(coinGui, 1040, 40, NULL, 0.0f, 0);
			app->render->DrawTexture(coinGui, 1070, 40, NULL, 0.0f, 0);
			app->render->DrawTexture(coinGui, 1100, 40, NULL, 0.0f, 0);
		}
	}
	// Damage screen
	if (damageScreenCooldown > 0) {
		app->render->DrawTexture(DamageScreen, 0, 0, NULL, 0.0f, 0);
		--damageScreenCooldown;
	}
	
	//Reset state
	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) {
		heart1->heart->body->SetTransform({ PIXEL_TO_METERS(497), PIXEL_TO_METERS(270) }, 0.0f);
		heart2->heart->body->SetTransform({ PIXEL_TO_METERS(1010), PIXEL_TO_METERS(240) }, 0.0f);
		heart1->isAwake = true;
		heart2->isAwake = true;
		coinsCollected = 0;
		coin1->isCollected = false;
		coin2->isCollected = false;
		coin3->isCollected = false;
	}
	if (app->gui->restartProgress > 0) {
		heart1->heart->body->SetTransform({ PIXEL_TO_METERS(497), PIXEL_TO_METERS(270) }, 0.0f);
		heart2->heart->body->SetTransform({ PIXEL_TO_METERS(1010), PIXEL_TO_METERS(240) }, 0.0f);
		heart1->isAwake = true;
		heart2->isAwake = true;
		coinsCollected = 0;
		coin1->isCollected = false;
		coin2->isCollected = false;
		coin3->isCollected = false;
		--app->gui->restartProgress;
	}

	return true;
}

bool Entities::PostUpdate()
{
	return true;
}

// Called before quitting
bool Entities::CleanUp()
{
	return true;
}


void Entities::SaveEntities(pugi::xml_node& save)
{
	save.attribute("heart1awake") = heart1->isAwake;
	save.attribute("heart2awake") = heart2->isAwake;

	save.attribute("coin1collected") = coin1->isCollected;
	save.attribute("coin2collected") = coin2->isCollected;
	save.attribute("coin3collected") = coin3->isCollected;

	save.attribute("coinsCollected") = coinsCollected;
}