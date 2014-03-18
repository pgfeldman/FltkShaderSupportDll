#pragma once
#include <FL/fl_ask.H>
#include <GLTools.h>	// OpenGL toolkit
#include <GLMatrixStack.h>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLGeometryTransform.h>
#include <GL/glut.h>
#include <GL/GLU.h>
#include <math.h>
#include <ctime>
#include "Dprint.h"

#define M_PI       3.14159265358979323846
#define SQR(a)		((a)*(a))

/**
 * @class	DrawableObject
 *
 * @brief	Base class for drawing an object in this framework. Extend to draw your own objects
 *
 * @author	Phil
 * @date	3/14/2012
 */

class DrawableObject
{
public:

	/**
	 * @fn	DrawableObject::DrawableObject(GLuint activeTexture);
	 *
	 * @brief	Constructor.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	activeTexture	The active texture.
	 */
	DrawableObject(GLuint activeTexture);

	/**
	 * @fn	DrawableObject::~DrawableObject(void);
	 *
	 * @brief	Destructor.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 */
	virtual ~DrawableObject(void);

	/**
	 * @fn	void DrawableObject::setColor(float r, float g, float b, float a)
	 *
	 * @brief	Sets a color for the object.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	r	red (0.0 - 1.0)
	 * @param	g	green (0.0 - 1.0)
	 * @param	b	blue (0.0 - 1.0)
	 * @param	a	alphs (0.0 - 1.0)
	 */
	void setColor(float r, float g, float b, float a) {
		curColor[0] = r;
		curColor[1] = g;
		curColor[2] = b;
		curColor[3] = a;
	}

	/**
	 * @fn	void DrawableObject::setPostion(float x, float y, float z)
	 *
	 * @brief	Sets a postion.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	x	
	 * @param	y	vertical
	 * @param	z	T
	 */
	void setPostion(float x, float y, float z){
		position[0] = x;
		position[1] = y;
		position[2] = z;
	}

	/**
	 * @fn	void DrawableObject::setOrientation(float pitch, float roll, float yaw)
	 *
	 * @brief	Sets an orientation.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	pitch
	 * @param	roll 
	 * @param	yaw  
	 */
	void setOrientation(float pitch, float roll, float yaw){
		orientation[0] = pitch;
		orientation[1] = roll;
		orientation[2] = yaw;
	}

	float getXpos(){
		return position[0];
	}

		float getYpos(){
		return position[1];
	}

	float getZpos(){
		return position[2];
	}

	/**
	 * @fn	void DrawableObject::setXpos(float x)
	 *
	 * @brief	Sets the xpos.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	x
	 */
	void setXpos(float x){
		position[0] = x;
	}

	/**
	 * @fn	void DrawableObject::setYpos(float y)
	 *
	 * @brief	Sets the ypos.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	y	
	 */
	void setYpos(float y){
		position[1] = y;
	}

	/**
	 * @fn	void DrawableObject::setZpos(float z)
	 *
	 * @brief	Sets the zpos.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	z	
	 */
	void setZpos(float z){
		position[2] = z;
	}

	/**
	 * @fn	void DrawableObject::setPitch(float pitch)
	 *
	 * @brief	Sets rotation around the x-axis
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	pitch
	 */
	void setPitch(float pitch){
		orientation[0] = pitch;
	}

	/**
	 * @fn	void DrawableObject::setRoll(float roll)
	 *
	 * @brief	Sets rotation around the z-axis
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	roll
	 */
	void setRoll(float roll){
		orientation[1] = roll;
	}

	/**
	 * @fn	void DrawableObject::setYaw(float yaw)
	 *
	 * @brief	Sets rotation around the y-axis
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	yaw	
	 */
	void setYaw(float yaw){
		orientation[2] = yaw;
	}

	/**
	 * @fn	void DrawableObject::setScalar(float s)
	 *
	 * @brief	Sets an overall scaling value
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	s	
	 */
	void setScalar(float s){
		scalar = s;
	}

	float getScalar(){
		return scalar;
	}

