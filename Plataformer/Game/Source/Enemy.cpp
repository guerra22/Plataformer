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
	flyingEnemy->idleAnim.PushBack({ 103, 345, 40, 55 });
	flyingEnemy->idleAnim.PushBack({ 52, 346, 40, 55 });
    flyingEnemy->idleAnim.PushBack({ 4, 347, 40, 55 });
	flyingEnemy->idleAnim.PushBack({ 52, 346, 40, 55 });
	flyingEnemy->idleAnim.PushBack({ 103, 345, 40, 55 });
    flyingEnemy->idleAnim.loop = true;
    flyingEnemy->idleAnim.mustFlip = true;
    flyingEnemy->idleAnim.speed = 0.02f;
    //Death anim
    flyingEnemy->deathAnim.PushBack({ 148, 347, 40, 55 });
    flyingEnemy->deathAnim.PushBack({ 196, 346, 40, 55 });
    flyingEnemy->deathAnim.PushBack({ 247, 345, 40, 55 });
    flyingEnemy->deathAnim.loop = false;
    flyingEnemy->deathAnim.mustFlip = true;
    flyingEnemy->deathAnim.speed = 1.0f;
	//texture and render
    flyingEnemyTexture = app->tex->Load("Assets/textures/Tileset.png");
	flyingEnemy->isDead = false;


	//Initializing land enemy struct data
	landEnemy = new LandEnemy;
	elandState = IDLE;
	landEnemy->enemy = app->physics->CreateCircle(200, 300, 7, b2_dynamicBody);
	landEnemy->enemy->body->SetFixedRotation(true);
	landEnemy->enemy->listener = this;
	landEnemy->enemy->type = PhysBody::Type::ENEMY_L;
	//Idle anim
	landEnemy->idleAnim.PushBack({ 10, 242, 20, 20 });
	landEnemy->idleAnim.PushBack({ 44, 242, 20, 20 });
	landEnemy->idleAnim.PushBack({ 78, 242, 20, 20 });
	landEnemy->idleAnim.PushBack({ 112, 242, 20, 20 });
	landEnemy->idleAnim.PushBack({ 146, 242, 20, 20 });
	landEnemy->idleAnim.PushBack({ 180, 242, 20, 20 });
	landEnemy->idleAnim.PushBack({ 214, 242, 20, 20 });
	landEnemy->idleAnim.PushBack({ 248, 241, 20, 20 });
	landEnemy->idleAnim.PushBack({ 281, 240, 20, 20 });
	landEnemy->idleAnim.PushBack({ 315, 242, 20, 20 });
	landEnemy->idleAnim.PushBack({ 350, 243, 20, 20 });
	landEnemy->idleAnim.loop = false;
	landEnemy->idleAnim.mustFlip = true;
	landEnemy->idleAnim.speed = 0.01f;
	//Walking anim
	landEnemy->walkingAnim.PushBack({ 9, 269, 20, 20 });
	landEnemy->walkingAnim.PushBack({ 43, 269, 20, 20 });
	landEnemy->walkingAnim.PushBack({ 78, 270, 20, 20 });
	landEnemy->walkingAnim.PushBack({ 112, 269, 20, 20 });
	landEnemy->walkingAnim.PushBack({ 146, 269, 20, 20 });
	landEnemy->walkingAnim.PushBack({ 180, 270, 20, 20 });
	landEnemy->idleAnim.loop = true;
	landEnemy->idleAnim.mustFlip = true;
	landEnemy->idleAnim.speed = 0.1f;
	//Death anim
	landEnemy->deathAnim.PushBack({ 44, 295, 20, 20 });
	landEnemy->deathAnim.PushBack({ 11, 294, 20, 20 });
	landEnemy->deathAnim.loop = false;
	landEnemy->deathAnim.mustFlip = true;
	landEnemy->deathAnim.speed = 1.0f;
	//texture and render
	landEnemyTexture = app->tex->Load("Assets/textures/Tileset.png");
	landEnemy->isDead = false;

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
		flyingEnemy->idleAnim.Update();
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
		landEnemy->idleAnim.Update();
		break;
	case WALKING:
		currentLandAnim = &landEnemy->walkingAnim;
		landEnemy->walkingAnim.Update();
		break;
	case DEATH:
		currentLandAnim = &landEnemy->deathAnim;
		break;
	}

	//Enemy Render
    if (app->gameState == 1 && flyingEnemy->isDead == false)
    {
        app->render->DrawTexture(flyingEnemyTexture, METERS_TO_PIXELS(flyingEnemy->enemy->body->GetPosition().x - 20), 
            METERS_TO_PIXELS(flyingEnemy->enemy->body->GetPosition().y - 20), &(currentFlyingAnim->GetCurrentFrame()), 1);
    }
	if (app->gameState == 1 && landEnemy->isDead == false)
	{
		app->render->DrawTexture(landEnemyTexture, METERS_TO_PIXELS(landEnemy->enemy->body->GetPosition().x - 11),
			METERS_TO_PIXELS(landEnemy->enemy->body->GetPosition().y - 11), &(currentLandAnim->GetCurrentFrame()), 1);
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

}