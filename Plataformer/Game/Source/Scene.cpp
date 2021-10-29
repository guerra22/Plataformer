#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"
#include "ModulePhysics.h"
#include "External/Box2D/Box2D/Box2D.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// L03: DONE: Load map
	//app->map->Load("iso_walk.tmx");
	app->map->Load("map.tmx");

	
	// Load music
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

	LOG("Creating Physics 2D environment");
	
	int points[8] = {
			0,0,
			1598,0,
			1598,480,
			0,480
	}; 
	

	PhysBody* pb_wall1 = app->physics->CreateChain(0, 0, points, 8, b2_staticBody);
	walls.add(pb_wall1);


	// paltforms creation

	Platform* plat1 = new Platform;
	Platform* plat2 = new Platform;
	Platform* plat3 = new Platform;
	Platform* plat4 = new Platform;
	Platform* plat5 = new Platform;
	Platform* plat6 = new Platform;
	Platform* plat7 = new Platform;
	Platform* plat8 = new Platform;
	Platform* plat9 = new Platform;
	Platform* plat10 = new Platform;
	Platform* plat11 = new Platform;
	Platform* plat12 = new Platform;
	Platform* plat13 = new Platform;
	Platform* plat14 = new Platform;
	Platform* plat15 = new Platform;
	Platform* plat16 = new Platform;

	plat1->platform = app->physics->CreateRectangle(92,368, 135, 30, b2_staticBody);
	plat2->platform = app->physics->CreateRectangle(272, 337, 95, 30, b2_staticBody);
	plat3->platform = app->physics->CreateRectangle(368, 304, 30, 30, b2_staticBody);
	plat4->platform = app->physics->CreateRectangle(433, 337, 30, 30, b2_staticBody);
	plat5->platform = app->physics->CreateRectangle(495, 304, 30, 30, b2_staticBody);
	plat6->platform = app->physics->CreateRectangle(559, 337, 30, 30, b2_staticBody);
	plat7->platform = app->physics->CreateRectangle(624, 304, 30, 30, b2_staticBody);
    plat8->platform = app->physics->CreateRectangle(720, 240, 95, 30, b2_staticBody);
	plat9->platform = app->physics->CreateRectangle(848, 240, 30, 95, b2_staticBody);

	plat10->platform = app->physics->CreateRectangle(912, 175, 30, 30, b2_staticBody);
	plat11->platform = app->physics->CreateRectangle(976, 143, 30, 30, b2_staticBody);
	plat12->platform = app->physics->CreateRectangle(1040, 112, 30, 30, b2_staticBody);
    plat13->platform = app->physics->CreateRectangle(1104, 80, 30, 30, b2_staticBody);

	plat14->platform = app->physics->CreateRectangle(1295, 400, 30, 30, b2_staticBody);
	plat15->platform = app->physics->CreateRectangle(1376, 368, 60, 30, b2_staticBody);
	plat16->platform = app->physics->CreateRectangle(1509, 336, 135, 30, b2_staticBody);
	
	//death floor created
	deathFloor = new PhysBody;
	deathFloor = app->physics->CreateRectangle(250, 470, 2800, 30, b2_kinematicBody);
	

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	
    // L02: DONE 3: Request Load / Save when pressing L/S
	if(app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->render->camera.x < 0)
		app->render->camera.x += 1;

	if(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->render->camera.x > -320)
		app->render->camera.x -= 2;

	// Draw map
	app->map->Draw();

	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
				   app->map->mapData.width, app->map->mapData.height,
				   app->map->mapData.tileWidth, app->map->mapData.tileHeight,
				   app->map->mapData.tilesets.count());

	app->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
