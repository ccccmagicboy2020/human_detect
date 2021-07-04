// XColorHexagonCtrlConstants.h  Version 1.1 - article available at www.codeproject.com
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// Description:
//     This file contains constants used by the CXColorHexagonCtrl class.
//
// License:
//     This software is released into the public domain.  You are free to use
//     it in any way you like, except that you may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this 
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XCOLORHEXAGONCTRLCONSTANTS_H
#define XCOLORHEXAGONCTRLCONSTANTS_H

static const COLORREF g_crCellColors[] = 
{
	RGB(  0,  51, 102),	// 1
	RGB( 51, 102, 153),
	RGB( 51, 102, 204),
	RGB(  0,  51, 153),
	RGB(  0,   0, 153),
	RGB(  0,   0, 204),
	RGB(  0,   0, 102),

	RGB(  0, 102, 102),	// 2
	RGB(  0, 102, 153),
	RGB(  0, 153, 204),
	RGB(  0, 102, 204),
	RGB(  0,  51, 204),
	RGB(  0,   0, 255),
	RGB( 51,  51, 255),
	RGB( 51,  51, 153),

	RGB(  0, 128, 128),	// 3
	RGB(  0, 153, 153),
	RGB( 51, 204, 204),
	RGB(  0, 204, 255),
	RGB(  0, 153, 255),
	RGB(  0, 102, 255),
	RGB( 51, 102, 255),
	RGB( 51,  51, 204),
	RGB(102, 102, 153),

	RGB( 51, 153, 102),	// 4
	RGB(  0, 204, 153),
	RGB(  0, 255, 204),
	RGB(  0, 255, 255),
	RGB( 51, 204, 255),
	RGB( 51, 153, 255),
	RGB(102, 153, 255),
	RGB(102, 102, 255),
	RGB(102,   0, 255),
	RGB(102,   0, 204),

	RGB( 51, 153,  51),	// 5
	RGB(  0, 204, 102),
	RGB(  0, 255, 153),
	RGB(102, 255, 204),
	RGB(102, 255, 255),
	RGB(102, 204, 255),
	RGB(153, 204, 255),
	RGB(153, 153, 255),
	RGB(153, 102, 255),
	RGB(153,  51, 255),
	RGB(153,   0, 255),

	RGB(  0, 102,   0),	// 6
	RGB(  0, 204,   0),
	RGB(  0, 255,   0),
	RGB(102, 255, 153),
	RGB(153, 255, 204),
	RGB(204, 255, 255),
	RGB(204, 236, 255),
	RGB(204, 204, 255),
	RGB(204, 153, 255),
	RGB(204, 102, 255),
	RGB(204,   0, 255),
	RGB(153,   0, 204),

	RGB(  0,  51,   0),	// 7
	RGB(  0, 128,   0),
	RGB( 51, 204,  51),
	RGB(102, 255, 102),
	RGB(153, 255, 153),
	RGB(204, 255, 204),
	RGB(255, 255, 255),
	RGB(255, 204, 255),
	RGB(255, 153, 255),
	RGB(255, 102, 255),
	RGB(255,   0, 255),
	RGB(204,   0, 204),
	RGB(102,   0, 102),

	RGB( 51, 102,   0),	// 8
	RGB(  0, 153,   0),
	RGB(102, 255,  51),
	RGB(153, 255, 102),
	RGB(204, 255, 153),
	RGB(255, 255, 204),
	RGB(255, 204, 204),
	RGB(255, 153, 204),
	RGB(255, 102, 204),
	RGB(255,  51, 204),
	RGB(204,   0, 153),
	RGB(128,   0, 128),

	RGB( 51,  51,   0),	// 9
	RGB(102, 153,   0),
	RGB(153, 255,  51),
	RGB(204, 255, 102),
	RGB(255, 255, 153),
	RGB(255, 204, 153),
	RGB(255, 153, 153),
	RGB(255, 102, 153),
	RGB(255,  51, 153),
	RGB(204,  51, 153),
	RGB(153,   0, 153),

	RGB(102, 102,  51),	// 10
	RGB(153, 204,   0),
	RGB(204, 255,  51),
	RGB(255, 255, 102),
	RGB(255, 204, 102),
	RGB(255, 153, 102),
	RGB(255, 124, 128),
	RGB(255,   0, 102),
	RGB(214,   0, 147),
	RGB(153,  51, 102),

	RGB(128, 128,   0),	// 11
	RGB(204, 204,   0),
	RGB(255, 255,   0),
	RGB(255, 204,   0),
	RGB(255, 153,  51),
	RGB(255, 102,   0),
	RGB(255,  80,  80),
	RGB(204,   0, 102),
	RGB(102,   0,  51),

	RGB(153, 102,  51),	// 12
	RGB(204, 153,   0),
	RGB(255, 153,   0),
	RGB(204, 102,   0),
	RGB(255,  51,   0),
	RGB(255,   0,   0),
	RGB(204,   0,   0),
	RGB(153,   0,  51),

	RGB(102,  51,   0),	// 13
	RGB(153, 102,   0),
	RGB(204,  51,   0),
	RGB(153,  51,   0),
	RGB(153,   0,   0),
	RGB(128,   0,   0),
	RGB(165,   0,  33)
};

static const int nCellColors = sizeof(g_crCellColors) / sizeof(g_crCellColors[0]);

	// gray scale
static const COLORREF g_crGrayScale[] = 
{
	RGB(248, 248, 248),	// 1
	RGB(221, 221, 221),
	RGB(178, 178, 178),
	RGB(128, 128, 128),
	RGB( 95,  95,  95),
	RGB( 51,  51,  51),
	RGB( 28,  28,  28),
	RGB(  8,   8,   8),

	RGB(234, 234, 234),	// 2
	RGB(192, 192, 192),
	RGB(150, 150, 150),
	RGB(119, 119, 119),
	RGB( 77,  77,  77),
	RGB( 41,  41,  41),
	RGB( 17,  17,  17)
};

static const int nGrayScale = sizeof(g_crGrayScale) / sizeof(g_crGrayScale[0]);

static const int nRowSize[]   = {  7,  8,  9, 10, 11, 12, 13, 12, 11, 10,  9,  8,  7, 0 };
static const int nRowOffset[] = { 42, 35, 28, 21, 14,  7,  0,  7, 14, 21, 28, 35, 42, 0 };
static const int nGrayScaleRowSize[]   = {  8,  7, 0 };
static const int nGrayScaleRowOffset[] = { 35, 42, 0 };

static const int XY_OFFSET         = 3;
static const int LARGE_CELL_WIDTH  = 28;
static const int LARGE_CELL_HEIGHT = 31;
static const int WHITE_CELL_X      = 0;

static const int SMALL_CELL_WIDTH  = 14;
static const int SMALL_CELL_HEIGHT = 15;
static const int GRAY_SCALE_STARTY = 0;

static const int SMALL_SELECTOR_WIDTH  = 19;
static const int SMALL_SELECTOR_HEIGHT = 19;
static const int LARGE_SELECTOR_WIDTH  = 33;
static const int LARGE_SELECTOR_HEIGHT = 33;

#endif //XCOLORHEXAGONCTRLCONSTANTS_H
