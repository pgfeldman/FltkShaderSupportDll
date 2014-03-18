#include "StdAfx.h"
#include "DrawableObject.h"

/**
 * @fn	DrawableObject::DrawableObject(GLuint activeTexture)
 *
 * @brief	Constructor. Sets up default positions and colors, and
 * 			creates a query object for picking
 *
 * @author	Phil
 * @date	3/14/2012
 *
 * @param	activeTexture	The active texture (i.e. GL_TEXTURE0, GL_TEXTURE1, etc).
 */

DrawableObject::DrawableObject(GLuint activeTexture)
{
	activeTextureID = activeTexture;
	drawQuery = 0; // if zero, there is no query

	setFloats( position, 3, 0.0, 0.0, 0.0);
	setFloats( orientation, 3, 0.0, 0.0, 0.0);
	scalar = 1.0f;


	setFloats( vRed, 4, 1.0f, 0.0f, 0.0f, 1.0f);
	setFloats( vGreen, 4, 0.0f, 1.0f, 0.0f, 1.0f);
	setFloats( vBlue, 4, 0.0f, 0.0f, 1.0f, 1.0f);
	setFloats( vWhite, 4, 1.0f, 1.0f, 1.0f, 1.0f);
	setFloats( vGray, 4, 0.5f, 0.5f, 0.5f, 1.0f);
	setFloats( vCyan, 4, 0.0f, 1.0f, 1.0f, 1.0f);
	setFloats( vMagenta, 4, 1.0f, 0.0f, 1.0f, 1.0f);
	setFloats( vYellow, 4, 1.0f, 1.0f, 0.0f, 1.0f);

	curTime = clock();
	prevTime = clock();
	deltaTime = 0.0f;

	pickQueryResult = READY;
	glGenQueries(1, &drawQuery);
	if(drawQuery == 0){
		printf("DrawableObject() glGenQueries ERROR: %s\n:", gluErrorString(glGetError()));
		pickQueryResult = PICK_ERROR;
	}
}


DrawableObject::~DrawableObject(void)
{
}

/**
 * @fn	void DrawableObject::setFloats(GLfloat *ptr, int size, ...)
 *
 * @brief	Sets values in an array of floats
 *
 * @author	Phil
 * @date	3/14/2012
 *
 * @param [in,out]	ptr	If non-null, the pointer.
 * @param	size	   	The size.
 */

void DrawableObject::setFloats(GLfloat *ptr, int size, ...)
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
 * @fn	void DrawableObject::placeInGlobalSpace()
 *
 * @brief	Place in global space. (Not currently used)
 *
 * @author	Phil
 * @date	3/14/2012
 */

void DrawableObject::placeInGlobalSpace(){
	/****
	glTranslatef(position[0], position[1], position[2]);
	glRotatef(orientation[2],0,1,0); 
	glRotatef(orientation[0],1,0,0);
	glScalef(scalar, scalar, scalar);
	/*****/
}

/**
 * @fn	void DrawableObject::orient33fromMat44(M3DMatrix33f &mat33, const M3DMatrix44f mat44)
 *
 * @brief	pull the orientation 3x3 matrix from a 4x4 matrix
 *
 * @author	Phil
 * @date	3/14/2012
 *
 * @param [in,out]	mat33	The 3x3 matrix
 * @param	mat44		 	The 4x4 matrix
 */

void DrawableObject::orient33fromMat44(M3DMatrix33f &mat33, const M3DMatrix44f mat44){
	mat33[0] = mat44[0];
	mat33[1] = mat44[1];
	mat33[2] = mat44[2];
	mat33[3] = mat44[4];
	mat33[4] = mat44[5];
	mat33[5] = mat44[6];
	mat33[6] = mat44[8];
	mat33[7] = mat44[9];
	mat33[8] = mat44[10];
}

/**
 * @fn	void DrawableObject::orient44fromMat44(M3DMatrix44f &targetMat,
 * 		const M3DMatrix44f sourceMat44)
 *
 * @brief	pull the orientation 4x4 matrix from a 4x4 matrix
 *
 * @author	Phil
 * @date	3/14/2012
 *
 * @param [in,out]	targetMat	Target 4x4 matrix.
 * @param	sourceMat44		 	Source 4x4 matrix.
 */
