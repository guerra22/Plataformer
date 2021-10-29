#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "ModulePhysics.h"
#include "Box2D/Box2D/Box2D.h"

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

	struct PlayerObj {
		PhysBody* playerBody;
		bool isDead;
		bool IsJumping;
	};

	PlayerObj* player;


};

#endif