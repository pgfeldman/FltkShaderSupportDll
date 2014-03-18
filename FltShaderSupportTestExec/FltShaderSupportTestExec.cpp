// FltShaderSupportTestExec.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GLTools.h>            // OpenGL toolkit
#include <FL/Fl.H>

#include "config.h"
#include "ShaderViewUI.h"

int _tmain(int argc, _TCHAR* argv[])
{
	gltSetWorkingDirectory((const char*)argv[0]);

	ShaderViewUI *svui = new ShaderViewUI;

	Fl::visual(FL_DOUBLE|FL_INDEX);
	svui->show(argc, (char**)argv);
	Fl::run();
	return 0;
}

