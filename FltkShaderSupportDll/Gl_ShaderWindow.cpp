/**
 * @class	Gl_ShaderWindow
 *
 * @brief	Extension of the FLTK Fl_Gl_Window (http://www.fltk.org/doc-1.3/classFl__Gl__Window.html) to support OpenGl 3.0+ rendering (i.e. no fixed function).
 *
 * @author	Phil
 * @date	3/15/2012
 */

#include "StdAfx.h"
#include "Gl_ShaderWindow.h"

static const GLenum windowBuff[] = { GL_BACK_LEFT };

#define CALC_SCREEN_PIXELS screenWidth*screenHeight*3*1 // XXX This should be unsigned byte

/**
 * @fn	Gl_ShaderWindow::Gl_ShaderWindow(int x,int y,int w,int h,const char *l)
 *
 * @brief	Extension of the FLTK Fl_Gl_Window (http://www.fltk.org/doc-
 * 			1.3/classFl__Gl__Window.html) to support OpenGl 3.0+ rendering (i.e. no fixed
 * 			function).
 *
 * @author	Phil
 * @date	3/15/2012
 *
 * @param	x	The x coordinate.
 * @param	y	The y coordinate.
 * @param	w	The width.
 * @param	h	The height.
 * @param	l	(optional) Label string.
 */
Gl_ShaderWindow::Gl_ShaderWindow(int x,int y,int w,int h,const char *l)
			: Fl_Gl_Window(x,y,w,h,l)
{
	refreshSeconds = 0.01f; // default to 1/100 sec per frame
	initialized = false;
	isPicking = false;

	tmode = WORLD_ROTATE;

	transScalar = 0.01f;
	rotScalar = 0.1f;
	mouseWheelScalar = 0.1f;
	modelTransScalar = 0.1f;

	setEyePos(0, 0, 0);
	setEyeOrient(0, 0, 0); // Default - looking straight down Z with Y up
	eyeVec[0] = 0.0f;
	eyeVec[1] = 0.0f;
	eyeVec[2] = 1.0f;

	setWorldPos(0, 0, -5);
	setWorldOrient(0, 0, 0);

	modelPos[0] = modelPos[1] = modelPos[2] = 0;
	
	Fl::add_timeout(refreshSeconds, timerCallback, this);
}

/**
* @fn	void Gl_ShaderWindow::setFloats(GLfloat *ptr, int amount, ...);
*
* @brief	Sets the floats in an array
*
* @author	Phil
* @date	3/15/2012
*
* @param [in,out]	ptr	If non-null, the pointer.
* @param	amount	the number of indicies.
* @param	...		The values to be set in the array
*/
void Gl_ShaderWindow::setFloats(GLfloat *ptr, int size, ...)
{
	int i;
	GLfloat val;
  
	va_list vl;
	va_start(vl, size);
	for (i = 0; i < size; i++)
	{
		val=(GLfloat)va_arg(vl, double);
		ptr[i] = val;
	}
	va_end(vl);
}

/**
* @fn	static void Gl_ShaderWindow::timerCallback(void* data);
*
* @brief	Drawing callback, triggered by the timer
*
* @author	Phil
* @date	3/15/2012
*
* @param [in,out]	data	pointer to any data to be used/modified within the callback
*/
void Gl_ShaderWindow::timerCallback(void* data){
	Gl_ShaderWindow *gvw = (Gl_ShaderWindow*)data;
	gvw->redraw();
	gvw->environmentCalc();
	Fl::repeat_timeout(gvw->refreshSeconds, timerCallback, data);
}

/**
* @fn	void Gl_ShaderWindow::init(int width, int height);
*
* @brief	Initialises the openGl canvas for shader use. 
* 			Default values are:
* 				glCullFace(GL_BACK);
*				glFrontFace(GL_CCW);
*				glEnable(GL_DEPTH_TEST);
*				glClearColor(0.75, 0.75, 0.75, 0.75);
*				view frustum of 45 degrees horizontal
* 				
* 			Also prints the GL_VERSION and all extensions to the console
*
* @author	Phil
* @date	3/15/2012
*
* @param	width 	The desired width of the canvas (forced to be a multiple of 4)
* @param	height	The height of the canvas.
*/
void Gl_ShaderWindow::init(int width, int height)
{
	screenWidth = (width/4 + 1) * 4;
	screenHeight = height;

	glViewport(0,0,screenWidth,screenHeight);
	if(!initialized)
	{
		GLenum err = glewInit();
		if (GLEW_OK != err) {
			fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
			fl_alert("GLEW Error: %s", glewGetErrorString(err));
			exit(-1);
		}

		fprintf(stderr, "GL_VERSION: %s\n", glGetString(GL_VERSION));
		string str = string((char *)glGetString(GL_EXTENSIONS ));
		char *cstr = new char [str.size()+1];
		strcpy (cstr, str.c_str());
		char *p=strtok (cstr," ");
		while (p!=NULL)
		{
			printf("%s\n", p);
			p=strtok(NULL," ");
		}
		delete[] cstr;

		shaderManager.InitializeStockShaders();

		// Cull backs of polygons
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.75, 0.75, 0.75, 0.75);

		localInit(); 
		initialized = true;
	}

	
	viewFrustum.SetPerspective(45.0f, (float)screenWidth/(float)screenHeight, 0.5f, 1000.0f);
	
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
	modelViewMatrix.LoadIdentity();

	resize();


}

