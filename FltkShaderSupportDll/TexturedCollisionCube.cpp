#include "StdAfx.h"
#include "TexturedCollisionCube.h"


TexturedCollisionCube::TexturedCollisionCube(GLuint activeTexture, float xsize, float ysize, float zsize, char* texFileName): CollisionCubeBase(activeTexture, xsize, ysize, zsize)
{
	setup(texFileName);
	materialType = NONE_SELECTED;
}


TexturedCollisionCube::~TexturedCollisionCube(void)
{
}

void TexturedCollisionCube::setup(char* texFileName){
	// Make the texture object
	glActiveTexture(activeTextureID);
	glGenTextures(1, &textureId);
	
	
	// Load the Marble
	glBindTexture(GL_TEXTURE_2D, textureId);
	bool success = LoadTGATexture(texFileName, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
	if(!success)
		fl_alert("Unable to load '%s' Make sure RLE is off!", texFileName);

	gltMakeCube(cubeBatch, 0.5f);
	timeScalar = .5f;
	angle = 0.0f;
}

void TexturedCollisionCube::calcCorners(){
	modelMat.LoadIdentity();
	modelMat.Translate(position[0], position[1], position[2]);
			
	modelMat.Rotate(orientation[1], 0.0f, 1.0f, 0.0f);
	modelMat.Rotate(orientation[2], 1.0f, 0.0f, 0.0f);

	modelMat.Translate(minAARB[0], minAARB[1], minAARB[2]);
	modelMat.GetMatrix(matrix);
	setPosFromMatrix(cornerPos[0]);

	modelMat.LoadIdentity();
	modelMat.Translate(position[0], position[1], position[2]);
			
	modelMat.Rotate(orientation[1], 0.0f, 1.0f, 0.0f);
	modelMat.Rotate(orientation[2], 1.0f, 0.0f, 0.0f);

	modelMat.Translate(maxAARB[0], maxAARB[1], maxAARB[2]);
	modelMat.GetMatrix(matrix);
	setPosFromMatrix(cornerPos[1]);
}

void TexturedCollisionCube::environmentCalc(){
	// since glut cube draws centered our position is minus size/2
	minAARB[0] = -size[0]*0.5f;
	minAARB[1] = -size[1]*0.5f;
	minAARB[2] = -size[2]*0.5f;

	maxAARB[0] = size[0]*0.5f;
	maxAARB[1] = size[1]*0.5f;
	maxAARB[2] = size[2]*0.5f;

	calcCorners();

	calcDeltaTime();
	orientation[1] += deltaTime * scalar;
	orientation[2] += deltaTime * scalar;
	//Dprint::add("deltatTime = %.2f, scalar = %.2f, cube angle = %.2f", deltaTime, scalar, orientation[1]);
}

void TexturedCollisionCube::render(GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack, GLShaderManager &shaderManager){
	static GLfloat vLightPos [] = { 100.0f, 100.0f, 50.0f };

	//Dprint::add("TexturedCollisionCube position = (%.2f, %.2f, %.2f)", position[0], position[1], position[2]);
	//Dprint::add("TexturedCollisionCube orientation = (%.2f, %.2f, %.2f)", orientation[0], orientation[1], orientation[2]);

	modelViewStack.PushMatrix();
		//modelViewStack.Translate(position[0] + size[0]*0.5f, position[1] + size[1]*0.5f, position[2] + size[2]*0.5f);
		modelViewStack.Translate(position[0], position[1], position[2]);
		modelViewStack.Rotate(orientation[1], 0.0f, 1.0f, 0.0f);
		modelViewStack.Rotate(orientation[2], 1.0f, 0.0f, 0.0f);
		modelViewStack.Scale(size[0], size[1], size[2]);

		glBindTexture(GL_TEXTURE_2D, textureId);
		shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewStack.GetMatrix(), projectionStack.GetMatrix(), vLightPos, vWhite, 0);
		cubeBatch.Draw();
	modelViewStack.PopMatrix();

	/***** Debug drawing 
	for(int i = 0; i < 2; ++i){
		modelViewStack.PushMatrix();
			modelViewStack.Translate(cornerPos[i][0], cornerPos[i][1], cornerPos[i][2]);

			glBindTexture(GL_TEXTURE_2D, textureId);
			shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewStack.GetMatrix(), projectionStack.GetMatrix(), vLightPos, vWhite, 0);
			cubeBatch.Draw();
		modelViewStack.PopMatrix();
	}

	modelViewStack.PushMatrix();
		modelViewStack.Scale(size[0], size[1], size[2]);

		glBindTexture(GL_TEXTURE_2D, textureId);
		shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewStack.GetMatrix(), projectionStack.GetMatrix(), vLightPos, vWhite, 0);
		cubeBatch.Draw();
	modelViewStack.PopMatrix();


	for(int i = 0; i < 2; ++i){
		setCollisionSphere(cornerPos[i][0], cornerPos[i][1], cornerPos[i][2], 1.0f);
		testSphereAABBCollision();
		Dprint::add("CollisionPoint[%d] = (%.2f, %.2f, %.2f)", i, xformed[0], xformed[1], xformed[2]);
		modelViewStack.PushMatrix();
		modelViewStack.Translate(xformed[0], xformed[1], xformed[2]);

			glBindTexture(GL_TEXTURE_2D, textureId);
			shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewStack.GetMatrix(), projectionStack.GetMatrix(), vLightPos, vWhite, 0);
			cubeBatch.Draw();
		modelViewStack.PopMatrix();
	}
	/****/
}

void TexturedCollisionCube::localCleanup(){
	glDeleteTextures(1, &textureId);
}
