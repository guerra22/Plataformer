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
    //Initializing player struct data
    p = new Player1;
    pState = IDLE;
 
    p->player = app->physics->CreateCircle(20, 300, 7, b2_dynamicBody);
    p->player->body->SetFixedRotation(true);
    p->player->type = PhysBody::Type::PLAYER;
    //Idle anim
    p->idlePlayerAnim.PushBack({ 8, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 86, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 164, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 242, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 320, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 398, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 476, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 554, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 632, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 710, 17, 50, 50 });
    p->idlePlayerAnim.PushBack({ 788, 17, 50, 50 });
    p->idlePlayerAnim.loop = false;
    p->idlePlayerAnim.mustFlip = true;
    p->idlePlayerAnim.speed = 0.01f;
    //Walking anim
    p->walkingPlayerAnim.PushBack({ 8, 79, 50, 50 });
    p->walkingPlayerAnim.PushBack({ 86, 77, 50, 50 });
    p->walkingPlayerAnim.PushBack({ 164, 78, 50, 50 });
    p->walkingPlayerAnim.PushBack({ 242, 81, 50, 50 });
    p->walkingPlayerAnim.PushBack({ 320, 79, 50, 50 });
    p->walkingPlayerAnim.PushBack({ 398, 77, 50, 50 });
    p->walkingPlayerAnim.PushBack({ 476, 78, 50, 50 });
    p->walkingPlayerAnim.PushBack({ 554, 81, 50, 50 });
    p->walkingPlayerAnim.loop = true;
    p->walkingPlayerAnim.mustFlip = true;
    p->walkingPlayerAnim.speed = 0.1f;
    //Jump anim
    p->jumpingPlayerAnim.PushBack({ 8, 188, 50, 50 });
    p->jumpingPlayerAnim.loop = true;
    p->walkingPlayerAnim.mustFlip = true;
    p->jumpingPlayerAnim.speed = 0.1f;
    //Death anim
    p->deathPlayerAnim.PushBack({ 5, 133, 50, 50 });
    p->deathPlayerAnim.PushBack({ 79, 140, 50, 50 });
    p->deathPlayerAnim.PushBack({ 156, 141, 50, 50 });
    p->deathPlayerAnim.PushBack({ 233, 142, 50, 50 });
    p->deathPlayerAnim.loop = false;
    p->deathPlayerAnim.mustFlip = true;
    p->deathPlayerAnim.speed = 1.0f;

    playerTexture = app->tex->Load("Assets/textures/player/4state.png");

    show = false;
	p->godmode = false;

	return true;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{
	//---------------------------------------
    switch (pState)
    {
    case IDLE:
        currentAnim = &p->idlePlayerAnim;
        break;
    case WALK:
        currentAnim = &p->walkingPlayerAnim;
        break;
    case JUMP:
        currentAnim = &p->jumpingPlayerAnim;
        break;
    case DEATH:
        currentAnim = &p->deathPlayerAnim;
        break;
    }

    if (app->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) { show = true; }
	if (app->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT) { p->godmode = true; }

    if (show == true)
    {
        app->render->DrawTexture(playerTexture, METERS_TO_PIXELS(p->player->body->GetPosition().x - 16), METERS_TO_PIXELS(p->player->body->GetPosition().y) - 26,
            &(currentAnim->GetCurrentFrame()), 1);
    }

    bool ret = true;
    //Player movement
    maxSpeedX = 1;
    minSpeedX = -1;
  
	if (p->godmode == true)
	{
		if ((app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) && (p->player->body->GetLinearVelocity().x <= maxSpeedX))
		{
			p->player->body->SetLinearVelocity({ 1, p->player->body->GetLinearVelocity().y });
			pState = WALK;
			p->walkingPlayerAnim.Update();
			p->idlePlayerAnim.Reset();
		}
		if ((app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) && (p->player->body->GetLinearVelocity().x >= minSpeedX))
		{
			p->player->body->SetLinearVelocity({ -1, p->player->body->GetLinearVelocity().y });
			pState = WALK;
			p->walkingPlayerAnim.Update();
			p->idlePlayerAnim.Reset();
		}
		if ((app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) && (p->player->body->GetLinearVelocity().y < maxSpeedX))
		{
			p->player->body->ApplyLinearImpulse({ 0.0f, -0.1f }, { 0, 0 }, true);
		}
	}
	else {
		if ((app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) && (p->player->body->GetLinearVelocity().x <= maxSpeedX))
		{
			p->player->body->SetLinearVelocity({ 1, p->player->body->GetLinearVelocity().y });
			pState = WALK;
			p->walkingPlayerAnim.Update();
			p->idlePlayerAnim.Reset();
		}
		//Left
		if ((app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) && (p->player->body->GetLinearVelocity().x >= minSpeedX))
		{
			p->player->body->SetLinearVelocity({ -1, p->player->body->GetLinearVelocity().y });
			pState = WALK;
			p->walkingPlayerAnim.Update();
			p->idlePlayerAnim.Reset();	
		}
		//Jump
		if ((app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) && (p->player->body->GetLinearVelocity().y == 0))
		{
			p->player->body->ApplyLinearImpulse({ 0.0f, -0.25f }, { 0, 0 }, true);
			pState = JUMP;
			p->jumpingPlayerAnim.Update();
			p->idlePlayerAnim.Reset();
			p->walkingPlayerAnim.Reset();
		}
	}


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


void Player::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA->type == PhysBody::Type::PLAYER && bodyB->type == PhysBody::Type::FLOOR)
	{
        p->player->body->SetTransform({ PIXEL_TO_METERS(20), PIXEL_TO_METERS(300) }, 0.0f);
	}
}