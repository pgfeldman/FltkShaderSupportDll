#include "StdAfx.h"
#include "ScreenRepaint.h"

static const GLenum windowBuff[] = { GL_BACK_LEFT };
static const GLenum fboBuffs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

#define CALC_SCREEN_PIXELS screenWidth*screenHeight*3*1 // XXX This should be unsigned byte

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
ScreenRepaint::ScreenRepaint(GLuint activeTexture, const char* vertFileName, const char* fragFileName)
	: DrawableObject(activeTexture)
{
	screenTextureID = activeTexture; // this is done in the superclass, but for some reason we need to do it here, or we get a GL_ERROR

	vertexFileName = (char*)malloc(strlen(vertFileName)+2);
	strcpy(vertexFileName, vertFileName);

	fragmentFileName = (char*)malloc(strlen(fragFileName)+2);
	strcpy(fragmentFileName, fragFileName);

	fboInitialized = false;
}


ScreenRepaint::~ScreenRepaint(void)
{
}

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
void ScreenRepaint::resize(int width, int height){
	screenWidth = width;
	screenHeight = height;
	initFrameBufferObjects();
}

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
void ScreenRepaint::initFrameBufferObjects(){
	if(!fboInitialized){
		// Create screen post process program
		//screenRenderProg =  gltLoadShaderPairWithAttributes("/shaders/texpassthrough.vs", "/shaders/gaussianGlow.fs", 2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_TEXTURE0, "texCoord0");
		screenRenderProg =  gltLoadShaderPairWithAttributes(vertexFileName, fragmentFileName, 2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_TEXTURE0, "texCoord0");
		glBindFragDataLocation(screenRenderProg, 0, "oColor"); // bind the output of the shader to index zero - this is the same as using the default (depricated) gl_FragColor
		glLinkProgram(screenRenderProg); // relink, so that the graphics card knows what it's outputting to.

		screenWidthLoc = glGetUniformLocation(screenRenderProg, "screenWidth");
		screenHeightLoc = glGetUniformLocation(screenRenderProg, "screenHeight");

		// Create screen textures
		glActiveTexture(screenTextureID); // note that we are using GL_TEXTURE1, not the default
		glGenTextures(1, screenTextures);

		// Allocate a pixel buffer to initialize textures and PBOs
		pixelDataSize = CALC_SCREEN_PIXELS;  

		// Setup texture unit
		// Initialize texture data
		
		glBindTexture(GL_TEXTURE_2D, screenTextures[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		

		// Alloc space for copying pixels so we dont call malloc on every draw
		glGenBuffers(1, pixBuffObjs);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, pixBuffObjs[0]);
		glBufferData(GL_PIXEL_PACK_BUFFER, pixelDataSize, NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

		// Create geometry and a matrix for screen aligned drawing
		gltGenerateOrtho2DMat(screenWidth/2, screenHeight/2, orthoMatrix, screenQuad);

		// Make sure all went well
		gltCheckErrors();
		fboInitialized = true;
	}else{
		// reset screen aligned quad
		gltGenerateOrtho2DMat(screenWidth, screenHeight, orthoMatrix, screenQuad);

		GLuint val = CALC_SCREEN_PIXELS; 

		if(pixelDataSize < val){
			pixelDataSize = val;

			//  Resize PBOs
			glBindBuffer(GL_PIXEL_PACK_BUFFER, pixBuffObjs[0]);
			glBufferData(GL_PIXEL_PACK_BUFFER, pixelDataSize, NULL, GL_DYNAMIC_COPY);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		}

		gltCheckErrors();
	}
}

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
void ScreenRepaint::setupScreenRenderProg(const M3DMatrix44f mat)
{
	// Set the blur program as the current one
	glUseProgram(screenRenderProg);

	// Set MVP matrix
	glUniformMatrix4fv(glGetUniformLocation(screenRenderProg, "mvpMatrix"), 1, GL_FALSE, mat);

	// Setup the texture units for the target texture
	glUniform1i(glGetUniformLocation(screenRenderProg, "textureUnit0"), 1);

	// Pass in the screen size
	glUniform1i(screenWidthLoc, screenWidth);
	glUniform1i(screenHeightLoc, screenHeight);
}

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
void ScreenRepaint::render(GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack, GLShaderManager &shaderManager){
	// First bind the PBO as the pack buffer, then read the pixels directly to the PBO
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pixBuffObjs[0]); // bind
	glReadPixels(0, 0, screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE, NULL); // act
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0); // unbind


	// Next bind the PBO as the unpack buffer, then push the pixels straight into the texture
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixBuffObjs[0]); // bind
		
	// Setup texture unit for the render surface
	glActiveTexture(screenTextureID); // switch to the GL_TEXTURE* value that we are using as our screen texture holder
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); // write the pixels
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0); // unbind

	// Draw full screen quad with screen textures
	projectionStack.PushMatrix(); 
		projectionStack.LoadMatrix(orthoMatrix);
		modelViewStack.PushMatrix();	
			modelViewStack.LoadIdentity();
			glDisable(GL_DEPTH_TEST); 
			setupScreenRenderProg(projectionStack.GetMatrix());
			screenQuad.Draw();
			glEnable(GL_DEPTH_TEST); 
		modelViewStack.PopMatrix(); 
	projectionStack.PopMatrix();
}

/**
* @fn	void ScreenRepaint::environmentCalc();
*
* @brief	Environment calculation. In this case, do nothing
*
* @author	Phil
* @date	3/15/2012
*/
void ScreenRepaint::environmentCalc(){
}

/**
* @fn	void ScreenRepaint::localCleanup();
*
* @brief	Make sure default FBO is bound
* 			Cleanup textures
* 			Delete detached textures
* 			Delete the pixel buffer
* 			
*
* @author	Phil
* @date	3/15/2012
*/
void ScreenRepaint::localCleanup(){
	// Make sure default FBO is bound
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	// Cleanup textures
	glActiveTexture(screenTextureID);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Now delete detached textures
	glDeleteTextures(1, screenTextures);

	// delete PBO
	glDeleteBuffers(1, pixBuffObjs);
}

