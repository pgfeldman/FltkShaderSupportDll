#pragma once

#include "DrawableObject.h"

/**
 * @class	GridStage
 *
 * @brief	DrawableObject that renders a color-coded set of 3 orthoganal grids with a user-specified size and density
 *
 * @author	Phil
 * @date	3/15/2012
 */

class GridStage :
	public DrawableObject
{
public:

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
	GridStage(float size, int divisions);
	GridStage();

	/**
	 * @fn	GridStage::~GridStage(void);
	 *
	 * @brief	Destructor.
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	~GridStage(void);

	void init(float size, int divisions);
	/**
	 * @fn	void GridStage::setup();
	 *
	 * @brief	Creates the geometry for this object
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	void setup();

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
	void render(GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack, GLShaderManager &shaderManager);

	/**
	 * @fn	void GridStage::environmentCalc()
	 *
	 * @brief	Environment calculations. In this case, do nothing
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	void environmentCalc(){};

	/**
	 * @fn	void GridStage::localCleanup()
	 *
	 * @brief	Local cleanup. In this case, do nothing
	 *
	 * @author	Phil
	 * @date	3/15/2012
	 */
	void localCleanup(){};

private:

	/**
	 * @summary	1/2 the total size for each plane
	 */
	float	_size;

	/**
	 * @summary	The number of divisions for each plane
	 */
	int	_divisions;

	/**
	 * @summary	The geometry for the XY plane. (from: http://code.google.com/p/oglsuperbible5/source/browse/trunk/Src/GLTools/include/GLBatch.h)
	 */
	GLBatch	_xyBatch;

	/**
	 * @summary	The geometry for the XZ plane. (from: http://code.google.com/p/oglsuperbible5/source/browse/trunk/Src/GLTools/include/GLBatch.h)
	 */
	GLBatch	_xzBatch;

	/**
	 * @summary	The geometry for the YZ plane. (from: http://code.google.com/p/oglsuperbible5/source/browse/trunk/Src/GLTools/include/GLBatch.h)
	 */
	GLBatch	_yzBatch;
};

