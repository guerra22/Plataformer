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

	
	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[4];
	int points[8] = {
			0,0,
			SCREEN_WIDTH,0,
			SCREEN_WIDTH,SCREEN_HEIGHT,
			0,SCREEN_HEIGHT
	};
	for (uint i = 0; i < 4; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, 4);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;
	
	


	b7 = app->physics->CreateRectangle(20, 20, 20, 20, b2_staticBody);
	b2PolygonShape box7;
	box7.SetAsBox(PIXEL_TO_METERS(10) * 0.5f, PIXEL_TO_METERS(10) * 0.5f);

	b2FixtureDef fixture7;
	fixture7.shape = &box7;
	fixture7.density = 1.0f;



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

	//if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) app->render->camera.y += 1;

	//if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) app->render->camera.y -= 1;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

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
