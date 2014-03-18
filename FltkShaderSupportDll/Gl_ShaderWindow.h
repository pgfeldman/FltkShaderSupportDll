#pragma once

#include <fl\fl_gl_window.h>
#include <FL/Fl_Menu_Button.h>
#include <FL/fl_ask.H>
#include <GLTools.h>	// OpenGL toolkit
#include <GLMatrixStack.h>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLGeometryTransform.h>

#include <math.h>


#include <GL/glut.h>
#include <GL/GLU.h>
#include "Dprint.h"
#include "ScreenRepaint.h"

#define M_PI       3.14159265358979323846

/**
 * @class	Gl_ShaderWindow
 *
 * @brief	Extension of the FLTK Fl_Gl_Window (http://www.fltk.org/doc-1.3/classFl__Gl__Window.html) to support OpenGl 3.0+ rendering (i.e. no fixed function).
 *
 * @author	Phil
 * @date	3/15/2012
 */

class Gl_ShaderWindow : public Fl_Gl_Window
{
public:

	/**
	 * @fn	Gl_ShaderWindow::Gl_ShaderWindow(int x,int y,int w,int h,const char *l=0);
	 *
	 * @brief	Constructor.
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

	Gl_ShaderWindow(int x,int y,int w,int h,const char *l=0);

	/**
	 * @fn	Gl_ShaderWindow::~Gl_ShaderWindow(void);
	 *
	 * @brief	Destructor.
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */

	virtual ~Gl_ShaderWindow(void);

	/**
	 * @fn	void Gl_ShaderWindow::setEyePos(float x, float y, float z)
	 *
	 * @brief	Sets an eye position.
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 *
	 * @param	x	
	 * @param	y	
	 * @param	z	
	 */
	void setEyePos(float x, float y, float z){
		eyePos[0] = x;
		eyePos[1] = y;
		eyePos[2] = z;
	}

	/**
	 * @fn	void Gl_ShaderWindow::setEyeOrient(float pitch, float roll, float yaw)
	 *
	 * @brief	Sets an eye orientation
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 *
	 * @param	pitch	
	 * @param	roll 
	 * @param	yaw  
	 */
	void setEyeOrient(float pitch, float roll, float yaw){
		eyeOrient[0] = pitch;
		eyeOrient[1] = roll;
		eyeOrient[2] = yaw;
	}

	/**
	 * @fn	void Gl_ShaderWindow::calcEyeVec()
	 *
	 * @brief	Calculates the eye vector based on the eye orientation 
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	void calcEyeVec(){
		float phi = degToRad(90.0f - eyeOrient[0]); 
		float theta = degToRad(eyeOrient[1]);

		eyeVec[0] = -sin(theta)*sin(phi);
		eyeVec[1] = cos(phi);
		eyeVec[2] = cos(theta)*sin(phi);
	}

	/**
	 * @fn	void Gl_ShaderWindow::setWorldPos(float x, float y, float z)
	 *
	 * @brief	Sets the position of the global origin. In other words, all objects placed in the scene will be relative to this point
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 *
	 * @param	x	The float to process.
	 * @param	y	The float to process.
	 * @param	z	The float to process.
	 */
	void setWorldPos(float x, float y, float z){
		worldPos[0] = x;
		worldPos[1] = y;
		worldPos[2] = z;
	}

	/**
	 * @fn	void Gl_ShaderWindow::setWorldOrient(float pitch, float roll, float yaw)
	 *
	 * @brief	Sets the orientation of the global origin. In other words, all objects placed in the scene will be relative to this orientation
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 *
	 * @param	pitch	The pitch.
	 * @param	roll 	The roll.
	 * @param	yaw  	The yaw.
	 */
	void setWorldOrient(float pitch, float roll, float yaw){
		worldOrient[0] = pitch;
		worldOrient[1] = roll;
		worldOrient[2] = yaw;
	}

	/**
	 * @fn	void Gl_ShaderWindow::setRotScalar(float scalar)
	 *
	 * @brief	Sets the scalar for transforming mouse movement to degrees
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 *
	 * @param	scalar	The scalar.
	 */
	void setRotScalar(float scalar){ rotScalar = scalar; };

	/**
	 * @fn	void Gl_ShaderWindow::setTransScalar(float scalar)
	 *
	 * @brief	Sets the scalar for transforming mouse movement to vovement of the camera or 'universe'
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 *
	 * @param	scalar	The scalar.
	 */
	void setTransScalar(float scalar){ transScalar = scalar; };

