#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <string>

#include <GL/glut.h>
#include <GL/GLU.h>

using namespace std;

/**
 * @class	Dprint
 *
 * @brief	Adds printf-like functionality to on OpenGL display. 
 *
 * @author	Phil
 * @date	3/14/2012
 */

class Dprint
{
public:

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
	static void add(const char * format, ...);

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
	static void screenPrint(int width, int height);

	/**
	 * @fn	void Dprint::reset()
	 *
	 * @brief	Clears all strings out of storage. Should be called immediately after rendering (i.e. screenPrint)
	 *
	 * @author	Phil
	 * @date	3/14/2012
	 */
	static void reset();

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
	static void addMatrix44(const char *str, const float farray[16]);

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
	static void addMatrix33(const char *str, const float farray[9]);

protected:
	Dprint(void);
	Dprint(const Dprint&);
	Dprint& operator= (const Dprint&);
	~Dprint(void);

private:
	static vector<string> stringVec;    
};

