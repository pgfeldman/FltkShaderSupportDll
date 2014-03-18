#pragma once
#include "CollisionCubeBase.h"
class CollisionCube :
	public CollisionCubeBase
{
public:
	CollisionCube(GLuint activeTexture, float xsize, float ysize, float zsize);
	~CollisionCube(void);

	virtual void setup();
	void render(GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack, GLShaderManager &shaderManager);
	//void environmentCalc();
	void localCleanup();
	//float testSphereAABBCollision();

protected:

	void drawPrimitive(GLBatch &batch, const GLfloat *vColor, const M3DMatrix44f &mCamera, GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack);
	//void setPosFromMatrix(float *vec){copyArray(3, &matrix[12], vec);};
	
	// Drawing

	GLuint              cubeTexture;
	GLint               reflectionShader;
	GLint               skyBoxShader;

	GLint               locMVPReflect, locMVReflect, locNormalReflect, locInvertedCamera;
	GLint				locMVPSkyBox;
	GLint				locCubeMap;
	GLint				locMaterialColor;

};