	/**
	 * @fn	void DrawableObject::calcDeltaTime()
	 *
	 * @brief	Calculates the delta time between the current and previous frame
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 */
	void calcDeltaTime(){
		curTime = clock();
		deltaTime = (float)(curTime-prevTime)/CLOCKS_PER_SEC;
		prevTime = curTime;
		//Dprint::add("prev time = %d, cur time = %d, deltaTime = %f", prevTime, curTime, deltaTime);
	}

	/**
	 * @fn	void DrawableObject::draw3dString(float x, float y, float z, char *string, void* font)
	 *
	 * @brief	Draw a text string in 3D space
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	x			  
	 * @param	y			  
	 * @param	z			  	
	 * @param 	string	the string.
	 * @param	font  	the font.
	 */
	void draw3dString(float x, float y, float z,  char *string,  void* font){
		glPushAttrib(GL_LIGHTING);
		glDisable(GL_LIGHTING);
		glRasterPos3f(x, y, z);
		for(char* c = string; *c != '\0'; c++){
			glutBitmapCharacter(font, *c);
		}
		glPopAttrib();
	}

	/*
	* Methods related to collision detection
	*/

	/**
	 * @fn	float DrawableObject::getCollisionRadius()
	 *
	 * @brief	Gets the collision radius.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @return	The collision radius.
	 */
	float getCollisionRadius(){
		return collisionRadius;
	};

	/**
	 * @fn	void DrawableObject::calcCollisionPoints()
	 *
	 * @brief	Calculates the collision points.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 */
	void calcCollisionPoints(){};

	/**
	 * @fn	void DrawableObject::setCollisionSphere(float x, float y, float z, float radius)
	 *
	 * @brief	Sets a collision sphere.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	x	  	
	 * @param	y	  
	 * @param	z	  
	 * @param	radius	The radius.
	 */
	void setCollisionSphere(float x, float y, float z, float radius){
		collisionPoint[0] = x;
		collisionPoint[1] = y;
		collisionPoint[2] = z;
		collisionRadius = radius;
	};

	/*
	 * @fn	float DrawableObject::aabbSphereIntersect(const float Bmin[], const float Bmax[],
	 * 		const float C[], float r)
	 * 		
	 * @brief "A Simple Method for Box-Sphere Intersection Testing" by Jim Arvo, in "Graphics Gems", Academic Press, 1990.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 * 			
	 * This routine tests for intersection between an 3-dimensional             
	 * axis-aligned box and an 3-dimensional sphere.  
	 *
	 *  @param    n;     The dimension of the space.           
	 *  @param  Bmin[];  The minimum of the box for each axis. 
	 *  @param  Bmax[];  The maximum of the box for each axis. 
	 *  @param  C[];     The sphere center in n-space.         
	 *  @param  r;       The radius of the sphere.
	 *       
	 *  @return the depth of collision if colliding, otherwise 0.0                                                                  
	 */
	float aabbSphereIntersect(const float Bmin[], const float Bmax[], const float C[], float r);

	/**
	 * @fn	bool DrawableObject::LoadTGATexture(const char *szFileName, GLenum minFilter,
	 * 		GLenum magFilter, GLenum wrapMode);
	 *
	 * @brief	Loads a tga texture.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	szFileName	Filename of the file.
	 * @param	minFilter 	A filter specifying the minimum.
	 * @param	magFilter 	A filter specifying the mag.
	 * @param	wrapMode  	The wrap mode.
	 *
	 * @return	true if it succeeds, false if it fails.
	 */
	bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
	// Load in a BMP file as a texture. Allows specification of the filters and the wrap mode

