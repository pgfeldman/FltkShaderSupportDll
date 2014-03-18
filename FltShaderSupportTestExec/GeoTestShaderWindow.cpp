#include "StdAfx.h"
#include "GeoTestShaderWindow.h"

// instance the globals?
GLTriangleBatch triangleBatch;


GeoTestShaderWindow::GeoTestShaderWindow(int x,int y,int w,int h,const char *l)
	: Gl_ShaderWindow(x,y,w,h,l)
{
	setWorldPos(0, -2, -25);
	
}


GeoTestShaderWindow::~GeoTestShaderWindow(void)
{
}


void GeoTestShaderWindow::localInit(){

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	gridStage = new GridStage(10.0f, 10);
	solarSystem = new SolarSystem(GL_TEXTURE0);

	screenRepaint = new ScreenRepaint(GL_TEXTURE1, "/shaders/texpassthrough.vs", "/shaders/gaussianGlow.fs");
}

void GeoTestShaderWindow::resize(){
	screenRepaint->resize(screenWidth,screenHeight);
}

void GeoTestShaderWindow::environmentCalc(){
	solarSystem->environmentCalc();
}

void GeoTestShaderWindow::draw(){
	DrawableObject::PICK_RESULT pickResult;

	
		// 3D draw stuff here!

	if(isPicking){
		preDraw3D();
			gridStage->pickRender(modelViewMatrix, projectionMatrix, shaderManager);
			pickResult = gridStage->pickResult();
			if(pickResult == DrawableObject::HIT)
				printf("Gridstage Hit\n");
			else if(pickResult == DrawableObject::MISS)
				printf("Gridstage Miss\n");
			else if(pickResult == DrawableObject::UNAVAILABLE)
				printf("Gridstage Unavailable\n");
		postDraw3D();
	}

	preDraw3D();
		gridStage->render(modelViewMatrix, projectionMatrix, shaderManager);
		solarSystem->render(modelViewMatrix, projectionMatrix, shaderManager);

	postDraw3D();
	screenRepaint->render(modelViewMatrix, projectionMatrix, shaderManager);
	
	draw2D();
}

void GeoTestShaderWindow::localCleanup(){
	gridStage->cleanup();
	delete(gridStage);

	solarSystem->cleanup();
	delete(solarSystem);

	screenRepaint->cleanup();
	delete (screenRepaint);
}