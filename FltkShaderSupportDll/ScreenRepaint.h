#pragma once
#include "DrawableObject.h"

/**
 * @class	ScreenRepaint
 *
 * @brief	DrawableObject that takes a 'snapshot' of what's been already been drawn and after processing, renders the result to a screen-aligned textured quad
 *
 * @author	Phil
 * @date	3/15/2012
 */

class ScreenRepaint :
	public DrawableObject
{
public:

	/**
	 * @fn	ScreenRepaint::ScreenRepaint(GLuint activeTexture, const char* vertFileName,
	 * 		const char* fragFileName);
	 *
	 * @brief	Constructor.
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 *
	 * @param	activeTexture	The active texture.
	 * @param	vertFileName 	Filename of the vertex shader code.
	 * @param	fragFileName 	Filename of the fragment shader code.
	 */
	ScreenRepaint(GLuint activeTexture, const char* vertFileName, const char* fragFileName);

	/**
	 * @fn	ScreenRepaint::~ScreenRepaint(void);
	 *
	 * @brief	Destructor.
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	~ScreenRepaint(void);

	/**
	 * @fn	void ScreenRepaint::resize(int width, int height);
	 *
	 * @brief	Resizes the screen-aligned quad and adjusts buffer memory
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 *
	 * @param	width 	The width.
	 * @param	height	The height.
	 */
	void resize(int width, int height);

	/**
	 * @fn	void ScreenRepaint::render(GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack,
	 * 		GLShaderManager &shaderManager);
	 *
	 * @brief	Renders the quad. Note! THis should be called *after* all 3D drawing (i.e. after postDraw3D()), but *before* draw2D()
	 * 			The steps are as follows:
	 * 				First bind the PBO as the pack buffer, then read the pixels directly to the PBO and unbind
	 * 				Next bind the PBO as the unpack buffer, then push the pixels straight into the texture
	 * 					Setup texture unit for the render surface
	 * 					switch to the GL_TEXTURE* value that we are using as our screen texture holder
	 * 					write the pixels
	 * 					unbind
	 * 				 Draw full screen quad with screen textures, calling setupScreenRenderProg() to access the chaders for the effect desired.
	 * 				
	 * 			
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 *
	 * @param [in,out]	modelViewStack 	modelviewMatrix stack
	 * @param [in,out]	projectionStack	projection matrix stack
	 * @param [in,out]	shaderManager  	Manager of default shaders
	 */
	void render(GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack, GLShaderManager &shaderManager);

	/**
	 * @fn	void ScreenRepaint::environmentCalc();
	 *
	 * @brief	Environment calculation. In this case, do nothing
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	void environmentCalc();

	/**
	 * @fn	void ScreenRepaint::localCleanup();
	 *
	 * @brief	Make sure default FBO is bound
	 * 			Cleanup textures
	 * 			Delete detached textures
	 * 			Delete the pixel buffer
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	void localCleanup();

protected:

	/**
	 * @fn	void ScreenRepaint::initFrameBufferObjects();
	 *
	 * @brief	Initialises the frame buffer objects. If this is the first time this method is called, then the 
	 * 			shader programs are read in and the texture and pixel buffer object handles are created and stored.
	 * 			Otherwise, if this is called as the result of a resize, a new ortho2D matrix is recalculated, and if 
	 * 			the canvas is larger than previous sizes, the pixel buffer size is re-allocated.
	 * 
	 * @todo		Fix bug that causes texture to draw incorrectly at large canvas sizes.
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	void ScreenRepaint::initFrameBufferObjects();

	/**
	 * @fn	void ScreenRepaint::setupScreenRenderProg(const M3DMatrix44f mat);
	 *
 	 * @brief	Sets up the screen render shader programs
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 *
	 * @param	mat	The combined projection and model view matrix ("mvpMatrix")
	 */
	void setupScreenRenderProg(const M3DMatrix44f mat);

	/**
	 * @summary	Width of the screen.
	 */

	GLsizei				screenWidth;			// Desired window or desktop width

	/**
	 * @summary	Height of the screen.
	 */

	GLsizei				screenHeight;			// Desired window or desktop height

	/**
	 * @summary	true if fbo initialized.
	 */

	bool				fboInitialized;

	/**
	 * @summary	The ortho matrix.
	 */

	M3DMatrix44f        orthoMatrix; 

	/**
	 * @summary	Filename of the vertex file.
	 */

	char*				vertexFileName;

	/**
	 * @summary	Filename of the fragment file.
	 */

	char*				fragmentFileName;

	// fullscreen image processing

	/**
	 * @summary	The screen render prog.
	 */

	GLuint				screenRenderProg;

	/**
	 * @summary	The screen quad.
	 */

	GLBatch             screenQuad;

	/**
	 * @summary	The screen textures.
	 */

	GLuint				screenTextures[1];

	/**
	 * @summary	The pix buffer objects.
	 */

	GLuint				pixBuffObjs[1];

	/**
	 * @summary	Size of the pixel data.
	 */

	GLuint				pixelDataSize;

	/**
	 * @summary	Identifier for the screen texture.
	 */

	GLuint				screenTextureID;

	/**
	 * @summary	The screen width location.
	 */

	GLuint				screenWidthLoc;

	/**
	 * @summary	The screen height location.
	 */

	GLuint				screenHeightLoc;
};