/**
* @fn	void Gl_ShaderWindow::cleanup();
*
* @brief	Cleans up this object.
*
* @author	Phil
* @date	3/15/2012
*/
void Gl_ShaderWindow::cleanup(){
	localCleanup();
}


/**
* @fn	static void Gl_ShaderWindow::Menu_CB(Fl_Widget* wp, void *data);
*
* @brief	Callback from menu selection
*
* @author	Phil
* @date	3/15/2012
*
* @param [in,out]	Fl_Widget	pointer to the widget that is the source of this call
* @param [in,out]	data	  	pointer to any data to be used/modified within the callback
*/
void Gl_ShaderWindow::Menu_CB(Fl_Widget* wp, void *data) {
	Gl_ShaderWindow *glvw = (Gl_ShaderWindow*)data;
	char name[80];
	//((Fl_Menu_Button*)data)->item_pathname(name, sizeof(name)-1);
	glvw->popupMenu->item_pathname(name, sizeof(name)-1);
	if(strcmp(name, "/Eyepoint") == 0){
		glvw->tmode = Gl_ShaderWindow::EYE_ROTATE;
		fprintf(stderr, "EYE_ROTATE\n");
	}else if(strcmp(name, "/Rotate World") == 0){
		glvw->tmode = Gl_ShaderWindow::WORLD_ROTATE;
		fprintf(stderr, "WORLD_ROTATE\n");
	}else if(strcmp(name, "/Move World") == 0){
		glvw->tmode = Gl_ShaderWindow::WORLD_MOVE;
		fprintf(stderr, "WORLD_MOVE\n");
	}else if(strcmp(name, "/Move Model") == 0){
		glvw->tmode = Gl_ShaderWindow::MODEL_MOVE;
		fprintf(stderr, "MODEL_MOVE\n");
	}else if(strcmp(name, "/Pick") == 0){
		glvw->tmode = Gl_ShaderWindow::PICK;
		fprintf(stderr, "PICK\n");
	}
	
}

/**
* @fn	void Gl_ShaderWindow::setPickMatrix(GLdouble x, GLdouble y, GLdouble deltax,
* 		GLdouble deltay, GLint viewport[4]);
*
* @brief	A slightly modified version of the original gluPickMatrix, taken from:
*			http://oss.sgi.com/cgi-bin/cvsweb.cgi/projects/ogl-sample/main/gfx/lib/glu/libutil/project.c?rev=1.4;content-type=text%2Fplain
*
*
* @author	Phil
* @date	3/15/2012
*
* @param	x			cursor x
* @param	y			cursor y
* @param	deltax  	x size of the selection area
* @param	deltay  	y size of the selection area
* @param	viewport	The viewport.
*/
void Gl_ShaderWindow::setPickMatrix(GLdouble x, GLdouble y, GLdouble deltax, GLdouble deltay, GLint viewport[4])
{
	projectionMatrix.PushMatrix();

	if (deltax <= 0 || deltay <= 0) { 
		return;
	}
				
	projectionMatrix.LoadIdentity();
			
	/* Translate and scale the picked region to the entire window */
	GLfloat dx = (float)((viewport[2] - 2 * (x - viewport[0])) / deltax);
	GLfloat dy = (float)((viewport[3] - 2 * (y - viewport[1])) / deltay);
	projectionMatrix.Translate(dx, dy, 0);
	projectionMatrix.Scale((float)(viewport[2] / deltax), (float)(viewport[3] / deltay), 1.0);

	projectionMatrix.MultMatrix(viewFrustum.GetProjectionMatrix());
}

