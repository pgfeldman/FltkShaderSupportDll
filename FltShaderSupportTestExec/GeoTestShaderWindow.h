#pragma once
#include "gl_shaderwindow.h"
#include "GridStage.h"
#include "SolarSystem.h"

class GeoTestShaderWindow :
	public Gl_ShaderWindow
{
public:
	GeoTestShaderWindow(int x,int y,int w,int h,const char *l);
	virtual ~GeoTestShaderWindow(void);
	void environmentCalc();
	void draw();
	void localInit();
	void resize();
	virtual void localCleanup();

private:
	GridStage			*gridStage;
	SolarSystem			*solarSystem;
	ScreenRepaint		*screenRepaint;
};