void DrawableObject::orient44fromMat44(M3DMatrix44f &targetMat, const M3DMatrix44f sourceMat44){
	targetMat[0] = sourceMat44[0];
	targetMat[1] = sourceMat44[1];
	targetMat[2] = sourceMat44[2];	
	targetMat[3] = sourceMat44[3];
	targetMat[4] = sourceMat44[4];
	targetMat[5] = sourceMat44[5];	
	targetMat[6] = sourceMat44[6];
	targetMat[7] = sourceMat44[7];
	targetMat[8] = sourceMat44[8];	
	targetMat[9] = sourceMat44[9];
	targetMat[10] = sourceMat44[10];
	targetMat[11] = sourceMat44[11];	
	targetMat[12] = 0.0f;
	targetMat[13] = 0.0f;	
	targetMat[14] = 0.0f;
	targetMat[15] = 1.0f;	
}

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
float DrawableObject::aabbSphereIntersect(const float Bmin[], const float Bmax[], const float C[], float r){

	float	dmin;
	float	r2 = SQR(r);
	int		dimensions = 3;

	dmin = 0;
	for( int i = 0; i < dimensions; i++ ) {
		if( C[i] < Bmin[i] ) 
			dmin += SQR( C[i] - Bmin[i] ); 
		else
		if( C[i] > Bmax[i] ) 
			dmin += SQR( C[i] - Bmax[i] );     
	}
	//Dprint::add("dmin = %f, r2 = %f dmin/r2 = %f", dmin, r2, dmin/r2);
	if( dmin <= r2 ){ 
		return 1.0f-(dmin/r2);
	}

	return 0.0f;
}

/**
 * @fn	bool DrawableObject::LoadTGATexture(const char *szFileName, GLenum minFilter,
 * 		GLenum magFilter, GLenum wrapMode)
 *
 * @brief	Load in a tga file as a texture. Allows specification of the filters and the wrap mode
 *
 * @author	Phil
 * @date	3/14/2012
 *
 * @param	szFileName	Filename of the texture file.
 * @param	minFilter 	e.g. GL_LINEAR_MIPMAP_LINEAR.
 * @param	magFilter 	e.g. GL_LINEAR.
 * @param	wrapMode  	The wrap mode (e.g. GL_REPEAT).
 *
 * @return	true if it succeeds, false if it fails.
 */
bool DrawableObject::LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	
	// Read the texture bits
	pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
	if(pBits == NULL) 
		return false;
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, nWidth, nHeight, 0,
	eFormat, GL_UNSIGNED_BYTE, pBits);
	
	free(pBits);

	if(minFilter == GL_LINEAR_MIPMAP_LINEAR || 
		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minFilter == GL_NEAREST_MIPMAP_NEAREST)
			glGenerateMipmap(GL_TEXTURE_2D);
			
	return true;
}


/**
 * @fn	bool DrawableObject::LoadBMPTexture(const char *szFileName, GLenum minFilter,
 * 		GLenum magFilter, GLenum wrapMode)
 *
 * @brief	Load in a BMP file as a texture. Allows specification of the filters and the wrap mode
 *
 * @author	Phil
 * @date	3/14/2012
 *
 * @param	szFileName	Filename of the texture file.
 * @param	minFilter 	e.g. GL_LINEAR_MIPMAP_LINEAR.
 * @param	magFilter 	e.g. GL_LINEAR.
 * @param	wrapMode  	The wrap mode (e.g. GL_REPEAT).
 *
 * @return	true if it succeeds, false if it fails.
 */
bool DrawableObject::LoadBMPTexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode){
		GLbyte *pBits;
	GLint iWidth, iHeight;

	pBits = gltReadBMPBits(szFileName, &iWidth, &iHeight);
	if(pBits == NULL)
		return false;

	// Set Wrap modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, pBits);

	// Do I need to generate mipmaps?
	if(minFilter == GL_LINEAR_MIPMAP_LINEAR || minFilter == GL_LINEAR_MIPMAP_NEAREST || minFilter == GL_NEAREST_MIPMAP_LINEAR || minFilter == GL_NEAREST_MIPMAP_NEAREST)
		glGenerateMipmap(GL_TEXTURE_2D);    

	return true;
}