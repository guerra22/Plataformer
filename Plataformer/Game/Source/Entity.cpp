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
#include "Entity.h"

Entity::Entity() : Module()
{
	name.Create("Entity");
}

// Destructor
Entity::~Entity()
{}

bool Entity::Awake()
{
	return true;
}

bool Entity::Start()
{
	return true;
}

bool Entity::PreUpdate()
{
	return true;
}

bool Entity::Update(float dt)
{
	
	return true;
}

bool Entity::PostUpdate()
{

	return true;
}

// Called before quitting
bool Enemy::CleanUp()
{

	return true;
}

