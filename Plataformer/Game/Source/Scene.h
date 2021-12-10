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

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
    
	struct Platform {
		PhysBody* platform;
	};

	enum GameScreen
	{
		INTRO,
		GAME,
		DEFEAT,
		VICTORY
	};
	
	GameScreen gameScreen;


	p2List<PhysBody*> walls;

	PhysBody* deathFloor;
	PhysBody* winBlock;

	SDL_Texture* StartScreen;
	SDL_Texture* GameOverScreen;
	SDL_Texture* WinScreen;

	p2List<PhysBody*> box;

private:
	SDL_Texture* img;

	b2World* world;
	b2Body* ground;

	

};

#endif // __SCENE_H__