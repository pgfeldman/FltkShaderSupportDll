#include "StdAfx.h"
#include "SolarSystem.h"

//M3DMatrix44f		cameraMatrix;

SolarSystem::SolarSystem(GLuint activeTexture) : DrawableObject(activeTexture)
{
	setup();
}


SolarSystem::~SolarSystem(void)
{
}

void SolarSystem::setup()
{
	gltMakeSphere(sphereBatch, 1, 18, 18);

	// Make the solid ground
	GLfloat texSize = 10.0f;
	floorBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
	floorBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	floorBatch.Vertex3f(-60.0f, 0.0f, 60.0f);
	
	floorBatch.MultiTexCoord2f(0, texSize, 0.0f);
    floorBatch.Vertex3f(60.0f, 0.0f, 60.0f);
	
	floorBatch.MultiTexCoord2f(0, texSize, texSize);
	floorBatch.Vertex3f(60.0f, 0.0f, -60.0f);
	
	floorBatch.MultiTexCoord2f(0, 0.0f, texSize);
	floorBatch.Vertex3f(-60.0f, 0.0f, -60.0f);
	floorBatch.End();

	// Make 3 texture objects
	glActiveTexture(activeTextureID);
	glGenTextures(3, uiTextures);
	
	
	// Load the Marble
	glBindTexture(GL_TEXTURE_2D, uiTextures[0]);
	bool success = LoadTGATexture("c:/textures/Marble.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
	if(!success)
		fl_alert("Unable to load 'c:/textures/Marble.tga'");

	// Load Mars
	glBindTexture(GL_TEXTURE_2D, uiTextures[1]);
	success = LoadTGATexture("c:/textures/SunTexture_128x128.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	if(!success)
		fl_alert("Unable to load ''c:/textures/SunTexture_128x128.tga'");
	
	// Load Moon
	glBindTexture(GL_TEXTURE_2D, uiTextures[2]);
	success = LoadTGATexture("c:/textures/Moonlike.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	if(!success)
		fl_alert("Unable to load 'c:/textures/Moonlike.tga'");
	/****/

	// load the shaders
	testShader = gltLoadShaderPairWithAttributes("/shaders/ADSTexture.vp", "/shaders/ADSTexture.fp", 3, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_NORMAL, "vNormal", GLT_ATTRIBUTE_TEXTURE0, "vTexture0");
	
	if(testShader == NULL)
		fl_alert("Unable to load '/shaders/DiffuseLight.*'");

	testLocAmbient = glGetUniformLocation(testShader, "ambientColor");
	testLocDiffuse = glGetUniformLocation(testShader, "diffuseColor");
	testLocSpecular = glGetUniformLocation(testShader, "specularColor");
	testLocLight = glGetUniformLocation(testShader, "vLightPosition");
	testLocMVP = glGetUniformLocation(testShader, "mvpMatrix");
	testLocMV  = glGetUniformLocation(testShader, "mvMatrix");
	testLocNM  = glGetUniformLocation(testShader, "normalMatrix");
	testLocTexture = glGetUniformLocation(testShader, "colorMap");
	

	dissolveShader = gltLoadShaderPairWithAttributes("/shaders/Dissolve.vp", "/shaders/Dissolve.fp", 3, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_NORMAL, "vNormal", GLT_ATTRIBUTE_TEXTURE0, "vTexture0");
	dissolveLocDissolveFactor = glGetUniformLocation(dissolveShader, "dissolveFactor");

	flatShader = gltLoadShaderPairWithAttributes("/shaders/FlatShader.vp", "/shaders/FlatShader.fp", 2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_TEXTURE0, "vTexture0");
	flatLocMVP = glGetUniformLocation(flatShader, "mvpMatrix");
	flatLocColorValue = glGetUniformLocation(flatShader, "vColorValue");
	flatLocDissolveFactor = glGetUniformLocation(flatShader, "dissolveFactor");
	flatLocTexture = glGetUniformLocation(flatShader, "colorMap");


	mercuryAngle = 0.0f;
	venusAngle = 0.0f;
	earthAngle = 0.0f;
	marsAngle = 0.0f;
	jupiterAngle = 0.0f;
	saturnAngle = 0.0f;
	uranusAngle = 0.0f;
	neptuneAngle = 0.0f;

	mercuryDist = 3.0f;
	venusDist = 6.0f;
	earthDist = 9.0f;
	marsDist = 12.0f;
	jupiterDist = 15.0f;
	saturnDist = 18.0f;
	uranusDist = 21.0f;
	neptuneDist = 24.0f;

	mercuryStep = 1.0f/mercuryDist;
	venusStep = 1.0f/venusDist;
	earthStep = 1.0f/earthDist;
	marsStep = 1.0f/marsDist;
	jupiterStep = 1.0f/jupiterDist;
	saturnStep = 1.0f/saturnDist;
	uranusStep = 1.0f/uranusDist;
	neptuneStep = 1.0f/neptuneDist;

	timeScalar = 5.0f;
	
}

void SolarSystem::drawPlanet(float angle, float dist, float size, GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack, GLShaderManager &shaderManager)
{
	float px = cos(angle)*dist;
	float pz = sin(angle)*dist;
	GLfloat vFloorColor[] = { 1.0f, 1.0f, 1.0f, 0.75f};
	GLfloat vAmbientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat vDiffuseColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat vSpecularColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	M3DVector4f vLightPos = {0.0f, 3.0f, 0.0f, 1.0f};
	M3DVector4f vLightEyePos;
	M3DMatrix33f normal33;



	m3dTransformVector4(vLightEyePos, vLightPos, cameraMatrix); // get the light position with respect to the saved camera position
	orient33fromMat44(normal33, cameraMatrix);
	//Dprint::add("SolarSystem::drawPlanet - vLightEyePos = (%.2f, %.2f, %.2f, %.2f)", vLightEyePos[0], vLightEyePos[1], vLightEyePos[2], vLightEyePos[3]);
	//Dprint::addMatrix33("normal33: ", normal33);


	modelViewStack.PushMatrix();
		modelViewStack.Translate(px, 0.0f, pz);
		modelViewStack.Scale(size, size, size);		
		//modelViewStack.Rotate(angle*200.0f, 0.0f, 1.0f, 0.0f);
		projectionStack.PushMatrix();
			projectionStack.MultMatrix(modelViewStack.GetMatrix());
			glBindTexture(GL_TEXTURE_2D, uiTextures[2]);
			//shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, modelViewStack.GetMatrix(), projectionStack.GetMatrix(), vLightEyePos,  vWhite, 0);
			/***/
			glUseProgram(testShader);
			glUniform4fv(testLocAmbient, 1, vAmbientColor);
			glUniform4fv(testLocDiffuse, 1, vDiffuseColor);
			glUniform4fv(testLocSpecular, 1, vSpecularColor);
			glUniform3fv(testLocLight, 1, vLightEyePos);
			glUniformMatrix4fv(testLocMVP, 1, GL_FALSE, projectionStack.GetMatrix());
			glUniformMatrix4fv(testLocMV, 1, GL_FALSE, modelViewStack.GetMatrix());
			glUniformMatrix3fv(testLocNM, 1, GL_FALSE, normal33);
			glUniform1i(testLocTexture, 0);
			/*****/
			sphereBatch.Draw();
			glUseProgram(NULL);
		projectionStack.PopMatrix();
	modelViewStack.PopMatrix();
}

void SolarSystem::render(GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack, GLShaderManager &shaderManager)
{
	float px;
	float pz;
	float dissolveFactor;

	dissolveFactor = (sin(earthAngle)+1.0f)*0.5f;
	px = cos(earthAngle)*earthDist;
	pz = sin(earthAngle)*earthDist;
	GLfloat vEyeLight[] = { px, 3.0, pz, 1.0 };
	GLfloat vFloorColor[] = { 1.0f, 1.0f, 1.0f, 0.75f};
	GLfloat vAmbientColor[] = { 0.2f, 0.2f, 0.2f, 0.75f };
	GLfloat vDiffuseColor[] = { 1.0f, 1.0f, 1.0f, 0.75f };
	GLfloat vSpecularColor[] = { 1.0f, 1.0f, 1.0f, 0.75f };
	M3DMatrix33f normal33;
	M3DVector4f vLightEyePos;

	Dprint::add("SolarSystem::render - dissolveFactor = %.2f", dissolveFactor);

	glActiveTexture(activeTextureID);

	modelViewStack.GetMatrix(cameraMatrix); // save off for the camera position
	m3dTransformVector4(vLightEyePos, vEyeLight, cameraMatrix); // get the light position with respect to the saved camera position
	//Dprint::add("SolarSystem::render - vLightEyePos = (%.2f, %.2f, %.2f, %.2f)", vLightEyePos[0], vLightEyePos[1], vLightEyePos[2], vLightEyePos[3]);
	orient33fromMat44(normal33, cameraMatrix);

	modelViewStack.PushMatrix();	
		// draw the 'reflection'
		modelViewStack.PushMatrix();	
			modelViewStack.Translate(0.0f, -3.0f, 0.0f);
			modelViewStack.Rotate(-6.0f, 0.0f, 0.0f, 1.0f);
			// draw the sun
			modelViewStack.PushMatrix();
				modelViewStack.Scale(2.0f, 2.0f, 2.0f);
				projectionStack.PushMatrix();
					projectionStack.MultMatrix(modelViewStack.GetMatrix());
					//shaderManager.UseStockShader(GLT_SHADER_DEFAULT_LIGHT, modelViewStack.GetMatrix(), projectionStack.GetMatrix(), vYellow);
					glBindTexture(GL_TEXTURE_2D, uiTextures[1]);
					shaderManager.UseStockShader(GLT_SHADER_TEXTURE_MODULATE, projectionStack.GetMatrix(), vWhite, 0);
					sphereBatch.Draw();
				projectionStack.PopMatrix();
			modelViewStack.PopMatrix();
		 
			drawPlanet(mercuryAngle, mercuryDist, 0.5f, modelViewStack, projectionStack, shaderManager);
			drawPlanet(venusAngle, venusDist, 0.75f, modelViewStack, projectionStack, shaderManager);
			drawPlanet(earthAngle, earthDist, 1.0f, modelViewStack, projectionStack, shaderManager);
			// draw the moon
			modelViewStack.PushMatrix();
				modelViewStack.Translate(px, 0.0f, pz);
				drawPlanet(-mercuryAngle, mercuryDist, 0.5f, modelViewStack, projectionStack, shaderManager);
			modelViewStack.PopMatrix();

			drawPlanet(marsAngle, marsDist, 0.75f, modelViewStack, projectionStack, shaderManager);
			drawPlanet(jupiterAngle, jupiterDist, 2.0f, modelViewStack, projectionStack, shaderManager);
			drawPlanet(saturnAngle, saturnDist, 1.5f, modelViewStack, projectionStack, shaderManager);
			drawPlanet(uranusAngle, uranusDist, 1.5f, modelViewStack, projectionStack, shaderManager);
			drawPlanet(neptuneAngle, neptuneDist, 1.75f, modelViewStack, projectionStack, shaderManager);
		modelViewStack.PopMatrix();	
		
		// Draw the solid ground
		projectionStack.PushMatrix();
			projectionStack.MultMatrix(modelViewStack.GetMatrix());
			glEnable(GL_BLEND);
			glBindTexture(GL_TEXTURE_2D, uiTextures[0]);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			shaderManager.UseStockShader(GLT_SHADER_TEXTURE_MODULATE, projectionStack.GetMatrix(), vFloorColor, 0);
			floorBatch.Draw();
		projectionStack.PopMatrix();
		
		modelViewStack.Translate(0.0f, 3.0f, 0.0f);
		modelViewStack.Rotate(6.0f, 0.0f, 0.0f, 1.0f);

		// draw the sun again without the fancy shaders and a bit smaller
		modelViewStack.PushMatrix();
			modelViewStack.Scale(1.93f, 1.93f, 1.93f);
			shaderManager.UseStockShader(GLT_SHADER_DEFAULT_LIGHT, modelViewStack.GetMatrix(), projectionStack.GetMatrix(), vYellow);
			sphereBatch.Draw();
		modelViewStack.PopMatrix();
		// draw the sun with the fancy shaders
		modelViewStack.PushMatrix();
			modelViewStack.Scale(2.0f, 2.0f, 2.0f);
			projectionStack.PushMatrix();
				projectionStack.MultMatrix(modelViewStack.GetMatrix());
				glBindTexture(GL_TEXTURE_2D, uiTextures[1]);
				glUseProgram(flatShader);
				glUniform4fv(flatLocColorValue, 1, vDiffuseColor);
				glUniformMatrix4fv(flatLocMVP, 1, GL_FALSE, projectionStack.GetMatrix());
				glUniform1i(flatLocTexture, 0);
				glUniform1f(flatLocDissolveFactor, dissolveFactor);
				sphereBatch.Draw();
				glUseProgram(NULL);
			projectionStack.PopMatrix();
		modelViewStack.PopMatrix();

		 
		drawPlanet(mercuryAngle, mercuryDist, 0.5f, modelViewStack, projectionStack, shaderManager);
		drawPlanet(venusAngle, venusDist, 0.75f, modelViewStack, projectionStack, shaderManager);
		drawPlanet(earthAngle, earthDist, 1.0f, modelViewStack, projectionStack, shaderManager);
		// draw the moon
		modelViewStack.PushMatrix();
			modelViewStack.Translate(px, 0.0f, pz);
			drawPlanet(-mercuryAngle, mercuryDist, 0.5f, modelViewStack, projectionStack, shaderManager);
		modelViewStack.PopMatrix();

		drawPlanet(marsAngle, marsDist, 0.75f, modelViewStack, projectionStack, shaderManager);
		drawPlanet(jupiterAngle, jupiterDist, 2.0f, modelViewStack, projectionStack, shaderManager);
		drawPlanet(saturnAngle, saturnDist, 1.5f, modelViewStack, projectionStack, shaderManager);
		drawPlanet(uranusAngle, uranusDist, 1.5f, modelViewStack, projectionStack, shaderManager);
		drawPlanet(neptuneAngle, neptuneDist, 1.75f, modelViewStack, projectionStack, shaderManager);
	modelViewStack.PopMatrix();

}

void SolarSystem::environmentCalc()
{
	calcDeltaTime();
	mercuryAngle += mercuryStep*deltaTime*timeScalar;
	venusAngle += venusStep*deltaTime*timeScalar;
	earthAngle += earthStep*deltaTime*timeScalar;
	marsAngle += marsStep*deltaTime*timeScalar;
	jupiterAngle += jupiterStep*deltaTime*timeScalar;
	saturnAngle += saturnStep*deltaTime*timeScalar;
	uranusAngle += uranusStep*deltaTime*timeScalar;
	neptuneAngle += neptuneStep*deltaTime*timeScalar;
}

void SolarSystem::localCleanup(){
	glActiveTexture(activeTextureID);
	glDeleteTextures(3, uiTextures);
}