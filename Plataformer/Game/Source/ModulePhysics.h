#pragma once
#include "Module.h"
#include "p2List.h"
#include "External/Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -6.5f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define RAD_PER_DEG ((float)180/b2_pi)
#define DEG_PER_RAD ((float)b2_pi/180)
#define RAD_TO_DEG(r) ((float) RAD_PER_DEG * r)
#define DEG_TO_RAD(r) ((float) DEG_PER_RAD * r)

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}
	~PhysBody()
	{
		body->GetWorld()->DestroyBody(body);
	}

	void GetPosition(int& x, int& y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
	bool pendingToDelete = false;

	enum class Type {
		FLOOR,
		PLAYER,
		PLATFORM
	};

	Type type;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics();
	~ModulePhysics();

	bool Start();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType type = b2_dynamicBody);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType type = b2_dynamicBody);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, b2BodyType type = b2_dynamicBody);
	PhysBody* CreateChain(int x, int y, int* points, int size, b2BodyType type = b2_dynamicBody);

	b2RevoluteJoint* CreateRevoluteJoint(PhysBody* A, b2Vec2 anchorA, PhysBody* B, b2Vec2 anchorB, float angle = 0.0f, bool collideConnected = false, bool enableLimit = true);
	b2PrismaticJoint* CreatePrismaticJoint(PhysBody* A, b2Vec2 anchorA, PhysBody* B, b2Vec2 anchorB, b2Vec2 axys, float maxHeight = 0.0f, bool collideConnected = true, bool enableLimit = true);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact) override;

	bool debug;
	b2MouseJoint* mouse_joint;
	b2World* world;

private:

	
	b2Body* ground;

	b2Body* mouseBody;
	b2Vec2 p;

	p2List<b2RevoluteJoint*> revoluteList;
};