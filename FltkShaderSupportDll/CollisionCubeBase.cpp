#include "StdAfx.h"
#include "CollisionCubeBase.h"


CollisionCubeBase::CollisionCubeBase(GLuint activeTexture, float xsize, float ysize, float zsize): DrawableObject(activeTexture)
{
	size[0] = xsize;
	size[1] = ysize;
	size[2] = zsize;

	position[0] = 0.0f;
	position[1] = 0.0f;
	position[2] = 0.0f;

	orientation[0] = 0.0f;
	orientation[1] = 0.0f;
	orientation[2] = 0.0f;

	// since glut cube draws centered our position is minus size/2
	boundingSphereRadius = sqrt(SQR(size[0]*0.5f)+SQR(size[1]*0.5f)+SQR(size[2]*0.5f) );

	minAARB[0] = position[0] - size[0]*0.5f;
	minAARB[1] = position[1] - size[1]*0.5f;
	minAARB[2] = position[2] - size[2]*0.5f;

	maxAARB[0] = position[0] + size[0]*0.5f;
	maxAARB[1] = position[1] + size[1]*0.5f;
	maxAARB[2] = position[2] + size[2]*0.5f;
}


CollisionCubeBase::~CollisionCubeBase(void)
{
}

void CollisionCubeBase::environmentCalc(){
	// since glut cube draws centered our position is minus size/2
	minAARB[0] = -size[0]*0.5f;
	minAARB[1] = -size[1]*0.5f;
	minAARB[2] = -size[2]*0.5f;

	maxAARB[0] = size[0]*0.5f;
	maxAARB[1] = size[1]*0.5f;
	maxAARB[2] = size[2]*0.5f;

	calcDeltaTime();
	orientation[1] += deltaTime * scalar;
	orientation[2] += deltaTime * scalar;
	Dprint::add("deltatTime = %.2f, scalar = %.2f, cube angle = %.2f", orientation[1]);
}

float CollisionCubeBase::testSphereAABBCollision(){
	modelStack.LoadIdentity();
	// build the inverse of the matrix that we used to draw the cube. Don't invert scale, because this has to handle a sphere test
	// we can do this because the scale is really only used to make the glutCube the size we want to draw.
	modelStack.Rotate(-orientation[2], 1.0f, 0.0f, 0.0f);
	modelStack.Rotate(-orientation[1], 0.0f, 1.0f, 0.0f);
	modelStack.Translate(-position[0], -position[1], -position[2]);


	// now translate the test point into this space
	collisionStack.LoadIdentity();
	collisionStack.Translate(collisionPoint[0], collisionPoint[1], collisionPoint[2]);		

	modelStack.MultMatrix(collisionStack.GetMatrix());
	modelStack.GetMatrix(matrix);
	setPosFromMatrix(xformed);
	
	float hit = aabbSphereIntersect(minAARB, maxAARB, xformed, collisionRadius);

	//Dprint::add("cube min: (%.2f, %.2f, %.2f), max: (%.2f, %.2f, %.2f)", minAARB[0], minAARB[1], minAARB[2], maxAARB[0], maxAARB[1], maxAARB[2]);
	//Dprint::add("sphere: (%.2f, %.2f, %.2f), xformed: (%.2f, %.2f, %.2f)", collisionPoint[0], collisionPoint[1], collisionPoint[2], xformed[0], xformed[1], xformed[2]);
	//Dprint::add("hit = %.2f", hit);
	return hit;
}