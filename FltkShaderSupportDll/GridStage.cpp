#include "StdAfx.h"
#include "GridStage.h"

GLBatch	_xyBatch;
GLBatch	_xzBatch;
GLBatch	_yzBatch;

/**
* @fn	GridStage::GridStage(float size, int divisions);
*
* @brief	Constructor.
*
* @author	Phil
* @date	3/15/2012
*
* @param	size	 	the size of each 'grid plane'from the origin. Since planes extend equally from the origin, this is actually 1/2 the size
* @param	divisions	The number of lines in each 'grid plane'.
*/
GridStage::GridStage(float size, int divisions):DrawableObject(GL_TEXTURE0){
	init(size, divisions);
}

GridStage::GridStage():DrawableObject(GL_TEXTURE0){
}

GridStage::~GridStage(void)
{
}

void GridStage::init(float size, int divisions){
	_size = size;
	_divisions = divisions;
	setup();
}
/**
* @fn	void GridStage::setup();
*
* @brief	Creates the geometry for this object
*
* @author	Phil
* @date	3/15/2012
*/
void GridStage::setup(){
		float step = _size/(float)_divisions;
		float pos;
		
		// XY plane (green)
		//glMaterial(GL_FRONT, GL_AMBIENT, lm.getGreenMaterial());
		//glColor4f(0.1f, 1.0f, 0.1f, 1.0f);
		_xyBatch.Begin(GL_LINES, (_divisions*2+1)*4);
			pos = -_size;
			while(pos <= (_size+step*0.5)){
				_xyBatch.Vertex3f(pos, -_size, 0.0f);
				_xyBatch.Vertex3f(pos, _size, 0.0f);
				_xyBatch.Vertex3f(-_size, pos, 0.0f);
				_xyBatch.Vertex3f(_size, pos, 0.0f);
				pos += step;
			}
		_xyBatch.End();	
		
		// XZ plane (red)
		//glColor4f(1.0f, 0.1f, 0.1f, 1.0f);
		_xzBatch.Begin(GL_LINES, (_divisions*2+1)*4);
			pos = -_size;
			while(pos <= (_size+step*0.5f)){
				_xzBatch.Vertex3f(pos, 0.0f, -_size);
				_xzBatch.Vertex3f(pos, 0.0f, _size);
				_xzBatch.Vertex3f(-_size, 0.0f, pos);
				_xzBatch.Vertex3f(_size, 0.0f, pos);
				pos += step;
			}
		_xzBatch.End();
		
		// YZ plane (blue)
		//glColor4f(0.1f, 0.1f, 1.0f, 1.0f);
		_yzBatch.Begin(GL_LINES, (_divisions*2+1)*4);
		pos = -_size;
		while(pos <= (_size+step*0.5)){
			_yzBatch.Vertex3f(0.0, pos, -_size);
			_yzBatch.Vertex3f(0.0, pos, _size);
			_yzBatch.Vertex3f(0.0, -_size, pos);
			_yzBatch.Vertex3f(0.0, _size, pos);
			pos += step;
		}
		_yzBatch.End();
}

/**
* @fn	void GridStage::render(GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack,
* 		GLShaderManager &shaderManager);
*
* @brief	Draws this object.
*
* @author	Phil
* @date	3/15/2012
*
* @param [in,out]	modelViewStack 	model view matrix stack.
* @param [in,out]	projectionStack	projections projection stack
* @param [in,out]	shaderManager  	collection of default shaders
*/
void GridStage::render(GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack, GLShaderManager &shaderManager){

	projectionStack.PushMatrix();
		projectionStack.MultMatrix(modelViewStack.GetMatrix());

		shaderManager.UseStockShader(GLT_SHADER_FLAT, projectionStack.GetMatrix(),	 vGreen);	
		_xyBatch.Draw();

		shaderManager.UseStockShader(GLT_SHADER_FLAT, projectionStack.GetMatrix(),	 vRed);	
		_xzBatch.Draw();

		shaderManager.UseStockShader(GLT_SHADER_FLAT, projectionStack.GetMatrix(),	 vBlue);	
		_yzBatch.Draw();
	projectionStack.PopMatrix();
}