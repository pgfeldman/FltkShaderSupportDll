#pragma once
#include "collisioncubebase.h"

class TexturedCollisionCube :
	public CollisionCubeBase
{
public:
	enum MATERIAL_TYPE{NONE_SELECTED, GLASS, PLASTIC, CEMENT, RUBBER, FOAM, FUR, COTTON, OIL, WATER};

	TexturedCollisionCube(GLuint activeTexture, float xsize, float ysize, float zsize, char* texFileName);
	~TexturedCollisionCube(void);

	void setup(char* texFileName);
	void environmentCalc();
	void render(GLMatrixStack &modelViewStack, GLMatrixStack &projectionStack, GLShaderManager &shaderManager);
	void localCleanup();

	static const char* materialTypeString(const MATERIAL_TYPE& mt){
		// NONE_SELECTED, GLASS, PLASTIC, CEMENT, RUBBER, FOAM, FUR, COTTON, OIL, WATER
		if(mt == NONE_SELECTED)
			return "NONE_SELECTED";
		else if(mt == GLASS)
			return "GLASS";
		else if(mt == PLASTIC)
			return "PLASTIC";
		else if(mt == CEMENT)
			return "CEMENT";
		else if(mt == RUBBER)
			return "RUBBER";
		else if(mt == FOAM)
			return "FOAM";
		else if(mt == FUR)
			return "FUR";
		else if(mt == COTTON)
			return "COTTON";
		else if(mt == OIL)
			return "OIL";
		else if(mt == WATER)
			return "WATER";
		else
			return "UNKNOWN";
	}

	static int materialTypeInt(const MATERIAL_TYPE& mt){
		// NONE_SELECTED, GLASS, PLASTIC, CEMENT, RUBBER, FOAM, FUR, COTTON, OIL, WATER
		if(mt == NONE_SELECTED)
			return 0;
		else if(mt == GLASS)
			return 1;
		else if(mt == PLASTIC)
			return 2;
		else if(mt == CEMENT)
			return 3;
		else if(mt == RUBBER)
			return 4;
		else if(mt == FOAM)
			return 5;
		else if(mt == FUR)
			return 6;
		else if(mt == COTTON)
			return 7;
		else if(mt == OIL)
			return 8;
		else if(mt == WATER)
			return 9;
		else
			return 99;
	}

	// Access the MaterialType
	const MATERIAL_TYPE& getMaterialType(void) const			{ return(materialType);			};
	void setMaterialType(const MATERIAL_TYPE& _materialType)	{ materialType = _materialType;	};


private:
	void calcCorners();

	MATERIAL_TYPE materialType;
	GLuint	textureId;
	GLuint	litTexShaderId;
	float cornerPos[8][3];
	GLMatrixStack modelMat;
};

