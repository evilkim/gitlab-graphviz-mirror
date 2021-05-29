/**
 * \brief remove overlaps between a set of rectangles.
 *
 * Authors:
 *   Tim Dwyer <tgdwyer@gmail.com>
 *
 * Copyright (C) 2005 Authors
 *
 * This version is released under the CPL (Common Public License) with
 * the Graphviz distribution.
 * A version is also available under the LGPL as part of the Adaptagrams
 * project: https://github.com/mjwybrow/adaptagrams.  
 * If you make improvements or bug fixes to this code it would be much
 * appreciated if you could also contribute those changes back to the
 * Adaptagrams repository.
 */

#include <iostream>
#include <cassert>
#include <vpsc/generate-constraints.h>
#include <vpsc/solve_VPSC.h>
#include <vpsc/variable.h>
#include <vpsc/constraint.h>
#include <fstream>
#include <vpsc/blocks.h>
#include <vector>
using std::ios;
using std::ofstream;

#ifndef RECTANGLE_OVERLAP_LOGGING
	#define RECTANGLE_OVERLAP_LOGGING 0
#endif

#define EXTRA_GAP 0.0001

double Rectangle::xBorder=0;
double Rectangle::yBorder=0;
