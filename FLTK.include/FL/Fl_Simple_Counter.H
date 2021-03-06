//
// "$Id: Fl_Simple_Counter.H 9127 2011-10-04 16:42:53Z greg.ercolano $"
//
// Simple counter header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     http://www.fltk.org/COPYING.php
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

/* \file
   Fl_Simple_Counter widget . */

#ifndef Fl_Simple_Counter_H
#define Fl_Simple_Counter_H

#include "Fl_Counter.H"
/** 
  This widget creates a counter with only 2 arrow buttons 
  <P align=center>\image html counter.png</P> 
  \image latex  counter.png "Fl_Simple_Counter" width=4cm
*/
class FL_EXPORT Fl_Simple_Counter : public Fl_Counter {
public:
    Fl_Simple_Counter(int X,int Y,int W,int H, const char *L = 0)
	: Fl_Counter(X,Y,W,H,L) {type(FL_SIMPLE_COUNTER);}
};

#endif

//
// End of "$Id: Fl_Simple_Counter.H 9127 2011-10-04 16:42:53Z greg.ercolano $".
//
