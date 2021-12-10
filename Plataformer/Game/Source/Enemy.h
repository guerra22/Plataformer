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

public:
	SDL_Texture* enemyTexture;

public:
	FlyingEnemy* flyingEnemy;
	Animation* currentAnim = nullptr;
	Animation idleAnim, deathAnim;
	enum State
	{
		IDLE,
		DEATH
	};
	State eState;

	float maxSpeedX;
	float minSpeedX;

	bool show;

};

#endif
