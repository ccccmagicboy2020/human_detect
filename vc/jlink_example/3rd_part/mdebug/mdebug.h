#pragma once

//////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <conio.h>
#include <stdio.h>

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow();	//未公开
//////////////////////////////////////////////////////////////////////////

void debugInit();		//初始化调试库
void debugFree();		//关闭调试

//输出类函数
void mPuts( char *szFormat, ... );		//基础数据类型格式化输出
void mPutsEx( char *szFormat, ... );	//增强型数据类型格式化输出

//检测类函数
BOOL matchString( const char *szSrc, char **szDest );		//串匹配


//////////////////////////////////////////////////////////////////////////

int printPOINT( POINT );				//输出一个点的坐标信息, 输出格式 (x, y)
int printRECT( RECT );					//输出一个矩形的坐标信息, 输出格式 (left=left, top=top, right=right, bottom=bottom)
int printSIZE( SIZE );					//输出一个尺寸的坐标信息, 输出格式 [cx=cx, cy=cy]
int printRGBQUAD( RGBQUAD );			//输出一组 RGB 值, 输出格式, (r=r, g=g, b=b)

//////////////////////////////////////////////////////////////////////////
