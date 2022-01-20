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
    flyingEnemy->enemy = app->physics->CreateCircle(30, 230, 7, b2_kinematicBody);
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
	landEnemy->enemy = app->physics->CreateCircle(730, 200, 7, b2_dynamicBody);
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
	landEnemy->direction = 0;

	

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

	// Restart enemies position
	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) { 
		flyingEnemy->enemy->body->SetTransform({ PIXEL_TO_METERS(30), PIXEL_TO_METERS(230) }, 0.0f); 
		landEnemy->enemy->body->SetTransform({ PIXEL_TO_METERS(730), PIXEL_TO_METERS(217) }, 0.0f);
	}

    bool ret = true;

    //Enemy movement
	if (flyingEnemy->enemy->body->GetPosition().y != app->player->p->player->body->GetPosition().y)	{
		if (flyingEnemy->enemy->body->GetPosition().y > app->player->p->player->body->GetPosition().y){
			flyingEnemy->enemy->body->SetLinearVelocity({ flyingEnemy->enemy->body->GetLinearVelocity().x , -0.5f });
		}
		if (flyingEnemy->enemy->body->GetPosition().y < app->player->p->player->body->GetPosition().y) {
			flyingEnemy->enemy->body->SetLinearVelocity({ flyingEnemy->enemy->body->GetLinearVelocity().x , 0.5f });
		}
	}
	if (flyingEnemy->enemy->body->GetPosition().x != app->player->p->player->body->GetPosition().x) {
		if (flyingEnemy->enemy->body->GetPosition().x > app->player->p->player->body->GetPosition().x) {
			flyingEnemy->enemy->body->SetLinearVelocity({ -0.6f , flyingEnemy->enemy->body->GetLinearVelocity().y });
		}
		if (flyingEnemy->enemy->body->GetPosition().x < app->player->p->player->body->GetPosition().x) {
			flyingEnemy->enemy->body->SetLinearVelocity({ 0.6 , flyingEnemy->enemy->body->GetLinearVelocity().y });
		}
	}


	//Load State
	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT) {
	    flyingEnemy->enemy->body->SetTransform({ PIXEL_TO_METERS(app->FlyEnemyX), PIXEL_TO_METERS(app->FlyEnemyY) }, 0.0f);
		landEnemy->enemy->body->SetTransform({ PIXEL_TO_METERS(app->LandEnemyX), PIXEL_TO_METERS(app->LandEnemyY) }, 0.0f);
	}

	
	if (landEnemy->direction == 0){
		landEnemy->enemy->body->SetLinearVelocity({ -0.2f, landEnemy->enemy->body->GetLinearVelocity().y });
		if (landEnemy->enemy->body->GetPosition().x < PIXEL_TO_METERS(690)) { landEnemy->direction = 1; }
	}
	else {
		landEnemy->enemy->body->SetLinearVelocity({ 0.2f, landEnemy->enemy->body->GetLinearVelocity().y });
		if (landEnemy->enemy->body->GetPosition().x > PIXEL_TO_METERS(760)) { landEnemy->direction = 0; }
	}

	
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

void Enemy::SaveEnemy(pugi::xml_node& save)
{
	save.attribute("xf") = METERS_TO_PIXELS(flyingEnemy->enemy->body->GetPosition().x);
	save.attribute("yf") = METERS_TO_PIXELS(flyingEnemy->enemy->body->GetPosition().y);

	save.attribute("xl") = METERS_TO_PIXELS(landEnemy->enemy->body->GetPosition().x);
	save.attribute("yl") = METERS_TO_PIXELS(landEnemy->enemy->body->GetPosition().y);


}