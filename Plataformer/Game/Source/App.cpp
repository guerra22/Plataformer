#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "Map.h"
#include "ModulePhysics.h"
#include "Defs.h"
#include "Log.h"
#include "Player.h"
#include "Enemy.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{


	win = new Window();
	input = new Input();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	scene = new Scene();
	map = new Map();
	physics = new ModulePhysics();
	enemy = new Enemy();
	player = new Player();
	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(physics);
	AddModule(map);
	AddModule(scene);
	AddModule(enemy);
	AddModule(player);
	// Render last to swap buffer
	AddModule(render);


	ptimer = new PerfTimer();
	frameDuration = new PerfTimer();
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool App::Awake()
{
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	// L01: DONE 3: Load config from XML
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// L01: DONE 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());

		maxFrameRate = configApp.child("frcap").attribute("value").as_int();
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{
			// L01: DONE 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	return ret;
}

// Called before the first frame
bool App::Start()
{
	gameState = 0;
	godeMode = false;

	startupTime.Start();
	lastSecFrameTime.Start();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
// NOTE: Function has been redesigned to avoid storing additional variables on the class
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	// L08: DONE 4: Calculate the dt: differential time since last frame
	dt = frameDuration->ReadMs();
	frameDuration->Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// L02: DONE 1: This is a good place to call Load / Save methods
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();

	float secondsSinceStartup = startupTime.ReadSec();

	if (lastSecFrameTime.Read() > 1000) {
		lastSecFrameTime.Start();
		framesPerSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
		averageFps = (averageFps + framesPerSecond) / 2;
	}

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %I64u ",
		averageFps, framesPerSecond, dt, secondsSinceStartup, frameCount);

	// L08: DONE 2: Use SDL_Delay to make sure you get your capped framerate
	float delay = float(maxFrameRate) - frameDuration->ReadMs();
	//LOG("F: %f Delay:%f", frameDuration->ReadMs(), delay);

	// L08: DONE 3: Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected
	PerfTimer* delayt = new PerfTimer();
	delayt->Start();
	if (maxFrameRate > 0 && delay > 0) SDL_Delay(delay);
	LOG("Expected %f milliseconds and the real delay is % f", delay, delayt->ReadMs());

	app->win->SetTitle(title);
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;
}

// ---------------------------------------
// L02: TODO 5: Create a method to actually load an xml file
// then call all the modules to load themselves
bool App::LoadGame()
{
	bool ret = false;


		pugi::xml_document saveFile;
		pugi::xml_parse_result result = saveFile.load_file("save_game.xml");

		if (result != NULL)
		{
			pugi::xml_node save_node = saveFile.child("game_state");

			pugi::xml_node level = save_node.child("level");
			//scene = (GameScene::GameLevel)level.attribute("value").as_int();

			pugi::xml_node player = save_node.child("player");
			//playerPosition.x = player.attribute("x").as_int();
		    //playerPosition.y = player.attribute("y").as_int();
			//playerState = player.attribute("state").as_int();

			pugi::xml_node camera = save_node.child("camera");
			//cameraPosition.x = camera.attribute("x").as_int();
			//cameraPosition.y = camera.attribute("y").as_int();

			//fromGameSaved = true;

			app->enemy->LoadState(save_node.child("entities"));

			//app->fade->FadeToBlack(this, this);
		}
	

	loadGameRequested = false;

	return ret;
}

// L02: TODO 7: Implement the xml save method for current state
bool App::SaveGame() const
{
	bool ret = true;

	
		pugi::xml_document saveFile;
		pugi::xml_parse_result result = saveFile.load_file("save_game.xml");

		pugi::xml_node save_node;
		pugi::xml_node level;
		pugi::xml_node player;
		pugi::xml_node camera;
		pugi::xml_node entities;

		if (result == NULL)
		{
			pugi::xml_node declaration = saveFile.append_child(pugi::node_declaration);
			declaration.append_attribute("version") = "1.0";

			save_node = saveFile.append_child("game_state");

			level = save_node.append_child("level");
			level.append_attribute("value");

			player = save_node.append_child("player");
			player.append_attribute("x");
			player.append_attribute("y");
			player.append_attribute("state");

			camera = save_node.append_child("camera");
			camera.append_attribute("x");
			camera.append_attribute("y");

			entities = save_node.append_child("entities");
		}
		else {
			save_node = saveFile.child("game_state");
			level = save_node.child("level");
			player = save_node.child("player");
			camera = save_node.child("camera");
			entities = save_node.child("entities");
		}

		level.attribute("value") = scene;

		app->enemy->SaveEnemy(entities);
		app->player->SavePlayer(player);

		camera.attribute("x") = app->render->camera.x;
		camera.attribute("y") = app->render->camera.y;

		saveFile.save_file("save_game.xml");
	

	saveGameRequested = false;

	return ret;
}



