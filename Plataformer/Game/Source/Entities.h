#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Module.h"
#include "ModulePhysics.h"
#include "Box2D/Box2D/Box2D.h"
#include "Module.h"
#include "Globals.h"
#include "Point.h"
#include "Animation.h"
#include "Textures.h"
#include "Render.h"

struct Heart {
	PhysBody* heart;
	bool isAwake;
};

struct CheckPoint {
	PhysBody* flag;
};

struct Coin {
	PhysBody* coin;
	bool isCollected;
};

class Entities : public Module
{
public:
	Entities();

	// Destructor
	virtual ~Entities();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Save function
	void SaveEntities(pugi::xml_node& save);

	int damageScreenCooldown;

	int coinsCollected;

	Heart* heart1;
	Heart* heart2;
	Coin* coin1;
	Coin* coin2;
	Coin* coin3;
	CheckPoint* checkpoint;
	SDL_Texture* heartTexture;
	SDL_Texture* DoorClosed;
	SDL_Texture* DoorOpenned;
	SDL_Texture* CheckpointTexture;
	SDL_Texture* coinTexture;
	SDL_Texture* DamageScreen;

	SDL_Texture* heartGui;
	SDL_Texture* coinGui;
	SDL_Texture* emptyHeartGui;
	SDL_Texture* emptyCoinGui;



};

#endif