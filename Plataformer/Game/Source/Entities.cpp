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
	return true;
}

bool Entities::PreUpdate()
{
	return true;
}

bool Entities::Update(float dt)
{

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