/**
* @fn	int Gl_ShaderWindow::handle(int event);
*
* @brief	The widget class handle(int event) override
*			The virtual method Fl_Widget::handle(int event) is called to handle each event passed to the widget. It can:
*				Change the state of the widget.
*				Call Fl_Widget::redraw() if the widget needs to be redisplayed.
*				Call Fl_Widget::damage(uchar c) if the widget needs a partial-update (assuming you provide support for this in your draw() method).
*				Call Fl_Widget::do_callback() if a callback should be generated.
*				Call Fl_Widget::handle() on child widgets.
*			Events are identified by the integer argument. Other information about the most recent event is stored in static locations
*			and acquired by calling the Fl::event_*() methods. This information remains valid until another event is handled.
*			
*			When implemented in a widget, this function must return 0 if the widget does not use the event or 1 otherwise.
*
* @author	Phil
* @date	3/15/2012
*
* @param	event	the kind of event received
*
*
* @return	0	if the event was not used or understood
*			1	if the event was used and can be deleted
*
*/
int Gl_ShaderWindow::handle(int event){
	float xtemp = 0.0f;
	float ytemp = 0.0f;
	float ztemp = 0.0f;
	int keyCode = 0;
	
	switch(event){
	case FL_PUSH: // set the values
		//printf("Gl_ShaderWindow::handle(FL_PUSH): Mouse = %d, %d\n", Fl::event_x(), Fl::event_y());
		lastMouseX = Fl::event_x();
		lastMouseY = Fl::event_y();

		if ( Fl::event_button() == 3 ) {
			// Dynamically create menu, pop it up
			popupMenu = new Fl_Menu_Button(Fl::event_x(), Fl::event_y(), 80, 1);

			popupMenu->add("Eyepoint",  0, Menu_CB, (void*)this);
			popupMenu->add("Rotate World",  0, Menu_CB, (void*)this);
			popupMenu->add("Move World", 0, Menu_CB, (void*)this);
			popupMenu->add("Move Model", 0, Menu_CB, (void*)this);
			popupMenu->add("Pick", 0, Menu_CB, (void*)this);
			popupMenu->popup();
		}

		if((tmode == PICK)&&( Fl::event_button() == 1 )){
			isPicking = true;
		}
		return 1;
	case FL_DRAG: // subtract from the values
		
		mouseX = lastMouseX - Fl::event_x();
		mouseY = lastMouseY - Fl::event_y();
		lastMouseX = Fl::event_x();
		lastMouseY = Fl::event_y();
		//printf("Gl_ShaderWindow::handle(FL_DRAG): Button = %d, Mouse = %d, %d\n", Fl::event_buttons(), mouseX, mouseY);
		break;
	case FL_RELEASE:
		//printf("Gl_ShaderWindow::handle(FL_RELEASE): Mouse = %d, %d\n", Fl::event_x(), Fl::event_y());
		mouseX = 0;
		mouseY = 0;
		if((tmode == PICK)&&( Fl::event_button() == 1 )){
			isPicking = false;
		}
		break;
	case FL_MOUSEWHEEL:
		//printf("Gl_ShaderWindow::handle(FL_MOUSEWHEEL): Mouse delta = %d, %d\n", Fl::event_dx(), Fl::event_dy());
		if(tmode == WORLD_ROTATE || tmode == WORLD_MOVE){
			ztemp = (Fl::event_dy() * mouseWheelScalar);
			worldPos[2] += ztemp;
		}else if(tmode == EYE_ROTATE || tmode == EYE_MOVE){ // EYE
			float vecSize = Fl::event_dy() * mouseWheelScalar;
			eyePos[0] += (vecSize * eyeVec[0]); 
			eyePos[1] += (vecSize * eyeVec[1]); 
			eyePos[2] += (vecSize * eyeVec[2]); 
		}
		
		break;

	}



	if(tmode == WORLD_MOVE){
		if(Fl::event_button1()){
			worldPos[0] -= mouseX * transScalar;
			worldPos[1] += mouseY * transScalar;
		}else if(Fl::event_button2()){
			worldPos[2] += mouseY * transScalar;
		} 
	}else if(tmode == WORLD_ROTATE){
		if(Fl::event_button1()){
			xtemp = -mouseY * rotScalar;
			ytemp = -mouseX * rotScalar;
			worldOrient[0] += xtemp;
			worldOrient[1] += ytemp;	
		}else if(Fl::event_button2()){
			ztemp = mouseY * transScalar;
			worldPos[2] += ztemp;
		}	
	}else if(tmode == MODEL_MOVE){
		if(Fl::event_button1()){
			modelPos[0] += mouseX * modelTransScalar;
			modelPos[2] += mouseY * modelTransScalar;
		}
		
	}else if(tmode == EYE_MOVE){
		if(Fl::event_button1()){
			eyePos[0] -= mouseX * transScalar; // moving "Left" and "Right" WRT the screen
			eyePos[1] += mouseY * transScalar; // moving "Up" and "Down" WRT the screen
		}else if(Fl::event_button2()){ // eyepos must be calculated with respect to the orientation
			float vecSize = -mouseY * transScalar; // moving "In" and "Out" WRT the screen
			eyePos[0] += (vecSize * eyeVec[0]); 
			eyePos[1] += (vecSize * eyeVec[1]); 
			eyePos[2] += (vecSize * eyeVec[2]); 
		}
	}else if(tmode == EYE_ROTATE){
		if(Fl::event_button1()){
			eyeOrient[0] -= mouseY * rotScalar;
			eyeOrient[1] -= mouseX * rotScalar;
			calcEyeVec();
		}else if(Fl::event_button2()){ // eyepos must be calculated with respect to the orientation
			float vecSize = -mouseY * transScalar; // moving "In" and "Out" WRT the screen
			eyePos[0] += (vecSize * eyeVec[0]); 
			eyePos[1] += (vecSize * eyeVec[1]); 
			eyePos[2] += (vecSize * eyeVec[2]); 
		}
	}


	return __super::handle(event);
}

