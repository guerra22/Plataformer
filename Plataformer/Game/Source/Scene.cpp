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

	//loading Screens
	StartScreen = app->tex->Load("Assets/textures/startScreen.png");
	GameOverScreen = app->tex->Load("Assets/textures/GameOverScreen.png");
	WinScreen = app->tex->Load("Assets/textures/WinScreen.png");
	
	// Load music
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

	LOG("Creating Physics 2D environment");
	int points[8] = {
			0,0,
			1280,0,
			1280,480,
			0,480
	}; 

	PhysBody* pb_wall1 = app->physics->CreateChain(0, 0, points, 8, b2_staticBody);
	walls.add(pb_wall1);
	pb_wall1->type = PhysBody::Type::PLATFORM;

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

	plat1->platform = app->physics->CreateRectangle(85,368, 150, 30, b2_staticBody);
	plat1->platform->type = PhysBody::Type::PLATFORM;
	plat2->platform = app->physics->CreateRectangle(272, 337, 95, 30, b2_staticBody);
	plat2->platform->type = PhysBody::Type::PLATFORM;
	plat3->platform = app->physics->CreateRectangle(368, 304, 30, 30, b2_staticBody);
	plat3->platform->type = PhysBody::Type::PLATFORM;
	plat4->platform = app->physics->CreateRectangle(433, 337, 30, 30, b2_staticBody);
	plat4->platform->type = PhysBody::Type::PLATFORM;
	plat5->platform = app->physics->CreateRectangle(495, 304, 30, 30, b2_staticBody);
	plat5->platform->type = PhysBody::Type::PLATFORM;
	plat6->platform = app->physics->CreateRectangle(559, 337, 30, 30, b2_staticBody);
	plat6->platform->type = PhysBody::Type::PLATFORM;
	plat7->platform = app->physics->CreateRectangle(624, 304, 30, 30, b2_staticBody);
	plat7->platform->type = PhysBody::Type::PLATFORM;
    plat8->platform = app->physics->CreateRectangle(720, 240, 95, 30, b2_staticBody);
	plat8->platform->type = PhysBody::Type::PLATFORM;
	plat9->platform = app->physics->CreateRectangle(848, 240, 30, 95, b2_staticBody);
	plat9->platform->type = PhysBody::Type::PLATFORM;

	plat10->platform = app->physics->CreateRectangle(912, 175, 30, 30, b2_staticBody);
	plat10->platform->type = PhysBody::Type::PLATFORM;
	plat11->platform = app->physics->CreateRectangle(1009, 272, 95, 30, b2_staticBody);
	plat11->platform->type = PhysBody::Type::PLATFORM;
	plat12->platform = app->physics->CreateRectangle(1188, 337, 135, 30, b2_staticBody);
	plat12->platform->type = PhysBody::Type::PLATFORM;
	
	//death floor created
	deathFloor = new PhysBody;
	deathFloor = app->physics->CreateRectangle(250, 470, 2800, 30, b2_staticBody);
	deathFloor->listener = this;
	deathFloor->type = PhysBody::Type::FLOOR;

	//win block created
	winBlock = new PhysBody;
	winBlock = app->physics->CreateRectangle(1230, 310, 30, 30, b2_staticBody);
	winBlock->listener = this;
	winBlock->type = PhysBody::Type::WIN;

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
	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width, app->map->mapData.height,
		app->map->mapData.tileWidth, app->map->mapData.tileHeight,
		app->map->mapData.tilesets.count());
	
	switch (gameScreen)
	{
	case Scene::INTRO:

		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
			gameScreen = GameScreen::GAME;
			app->gameState = 1;
		}
		app->render->DrawTexture(StartScreen, 0, 0, NULL, 0.0f, 0);
		break;

	case Scene::GAME:
		//Toggle godmode
		if (app->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN) { app->godeMode = !app->godeMode; }

		//game state change
		if (app->gameState == 2) { gameScreen = GameScreen::DEFEAT; }
		if (app->gameState == 3) { gameScreen = GameScreen::VICTORY; }

		// L02: DONE 3: Request Load / Save when pressing L/S
		if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
			app->LoadGameRequest();
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
			app->SaveGameRequest();

		// Draw map
		app->map->Draw();

		app->win->SetTitle(title.GetString());
		break;
	case Scene::DEFEAT:
		app->render->DrawTexture(GameOverScreen, 0, 0, NULL, 0.0f, 0);
		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
			app->gameState = 1;
			gameScreen = GameScreen::GAME; 
		}
		break;
	case Scene::VICTORY:
		app->render->DrawTexture(WinScreen, 0, 0, NULL, 0.0f, 0);
		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) { 
			app->gameState = 1;
			gameScreen = GameScreen::GAME; 
		}
		break;
	default:
		break;
	}  

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

void Scene::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA->type == PhysBody::Type::FLOOR && bodyB->type == PhysBody::Type::PLAYER && app->godeMode == false && app->gameState == 1) {
		app->gameState = 2;
		gameScreen = GameScreen::DEFEAT;
	}
	if (bodyA->type == PhysBody::Type::WIN && bodyB->type == PhysBody::Type::PLAYER && app->gameState == 1) {
		app->gameState = 3;
		gameScreen = GameScreen::VICTORY;
	}
}