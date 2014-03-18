#pragma once
#include "DrawableObject.h"

class CollisionCubeBase	: 
	public DrawableObject
{
public:
	CollisionCubeBase(GLuint activeTexture, float xsize, float ysize, float zsize);
	virtual ~CollisionCubeBase(void);

	virtual void render(GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack, GLShaderManager &shaderManager) = 0;
	virtual void environmentCalc();
	virtual void localCleanup()=0;
	float testSphereAABBCollision();

protected:
	float size[3];

	// Drawing
	float				timeScalar;
	float				angle;
	GLBatch             cubeBatch;

	// collision detection
	GLMatrixStack		modelStack;
	GLMatrixStack		collisionStack;
};