	/**
	 * @fn	bool DrawableObject::LoadBMPTexture(const char *szFileName, GLenum minFilter,
	 * 		GLenum magFilter, GLenum wrapMode);
	 *
	 * @brief	Loads a bitmap texture.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	szFileName	Filename of the file.
	 * @param	minFilter 	A filter specifying the minimum.
	 * @param	magFilter 	A filter specifying the mag.
	 * @param	wrapMode  	The wrap mode.
	 *
	 * @return	true if it succeeds, false if it fails.
	 */
	bool LoadBMPTexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);

	/**
	 * @fn	virtual void DrawableObject::render(GLMatrixStack &modelViewStack,
	 * 		GLMatrixStack &projectionStack, GLShaderManager &shaderManager) = 0;
	 *
	 * @brief	Renders this object.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param [in,out]	modelViewStack 	modelView matrix stack
	 * @param [in,out]	projectionStack	projection matrix stack
	 * @param [in,out]	shaderManager  	OpenGlSuperBible (5th ed) GLShaderManager
	 */
	virtual void render(GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack, GLShaderManager &shaderManager) = 0;

	/**
	 * @fn	void DrawableObject::pickRender(GLMatrixStack &modelViewStack,
	 * 		GLMatrixStack &projectionStack, GLShaderManager &shaderManager)
	 *
	 * @brief	Set up rendering of this object for picking
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param [in,out]	modelViewStack 	modelView matrix stack
	 * @param [in,out]	projectionStack	projection matrix stack
	 * @param [in,out]	shaderManager  	OpenGlSuperBible (5th ed) GLShaderManager
	 */
	void pickRender(GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack, GLShaderManager &shaderManager){
		if(pickQueryResult == READY)
			glBeginQuery(GL_ANY_SAMPLES_PASSED, drawQuery);

		render(modelViewStack, projectionStack, shaderManager);

		if(pickQueryResult == READY)
			glEndQuery(GL_ANY_SAMPLES_PASSED);
	};

	/**
	 * @enum	PICK_RESULT
	 *
	 * @brief	Values that represent PICK_RESULT.
	 */
	static enum PICK_RESULT{UNAVAILABLE, HIT, MISS, READY, PICK_ERROR};

	/**
	 * @fn	PICK_RESULT DrawableObject::pickResult()
	 *
	 * @brief	Determine the result of the pick request
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @return	.
	 */
	PICK_RESULT pickResult(){
		GLint result;

		if(pickQueryResult == PICK_ERROR){
			return UNAVAILABLE;
		}

		/** use this if we don't want to wait for the result. Doesn't seem to be working though
		glGetQueryObjectiv(drawQuery, GL_QUERY_RESULT_AVAILABLE, &result);
		if(result == 0){		
			pickQueryResult = UNAVAILABLE;
			return UNAVAILABLE;
		}
		pickQueryResult = READY;
		/***/

		glGetQueryObjectiv(drawQuery, GL_QUERY_RESULT, &result); // seems to slow down the rendering
		if(result)
			return HIT;
		else
			return MISS; 
	};

	/**
	 * @fn	virtual void DrawableObject::environmentCalc() = 0;
	 *
	 * @brief	Perform calculations required for this object to do something on a frame-by-frame basis
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 */
	virtual void environmentCalc() = 0;

	/**
	 * @fn	virtual void DrawableObject::localCleanup() = 0;
	 *
	 * @brief	Prepare for deletion of this object
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 */
	virtual void localCleanup() = 0;

	/**
	 * @fn	void DrawableObject::cleanup()
	 *
	 * @brief	Prepare for deletion of this object
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 */
	void cleanup(){
		if(drawQuery != 0)
			glDeleteQueries(1, &drawQuery);
		localCleanup();
	};

	/**
	 * @fn	inline float DrawableObject::degToRad(float degAngle)
	 *
	 * @brief	Degrees to radians.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	degAngle	The degrees angle.
	 *
	 * @return	.
	 */
	static inline float degToRad(float degAngle){	return (float)(degAngle * M_PI/180.0f);	};

	/**
	 * @fn	inline float DrawableObject::radToDeg(float radAngle)
	 *
	 * @brief	Radians to degrees.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	radAngle	The radians angle.
	 *
	 * @return	.
	 */
	static inline float radToDeg(float radAngle){	return (float)(radAngle * 180.f/M_PI);	};

	static inline double frand(double fMin, double fMax)
	{
		double f = (double)rand() / RAND_MAX;
		return fMin + f * (fMax - fMin);
	}

