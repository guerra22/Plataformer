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
	heart1->heart = app->physics->CreateCircle(40, 300, 7, b2_staticBody);
	heart1->heart->listener = this;
	heart1->heart->type = PhysBody::Type::HEART;
	heart1->idleHeart->PushBack({ 0, 0, 24, 22 });
	heart1->idleHeart.loop = false;
	heart1->idleHeart.mustFlip = true;
	heart1->idleHeart.speed = 0.01f;

	heart2 = new Heart;
	heart2->heart = app->physics->CreateCircle(50, 300, 7, b2_staticBody);
	heart2->heart->listener = this;
	heart2->heart->type = PhysBody::Type::HEART;
	heart2->idleHeart->PushBack({ 0, 0, 22, 22 });
	heart2->idleHeart.loop = false;
	heart2->idleHeart.mustFlip = true;
	heart2->idleHeart.speed = 0.01f;

	heartTexture = app->tex->Load("Assets/textures/Entities/heart.png");

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
	if (heart1->isAwake == true) { 
		app->render->DrawTexture(heartTexture, heart1->heart->body->GetPosition().x, heart1->heart->body->GetPosition().y, &(currentHeartAnim->GetCurrentFrame()), 1); 
	}
	if (heart2->isAwake == true) {
		app->render->DrawTexture(heartTexture, heart2->heart->body->GetPosition().x, heart2->heart->body->GetPosition().y, &(currentHeartAnim->GetCurrentFrame()), 1);
	}

	//Load State
	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT) {
		heart1->isAwake = app->heart1Awake;
		heart2->isAwake = app->heart2Awake;
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