	/**
	 * @fn	void Gl_ShaderWindow::setMouseWheelScalar(float scalar)
	 *
	 * @brief	Sets a mouse wheel scalar for translation to z-motion of the camera
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 *
	 * @param	scalar	The scalar.
	 */
	void setMouseWheelScalar(float scalar){ mouseWheelScalar = scalar; };

	/**
	 * @fn	void Gl_ShaderWindow::setEyeTransformMode()
	 *
	 * @brief	Sets the eye transform mode to EYE_ROTATE
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	void setEyeTransformMode(){tmode = EYE_ROTATE;};

	/**
	 * @fn	void Gl_ShaderWindow::setWorldTransformMode()
	 *
	 * @brief	Sets the world transform mode to WORLD_ROTATE
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	void setWorldTransformMode(){tmode = WORLD_ROTATE;};

	/**
	 * @fn	float Gl_ShaderWindow::degToRad(float degAngle)
	 *
	 * @brief	Degrees to radians.
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 *
	 * @param	degAngle	The angle in degrees.
	 *
	 * @return	the angle in radians
	 */
	float degToRad(float degAngle){	return (float)(degAngle * M_PI/180.0f);	};	

	/**
	 * @fn	float Gl_ShaderWindow::radToDeg(float radAngle)
	 *
	 * @brief	Radians to degrees.
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 *
	 * @param	radAngle	The angle in radians
	 *
	 * @return	the angle in degrees
	 */
	float radToDeg(float radAngle){	return (float)(radAngle * 180.f/M_PI);	};

	/**
	 * @fn	void Gl_ShaderWindow::setRefreshSeconds(float duration)
	 *
	 * @brief	Sets the minimum 'pause' between frames. Typical values range between 0.001 to 0.1 seconds. 
	 * 			Complex frames may take longer to draw
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 *
	 * @param	duration	The duration.
	 */
	void setRefreshSeconds(float duration) {refreshSeconds = duration;};

	/**
	 * @fn	void Gl_ShaderWindow::init(int width, int height);
	 *
	 * @brief	Initialises the openGl canvas
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 *
	 * @param	width 	The width.
	 * @param	height	The height.
	 */
	void init(int width, int height);

	/**
	 * @fn	virtual void Gl_ShaderWindow::localInit() = 0;
	 *
	 * @brief	any additional initialization required by a subclass should add their code here. It is called in the correct order within init()
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	virtual void localInit() = 0;

	/**
	 * @fn	virtual void Gl_ShaderWindow::resize()
	 *
	 * @brief	Resizes the canvas and reinitializes any OpenGl values that may need it
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	virtual void resize(){};

	/**
	 * @fn	virtual void Gl_ShaderWindow::draw3Dsetup();
	 *
	 * @brief	Called before all drawing begins by init()
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	virtual void draw3Dsetup();

	/**
	 * @fn	void Gl_ShaderWindow::preDraw3D();
	 *
	 * @brief	Called every frame before drawing. Contains things like glClear()
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	void preDraw3D(); 

	/**
	 * @fn	void Gl_ShaderWindow::postDraw3D();
	 *
	 * @brief	Called every frame after all 3D drawing has completed. The default cleans up after 
	 * 			picking and calls modelViewMatrixStack.popMatrix();
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	void postDraw3D(); 

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
	void draw2D();

	/**
	 * @fn	virtual void Gl_ShaderWindow::environmentCalc() = 0;
	 *
	 * @brief	Do any calculations for things that happen as a function of time in the running system and call the 
	 * 			environmentCalc() methods in any DrawableObject
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	virtual void environmentCalc() = 0;

	/**
	 * @fn	void Gl_ShaderWindow::cleanup();
	 *
	 * @brief	Cleans up this object.
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	void cleanup();

	/**
	 * @fn	virtual void Gl_ShaderWindow::localCleanup() = 0;
	 *
	 * @brief	Place to do cleanup in inhereting classes, such as calling delete() on DrawableObjects
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	virtual void localCleanup() = 0;

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
	int handle(int event);

	static inline double frand(double fMin, double fMax)
	{
		double f = (double)rand() / RAND_MAX;
		return fMin + f * (fMax - fMin);
	}

	/**
	 * @enum	TRANSFORM_MODE
	 *
	 * @brief	Values that represent TRANSFORM_MODE.
	 */

	static enum TRANSFORM_MODE{EYE_MOVE, EYE_ROTATE, WORLD_MOVE, WORLD_ROTATE, PICK, MODEL_MOVE, MODEL_ROTATE};

protected:

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
	void setFloats(GLfloat *ptr, int amount, ...);