protected:

	/**
	 * @fn	void DrawableObject::copyArray(int num, float *source, float *target)
	 *
	 * @brief	Deep array copy
	 *
	 * @author	Phil
	 * @date	3/25/2012
	 *
	 * @param	num			  	Number ofelements in the array
	 * @param	source			source array
	 * @param [in,out]			target array
	 */

	void copyArray(int num, const float *source, float *target){
		for(int i = 0; i < num; ++i){
			target[i] = source[i];
		}
	}

	/**
	 * @fn	void DrawableObject::setPosFromMatrix(float *vec)
	 *
	 * @brief	Copies the xyz position from an openGL 4x4 matrix i.e. matrix[12], matrix[13], matrix[14]
	 *
	 * @author	Phil
	 * @date	3/25/2012
	 *
	 * @param [in,out]	the vector to be set with the postion
	 */

	void setPosFromMatrix(float *vec){copyArray(3, &matrix[12], vec);};

	/**
	 * @fn	void DrawableObject::setFloats(GLfloat *ptr, int amount, ...);
	 *
	 * @brief	Sets the floats.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param [in,out]	ptr	If non-null, the pointer.
	 * @param	amount	   	The amount.
	 */
	void setFloats(GLfloat *ptr, int amount, ...);



	/**
	 * @fn	void DrawableObject::showNormals(bool show)
	 *
	 * @brief	Shows the normals.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param	show	true to show, false to hide.
	 */
	void showNormals(bool show){	drawNormals = show;	};

	/**
	 * @fn	void DrawableObject::orient33fromMat44(M3DMatrix33f &mat33, const M3DMatrix44f mat44);
	 *
	 * @brief	pull the orientation 3x3 matrix from a 4x4 matrix
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param [in,out]	mat33	The third mat 3.
	 * @param	mat44		 	The fourth mat 4.
	 */
	void orient33fromMat44(M3DMatrix33f &mat33, const M3DMatrix44f mat44);

	/**
	 * @fn	void DrawableObject::orient44fromMat44(M3DMatrix44f &targetMat,
	 * 		const M3DMatrix44f sourceMat44);
	 *
	 * @brief	pull the orientation 4x4 matrix from a 4x4 matrix - basically zero out the position values
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 *
	 * @param [in,out]	targetMat	Target mat.
	 * @param	sourceMat44		 	Source mat 44.
	 */
	void orient44fromMat44(M3DMatrix44f &targetMat, const M3DMatrix44f sourceMat44);

	/**
	 * @fn	void DrawableObject::placeInGlobalSpace();
	 *
	 * @brief	Perform the matrix multiplications that put this object where we ant it to go in global space
	 * 			Currently commented out.
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 */
	void placeInGlobalSpace();

	/**
	 * @summary	The current color.
	 */
	GLfloat curColor[4];

	/**
	 * @summary	The position and orientation matrix.
	 */
	GLfloat matrix[16];

	/**
	 * @summary	true to draw normals.
	 */
	bool drawNormals;

	/**
	 * @summary	true to show, false to hide the data.
	 */
	bool showData;

	/**
	 * @summary	The XYZ position.
	 */
	float position[3];

	/**
	 * @summary	The orientation (pitch Roll Yaw)
	 */

	float orientation[3];

	/**
	 * @summary	The gloabl scalar.
	 */
	float scalar;

	/**
	 * @summary	The bounding sphere radius.
	 */
	float boundingSphereRadius;

	/**
	 * @summary	The minimum axis-aligned bounding box XYZ coordinates
	 */
	float minAARB[3];

	/**
	 * @summary	The maximum axis-aligned bounding box XYZ coordinates
	 */
	float maxAARB[3];

	/**
	 * @summary	The position of the point that we'll test collisions against
	 */
	float collisionPoint[3];

	/**
	 * @summary	The radius of the point that we'll test collisions against
	 */
	float collisionRadius;

	/**
	 * @summary	The xformed collision point
	 */
	float xformed[3];

	/**
	 * @summary	The current time.
	 */
	clock_t curTime;

	/**
	 * @summary	Time of the previous frame.
	 */
	clock_t prevTime;

	/**
	 * @summary	delta time between frames
	 */
	float deltaTime;

	/**
	* @summary color vectors 
	*/
	float vRed[4];
	float vGreen[4];
	float vBlue[4];
	float vWhite[4];
	float vGray[4];
	float vCyan[4];
	float vMagenta[4];
	float vYellow[4];

	/**
	 * @summary	Identifier for the active texture.
	 */
	GLuint activeTextureID;

	/**
	 * @summary	The draw query ID.
	 */
	GLuint drawQuery;

	/**
	 * @summary	The pick query result.
	 */
	PICK_RESULT pickQueryResult;
};

