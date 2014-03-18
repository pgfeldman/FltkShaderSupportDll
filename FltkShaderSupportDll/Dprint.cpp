#include "StdAfx.h"
#include "Dprint.h"

vector<string> Dprint::stringVec;    

Dprint::Dprint(void)
{
}


Dprint::~Dprint(void)
{
}

/**
 * @fn	void Dprint::screenPrint(int width, int height)
 *
 * @brief	Print all the stored strings to the screen
 *
 * @author	Phil
 * @date	3/14/2012
 *
 * @param	width 	The width of the screen in pixels.
 * @param	height	The height of the screen in pixels.
 */

void Dprint::screenPrint(int width, int height){
	char msg[512];

	gl_font(FL_HELVETICA, 12);
	int fontHeight = glutBitmapHeight(GLUT_BITMAP_HELVETICA_12);
	int yPos = height-fontHeight;
	for(unsigned int i = 0; i < stringVec.size(); ++i){
		sprintf_s(msg, "[%d] %s\n", i, stringVec[i].c_str());
		gl_draw(msg, 10, yPos);
		yPos -= fontHeight;
	}
}

/**
 * @fn	void Dprint::reset()
 *
 * @brief	Clears all strings out of storage. Should be called immediately after rendering (i.e. screenPrint)
 *
 * @author	Phil
 * @date	3/14/2012
 */

void Dprint::reset(){
	stringVec.clear();
}

/**
 * @fn	void Dprint::add(const char * format, ...)
 *
 * @brief	Adds a string using pritf formatting
 *
 * @author	Phil
 * @date	3/14/2012
 *
 * @param	format	The string containing formatting info
 * @param	...		The values referenced in the format string
 */
void Dprint::add(const char * format, ...){
	char Data[4096];
	va_list v;
	va_start(v,format);
	_vsnprintf_s(Data, sizeof(Data), format,v);
	va_end(v);

	string str(Data);
	stringVec.push_back(str);
}

/**
 * @fn	void Dprint::addMatrix44(const char *str, const float farray[16])
 *
 * @brief	A convience method for printing out 4 x 4 matrices
 *
 * @author	Phil
 * @date	3/14/2012
 *
 * @param	str   	A string describing or naming the array
 * @param	farray	The farray.
 */
void Dprint::addMatrix44(const char *str, const float farray[16]){

	char buf[512];
	int index = 0;
	for(int col = 0; col < 4; ++col){
		sprintf_s(buf, "%s", str);
		for(int row = 0; row < 4; ++row){
			sprintf_s(buf, "%s[%d]%.2f\t", buf, index, farray[index++]);
		}
		add("%s", buf);
	}
}

/**
 * @fn	void Dprint::addMatrix33(const char *str, const float farray[16])
 *
 * @brief	A convience method for printing out 3 x 3 matrices
 *
 * @author	Phil
 * @date	3/14/2012
 *
 * @param	str   	A string describing or naming the array
 * @param	farray	The farray.
 */
void Dprint::addMatrix33(const char *str, const float farray[16]){

	char buf[512];
	int index = 0;
	for(int col = 0; col < 3; ++col){
		sprintf_s(buf, "%s", str);
		for(int row = 0; row < 3; ++row){
			sprintf_s(buf, "%s[%d]%.2f\t", buf, index, farray[index++]);
		}
		add("%s", buf);
	}
}