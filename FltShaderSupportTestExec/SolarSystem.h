#pragma once
#include "drawableobject.h"
class SolarSystem :
	public DrawableObject
{
public:
	SolarSystem(GLuint activeTexture);
	~SolarSystem(void);
	void setup();
	void render(GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack, GLShaderManager &shaderManager);
	void environmentCalc();
	void drawPlanet(float angle, float dist, float size, GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack, GLShaderManager &shaderManager);
	void localCleanup();

private:
	GLTriangleBatch     sphereBatch;
	GLBatch				floorBatch;
	M3DMatrix44f		cameraMatrix;
	GLuint				uiTextures[3];

	GLuint	flatShader;
	GLint	flatLocMVP;				// The location of the ModelViewProjection matrix uniform
	GLint	flatLocColorValue;		// The location of the color
	GLint	flatLocDissolveFactor;	// The location of the dissolve effect value
	GLint   flatLocTexture;			// The location of the texture

	GLuint  dissolveShader;		// dissolve shader
	GLint	dissolveLocDissolveFactor;	// The location of the dissolve effect value

	GLuint	testShader;			// The diffuse light shader
	GLint	testLocAmbient;			// The location of the ambient color
	GLint   testLocDiffuse;			// The location of the diffuse color
	GLint   testLocSpecular;		// The location of the specular color
	GLint	testLocLight;			// The location of the Light in eye coordinates
	GLint	testLocMVP;				// The location of the ModelViewProjection matrix uniform
	GLint	testLocMV;				// The location of the ModelView matrix uniform
	GLint	testLocNM;				// The location of the Normal matrix uniform
	GLint   testLocTexture;			// The location of the texture
	

	
	float mercuryAngle;
	float venusAngle;
	float earthAngle;
	float marsAngle;
	float jupiterAngle;
	float saturnAngle;
	float uranusAngle;
	float neptuneAngle;

	float mercuryStep;
	float venusStep;
	float earthStep;
	float marsStep;
	float jupiterStep;
	float saturnStep;
	float uranusStep;
	float neptuneStep;
	
	float mercuryDist;
	float venusDist;
	float earthDist;
	float marsDist;
	float jupiterDist;
	float saturnDist;
	float uranusDist;
	float neptuneDist;

	float timeScalar;
};

