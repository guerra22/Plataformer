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
	Animation idleHeart;
	bool isAwake;
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

	Heart* heart1;
	Heart* heart2;
	Animation* idleHeart;
	Animation* currentHeartAnim = nullptr;
	SDL_Texture* heartTexture;



};

#endif