/**
* @fn	virtual void Gl_ShaderWindow::draw3Dsetup();
*
* @brief	Called before all drawing begins by init()
*
* @author	Phil
* @date	3/15/2012
*/
void Gl_ShaderWindow::draw3Dsetup(){
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		fprintf (stderr, "Gl_ShaderWindow::draw3Dsetup() OpenGL Error: %s\n", errString);
	}
	//Dprint::add("Gl_ShaderWindow::draw3Dsetup() - size = (%.2f, %.2f)", width, height);


	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffers(1, windowBuff);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
* @fn	void Gl_ShaderWindow::postDraw3D();
*
* @brief	Called every frame after all 3D drawing has completed. The default is simply a modelViewMatrixStack.popMatrix();
*
* @author	Phil
* @date	3/15/2012
*/
void Gl_ShaderWindow::preDraw3D() {
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);

	//Dprint::add("viewport = (%d, %d, %d, %d)", viewport[0], viewport[1], viewport[2], viewport[3]);
	//Dprint::add("Gl_ShaderWindow::preDraw3D() - world orient =  = (%.2f, %.2f, %.2f)", worldOrient[0], worldOrient[1], worldOrient[2]);
	//Dprint::add("Gl_ShaderWindow::preDraw3D() - world pos =  = (%.2f, %.2f, %.2f)", worldPos[0], worldPos[1], worldPos[2]);
	//Dprint::add("current dimensions (%d, %d), saved dimensions (%d, %d)", w(), h(), screenWidth, screenHeight);
	calcEyeVec();
		if(!valid()){
		init(w(), h());
	}

	draw3Dsetup();


	modelViewMatrix.PushMatrix();

		// global eye transformations	
		modelViewMatrix.Rotate(eyeOrient[1], 0, 1, 0);
		modelViewMatrix.Rotate(eyeOrient[0], 1, 0, 0);	
		modelViewMatrix.Translate(eyePos[0], eyePos[1], eyePos[2]);

		// global world transformations
		modelViewMatrix.Translate(worldPos[0], worldPos[1], worldPos[2]);

		modelViewMatrix.Rotate(worldOrient[0], 1, 0, 0);
		modelViewMatrix.Rotate(worldOrient[1], 0, 1, 0);
		
		// set up picking
		if(isPicking){
			setPickMatrix(lastMouseX,viewport[3]-lastMouseY,10,10,viewport);
			//printf("Started Picking\n");
		}
}

/**
* @fn	void Gl_ShaderWindow::postDraw3D();
*
* @brief	Called every frame after all 3D drawing has completed. The default cleans up after 
* 			picking and calls modelViewMatrixStack.popMatrix();
*
* @author	Phil
* @date	3/15/2012
*/
void Gl_ShaderWindow::postDraw3D() {
	// finish up picking
	if(isPicking){
		isPicking = false;
		projectionMatrix.PopMatrix();
		//printf("Finished Picking\n");
	}
	modelViewMatrix.PopMatrix();
}

/**
* @fn	void Gl_ShaderWindow::draw2D();
*
* @brief	Contains 2D overlay drawing. This uses fixed function pipline calls, and should be replaced at some time with 
* 			a text overlay shader. I think that the way it could be done would be to create a single grid with all the 
* 			characters and pass that to the vertex shader with the XY index and size of the letter. The vertex shader moves 
* 			the texture so that the letter is centered and then passes off the texture and size to the fragment shader, 
* 			which clips the output so only the particular letter is shown.
* 			
*			Here’s a tutorial on how to make the text texture: http://nehe.gamedev.net/tutorial/2d_texture_font/18002/
*
* @author	Phil
* @date	3/15/2012
*/
void Gl_ShaderWindow::draw2D() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (double)w(), 0.0, (double)h());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushAttrib(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);


	glColor3f(1.0f, 1.0f, 1.0f);

	Dprint::screenPrint(screenWidth, screenHeight);
	Dprint::reset();

	glPopAttrib();
}

Gl_ShaderWindow::~Gl_ShaderWindow(void)
{
}
