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


Enemy::Enemy() : Module()
{
    name.Create("enemy");
}

// Destructor
Enemy::~Enemy()
{}

bool Enemy::Awake()
{
    LOG("Loading Player");
    bool ret = true;

    return ret;
}

bool Enemy::Start()
{
    //Initializing flying enemy struct data
    flyingEnemy = new FlyingEnemy;
    eflyingState = IDLE;
    flyingEnemy->enemy = app->physics->CreateCircle(100, 300, 7, b2_kinematicBody);
    flyingEnemy->enemy->body->SetFixedRotation(true);
	flyingEnemy->enemy->listener = this;
    flyingEnemy->enemy->type = PhysBody::Type::ENEMY_F;
    //Idle anim
    flyingEnemy->idleAnim.PushBack({ 4, 347, 50, 50 });
    flyingEnemy->idleAnim.PushBack({ 52, 346, 50, 50 });
    flyingEnemy->idleAnim.PushBack({ 103, 345, 50, 50 });
    flyingEnemy->idleAnim.loop = false;
    flyingEnemy->idleAnim.mustFlip = true;
    flyingEnemy->idleAnim.speed = 0.01f;
    //Death anim
    flyingEnemy->deathAnim.PushBack({ 148, 347, 50, 50 });
    flyingEnemy->deathAnim.PushBack({ 196, 346, 50, 50 });
    flyingEnemy->deathAnim.PushBack({ 247, 345, 50, 50 });
    flyingEnemy->deathAnim.loop = false;
    flyingEnemy->deathAnim.mustFlip = true;
    flyingEnemy->deathAnim.speed = 1.0f;
	//texture and render
    flyingEnemyTexture = app->tex->Load("Assets/textures/Tileset.png");
	flyingEnemy->show = false;


	//Initializing land enemy struct data
	landEnemy = new LandEnemy;
	elandState = IDLE;
	landEnemy->enemy = app->physics->CreateCircle(200, 300, 7, b2_kinematicBody);
	landEnemy->enemy->body->SetFixedRotation(true);
	landEnemy->enemy->listener = this;
	landEnemy->enemy->type = PhysBody::Type::ENEMY_L;
	//Idle anim
	landEnemy->idleAnim.PushBack({ 4, 347, 50, 50 });
	landEnemy->idleAnim.PushBack({ 52, 346, 50, 50 });
	landEnemy->idleAnim.PushBack({ 103, 345, 50, 50 });
	landEnemy->idleAnim.loop = false;
	landEnemy->idleAnim.mustFlip = true;
	landEnemy->idleAnim.speed = 0.01f;
	//Walking anim
	//---
	//Death anim
	landEnemy->deathAnim.PushBack({ 148, 347, 50, 50 });
	landEnemy->deathAnim.PushBack({ 196, 346, 50, 50 });
	landEnemy->deathAnim.PushBack({ 247, 345, 50, 50 });
	landEnemy->deathAnim.loop = false;
	landEnemy->deathAnim.mustFlip = true;
	landEnemy->deathAnim.speed = 1.0f;
	//texture and render
	landEnemyTexture = app->tex->Load("Assets/textures/Tileset.png");
	landEnemy->show = false;

    return true;
}

bool Enemy::PreUpdate()
{
    return true;
}

bool Enemy::Update(float dt)
{
    //Enemy Animation states
    switch (eflyingState)
    {
    case IDLE:
        currentFlyingAnim = &flyingEnemy->idleAnim;
        break;
	case WALKING:
		break;
    case DEATH:
		currentFlyingAnim = &flyingEnemy->deathAnim;
        break;
    }

	switch (elandState)
	{
	case IDLE:
		currentLandAnim = &landEnemy->idleAnim;
		break;
	case WALKING:
		currentLandAnim = &landEnemy->walkingAnim;
		break;
	case DEATH:
		currentLandAnim = &landEnemy->deathAnim;
		break;
	}

	//Enemy Render
    if (app->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) { 
		flyingEnemy->show = true;
		landEnemy->show = true;
	}
    if (flyingEnemy->show == true)
    {
        app->render->DrawTexture(flyingEnemyTexture, METERS_TO_PIXELS(flyingEnemy->enemy->body->GetPosition().x - 20), 
            METERS_TO_PIXELS(flyingEnemy->enemy->body->GetPosition().y - 20), &(currentFlyingAnim->GetCurrentFrame()), 1);
    }
	if (landEnemy->show == true)
	{
		app->render->DrawTexture(landEnemyTexture, METERS_TO_PIXELS(landEnemy->enemy->body->GetPosition().x - 20),
			METERS_TO_PIXELS(flyingEnemy->enemy->body->GetPosition().y - 20), &(currentLandAnim->GetCurrentFrame()), 1);
	}

    bool ret = true;

    //Enemy movement
    maxSpeedX = 1;
    minSpeedX = -1;


    return true;
}

bool Enemy::PostUpdate()
{
    bool ret = true;
    return ret;
}

// Called before quitting
bool Enemy::CleanUp()
{
    LOG("Freeing Player");

    return true;
}


void Enemy::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA->type == PhysBody::Type::ENEMY_F && bodyB->type == PhysBody::Type::PLAYER) { 
		flyingEnemy->show = false;
		landEnemy->show = false;
	}
	if (bodyA->type == PhysBody::Type::ENEMY_L && bodyB->type == PhysBody::Type::PLAYER) { 
		flyingEnemy->show = false;
		landEnemy->show = false;
	}
}