	/**
	 * @fn	static void Gl_ShaderWindow::timerCallback(void* data);
	 *
	 * @brief	Drawing callback, triggered by the timer
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 *
	 * @param [in,out]	pointer to any data to be used/modified within the callback
	 */
	static void timerCallback(void* data);

	/**
	 * @fn	static void Gl_ShaderWindow::Menu_CB(Fl_Widget*, void *data);
	 *
	 * @brief	Callback from menu selection
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 *
	 * @param [in,out]	parameter1	pointer to the widget that is the source of this call
	 * @param [in,out]	data	  	pointer to any data to be used/modified within the callback
	 */
	static void Menu_CB(Fl_Widget* wp, void *data);

	/**
	 * @fn	void Gl_ShaderWindow::setPickMatrix(GLdouble x, GLdouble y, GLdouble deltax,
	 * 		GLdouble deltay, GLint viewport[4]);
	 *
	 * @brief	A slightly modified version of the original gluPickMatrix, taken from:
	 *				http://oss.sgi.com/cgi-bin/cvsweb.cgi/projects/ogl-sample/main/gfx/lib/glu/libutil/project.c?rev=1.4;content-type=text%2Fplain
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
	void setPickMatrix(GLdouble x, GLdouble y, GLdouble deltax, GLdouble deltay, GLint viewport[4]);

	/**
	 * @summary	The refresh rate in seconds. Typically 0.001 to 0.1
	 */
	float refreshSeconds;

	/**
	 * @summary	true if graphics have been initialized.
	 */
	bool initialized;

	/**
	 * @summary	true if we are currently picking
	 */
	bool isPicking;

	/**
	 * @summary	The viewing mode.
	 */
	TRANSFORM_MODE tmode;

	/**
	 * @summary	The eye position.
	 */
	GLfloat eyePos[3];

	/**
	 * @summary	The eye orientation.
	 */
	GLfloat eyeOrient[3];

	/**
	 * @summary	The eye vector.
	 */
	GLfloat eyeVec[3];

	/**
	 * @summary	The world position.
	 */
	GLfloat worldPos[3];

	/**
	 * @summary	The world orientation.
	 */
	GLfloat worldOrient[3];

	/**
	 * @summary	The model position.
	 */
	GLfloat modelPos[3];
	GLfloat modelTransScalar;

	/**
	 * @summary	The mouse rotation angle scalar.
	 */
	float rotScalar;

	/**
	 * @summary	The mouse translation scalar.
	 */
	float transScalar;

	/**
	 * @summary	The mouse wheel (Z) scalar.
	 */
	float mouseWheelScalar;

	/**
	 * @summary	The last mouse x coordinate.
	 */
	int lastMouseX;

	/**
	 * @summary	The last mouse y coordinate.
	 */
	int lastMouseY;

	/**
	 * @summary	The current mouse x coordinate.
	 */
	int mouseX;

	/**
	 * @summary	The current mouse y coordinate.
	 */
	int mouseY;

	/**
	 * @summary	Width of the screen. Must be a multiple of 4
	 */
	GLsizei	 screenWidth;			

	/**
	 * @summary	Height of the screen.
	 */
	GLsizei  screenHeight;			

	/**
	 * @summary	The popup menu.
	 */
	Fl_Menu_Button *popupMenu;

	/**
	 * @summary	The model view matrix from the openGl SuperBible 5th Edition. (http://code.google.com/p/oglsuperbible5/source/browse/trunk/Src/GLTools/include/GLMatrixStack.h)
	 */
	GLMatrixStack       modelViewMatrix;

	/**
	 * @summary	The projection matrix from the openGl SuperBible 5th Edition. (http://code.google.com/p/oglsuperbible5/source/browse/trunk/Src/GLTools/include/GLMatrixStack.h)
	 */
	GLMatrixStack       projectionMatrix;

	/**
	 * @summary	The view frustum from the openGl SuperBible 5th Edition. (http://code.google.com/p/oglsuperbible5/source/browse/trunk/Src/GLTools/include/GLFrustum.h)
	 */
	GLFrustum           viewFrustum;

	/**
	 * @summary	The transform pipeline from the openGl SuperBible 5th Edition. (http://code.google.com/p/oglsuperbible5/source/browse/trunk/Src/GLTools/include/GLGeometryTransform.h)
	 */
	GLGeometryTransform transformPipeline;

	/**
	 * @summary	Shader manager from the openGl SuperBible 5th Edition. (http://code.google.com/p/oglsuperbible5/source/browse/trunk/Src/GLTools/src/GLShaderManager.cpp)
	 */
	GLShaderManager     shaderManager;

	
};

/**
// End of Gl_ShaderWindow.h
 */

