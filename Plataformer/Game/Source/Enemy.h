#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Module.h"
#include "ModulePhysics.h"
#include "Box2D/Box2D/Box2D.h"
#include "Module.h"
#include "Globals.h"
#include "Point.h"
#include "Animation.h"
#include "Textures.h"
#include "Render.h"


struct FlyingEnemy {
	PhysBody* enemy;
	Animation idleAnim;
	Animation deathAnim;
	bool isDead;
};

struct LandEnemy {
	PhysBody* enemy;
	Animation idleAnim;
	Animation walkingAnim;
	Animation deathAnim;
	bool isDead;
};

class Enemy : public Module
{
public:
	Enemy();

	// Destructor
	virtual ~Enemy();

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

	void SaveEnemy(pugi::xml_node& save);

public:
	SDL_Texture* flyingEnemyTexture;
	SDL_Texture* landEnemyTexture;

public:
	FlyingEnemy* flyingEnemy;
	LandEnemy* landEnemy;
	Animation* currentFlyingAnim = nullptr;
	Animation* currentLandAnim = nullptr;
	enum State
	{
		IDLE,
		WALKING,
		DEATH
	};
	State eflyingState;
	State elandState;
};

#endif
