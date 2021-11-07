#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "ModulePhysics.h"
#include "Box2D/Box2D/Box2D.h"
#include "Module.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

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
    
	struct Platform {
		PhysBody* platform;
	};


	p2List<PhysBody*> walls;

	PhysBody* deathFloor;



	p2List<PhysBody*> box;


private:
	SDL_Texture* img;

	b2World* world;
	b2Body* ground;
	b2Vec2 p;

	

};

#endif // __SCENE_H__