#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "ModulePhysics.h"
#include "Box2D/Box2D/Box2D.h"
#include "Module.h"
#include "Globals.h"
#include "Point.h"
#include "Animation.h"
#include "Textures.h"
#include "Render.h"


struct Player1 {
	PhysBody* player;
	Animation idlePlayerAnim;
	Animation jumpingPlayerAnim;
	Animation walkingPlayerAnim;
	Animation deathPlayerAnim;
	bool IsDirectionRight;
};

class Player : public Module
{
public:
	Player();

	// Destructor
	virtual ~Player();

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
	SDL_Texture* playerTexture;

public:
	Player1* p;
	Animation* currentAnim = nullptr;
	Animation idlePlayerAnim, walkingPlayerAnim, jumpingPlayerAnim, deathPlayerAnim;
	enum State
	{
		IDLE,
		WALK,
		JUMP,
		DEATH
	};
	State pState;
	bool IsDirectionRight;
	float maxSpeedX;
	float minSpeedX;
	SDL_RendererFlip direction;

	bool isJumping;
	bool show;

};

#endif