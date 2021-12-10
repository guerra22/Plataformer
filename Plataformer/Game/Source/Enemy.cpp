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
    //Initializing player struct data
    flyingEnemy = new FlyingEnemy;
    eState = IDLE;

    flyingEnemy->enemy = app->physics->CreateCircle(20, 300, 7, b2_kinematicBody);
    flyingEnemy->enemy->body->SetFixedRotation(true);
    flyingEnemy->enemy->type = PhysBody::Type::ENEMY;
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

    enemyTexture = app->tex->Load("Assets/textures/Tileset.png");

    show = false;

    return true;
}

bool Enemy::PreUpdate()
{
    return true;
}

bool Enemy::Update(float dt)
{
    //---------------------------------------
    switch (eState)
    {
    case IDLE:
        currentAnim = &flyingEnemy->idleAnim;
        break;
    case DEATH:
        currentAnim = &flyingEnemy->deathAnim;
        break;
    }

    if (app->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) { show = true; }

    if (show == true)
    {
        app->render->DrawTexture(enemyTexture, METERS_TO_PIXELS(flyingEnemy->enemy->body->GetPosition().x - 23), 
            METERS_TO_PIXELS(flyingEnemy->enemy->body->GetPosition().y - 20), &(currentAnim->GetCurrentFrame()), 1);
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