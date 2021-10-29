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
#include "ModulePhysics.h"
#include "External/Box2D/Box2D/Box2D.h"


Player::Player() : Module()
{
	name.Create("player");
}

// Destructor
Player::~Player()
{}

bool Player::Awake()
{
	LOG("Loading Player");
	bool ret = true;

	return ret;
}

bool Player::Start()
{
	player = new PlayerObj;
	player->playerBody = app->physics->CreateRectangle(92, 368, 30, 60, b2_dynamicBody);
	player->isDead = false;
	player->IsJumping = false;

	


	return true;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) player->playerBody->body->SetLinearVelocity({ 1, 0 });
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) player->playerBody->body->SetLinearVelocity({ -1, 0 });

	return true;
}

bool Player::PostUpdate()
{
	bool ret = true;
	return ret;
}

// Called before quitting
bool Player::CleanUp()
{
	LOG("Freeing Player");

	return true;
}
