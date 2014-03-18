#include "StdAfx.h"
#include "CollisionCube.h"





CollisionCube::CollisionCube(GLuint activeTexture, float xsize, float ysize, float zsize): CollisionCubeBase(activeTexture, xsize, ysize, zsize)
{
	setup();
}


CollisionCube::~CollisionCube(void)
{
}

void CollisionCube::setup(){
	GLbyte *pBytes;
    GLint iWidth, iHeight, iComponents;
    GLenum eFormat;
    int i;

	// Six sides of a cube map
	const char *szCubeFaces[6] = { 
						"/textures/gradient_cube_side_128x128.tga", 
						"/textures/gradient_cube_side_128x128.tga", 
						"/textures/gradient_cube_top_128x128.tga", 
						"/textures/gradient_cube_bottom_128x128.tga", 
						"/textures/gradient_cube_side_128x128.tga", 
						"/textures/gradient_cube_side_128x128.tga" };

	GLenum  cube[6] = {  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
						 GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
						 GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
						 GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
						 GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
						 GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

    // Load the cube map    
    glGenTextures(1, &cubeTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
        
    // Set up texture maps        
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);       
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
  
    // Load Cube Map images
    for(i = 0; i < 6; i++)
        {        
        // Load this texture map
        pBytes = gltReadTGABits(szCubeFaces[i], &iWidth, &iHeight, &iComponents, &eFormat);
        glTexImage2D(cube[i], 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
        free(pBytes);
        }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    
    reflectionShader = gltLoadShaderPairWithAttributes("/shaders/coloredReflection.vp", "/shaders/coloredReflection.fp", 3, 
                                                GLT_ATTRIBUTE_VERTEX, "vVertex",
                                                GLT_ATTRIBUTE_NORMAL, "vNormal",
												GLT_ATTRIBUTE_TEXTURE0, "vTexCoords");
                                                
    locMVPReflect = glGetUniformLocation(reflectionShader, "mvpMatrix");
    locMVReflect = glGetUniformLocation(reflectionShader, "mvMatrix");
    locNormalReflect = glGetUniformLocation(reflectionShader, "normalMatrix");
	locInvertedCamera = glGetUniformLocation(reflectionShader, "mInverseCamera");
	locCubeMap = glGetUniformLocation(reflectionShader, "cubeMap");
	locMaterialColor = glGetUniformLocation(reflectionShader, "materialColor");
                                                
                                                
    skyBoxShader = gltLoadShaderPairWithAttributes("/shaders/SkyBox.vp", "/shaders/SkyBox.fp", 2, 
                                                GLT_ATTRIBUTE_VERTEX, "vVertex",
                                                GLT_ATTRIBUTE_NORMAL, "vNormal");

	locMVPSkyBox = glGetUniformLocation(skyBoxShader, "mvpMatrix");

	// Set textures to their texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);

	gltMakeCube(cubeBatch, 0.5f);
	timeScalar = .5f;
	angle = 0.0f;
}


void CollisionCube::drawPrimitive(GLBatch &batch, const GLfloat *vColor, const M3DMatrix44f &mCamera, GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack){
	M3DMatrix44f mCameraRotOnly;
	M3DMatrix44f mInverseCamera;
	M3DMatrix33f mNormalMat;

	orient44fromMat44(mCameraRotOnly, mCamera);
	orient33fromMat44(mNormalMat, modelViewStack.GetMatrix());
	m3dInvertMatrix44(mInverseCamera, mCameraRotOnly);

	projectionStack.PushMatrix();
		projectionStack.MultMatrix(modelViewStack.GetMatrix());
		glUseProgram(reflectionShader);
		glUniformMatrix4fv(locMVPReflect, 1, GL_FALSE, projectionStack.GetMatrix());
		glUniformMatrix4fv(locMVReflect, 1, GL_FALSE, modelViewStack.GetMatrix());
		glUniformMatrix3fv(locNormalReflect, 1, GL_FALSE, mNormalMat);
		glUniformMatrix4fv(locInvertedCamera, 1, GL_FALSE, mInverseCamera);
		glUniform1i(locCubeMap, 0);
		glUniform4fv(locMaterialColor, 1, vColor);
		batch.Draw();
		glUseProgram(NULL);
	projectionStack.PopMatrix();
}

void CollisionCube::render(GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack, GLShaderManager &shaderManager){
	
	Dprint::add("CollisionCube position = (%.2f, %.2f, %.2f)", position[0], position[1], position[2]);
	Dprint::add("CollisionCube orientation = (%.2f, %.2f, %.2f)", orientation[0], orientation[1], orientation[2]);
	const M3DMatrix44f &mCamera = modelViewStack.GetMatrix(); // need this for reflection vectors to work

	modelViewStack.PushMatrix();
		//modelViewStack.Translate(position[0] + size[0]*0.5f, position[1] + size[1]*0.5f, position[2] + size[2]*0.5f);
		modelViewStack.Translate(position[0], position[1], position[2]);
		modelViewStack.Rotate(orientation[1], 0.0f, 1.0f, 0.0f);
		modelViewStack.Rotate(orientation[2], 1.0f, 0.0f, 0.0f);
		modelViewStack.Scale(size[0], size[1], size[2]);
		drawPrimitive(cubeBatch, vGray, mCamera, modelViewStack, projectionStack);
	modelViewStack.PopMatrix();
}

void CollisionCube::localCleanup(){
	glDeleteTextures(1, &cubeTexture);
}