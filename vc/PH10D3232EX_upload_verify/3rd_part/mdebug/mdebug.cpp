//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include "mdebug.h"

//////////////////////////////////////////////////////////////////////////

void debugInit()
{
	AllocConsole();

	HWND cmd = GetConsoleWindow();

	DeleteMenu(GetSystemMenu(cmd, FALSE), SC_CLOSE, MF_BYCOMMAND);
	DeleteMenu(GetSystemMenu(cmd, FALSE), SC_MAXIMIZE, MF_BYCOMMAND);
	DeleteMenu(GetSystemMenu(cmd, FALSE), SC_MINIMIZE, MF_BYCOMMAND);
    DrawMenuBar(cmd);

	::system("title DEBUG输出");
	::system("mode con: cols=80 lines=300");

	//::SetWindowPos(cmd, HWND_TOPMOST, 0, 0, 700, 250, SWP_HIDEWINDOW);
	::SetWindowLong(cmd, GWL_STYLE, GetWindowLong(cmd, GWL_STYLE)&~WS_MINIMIZEBOX);
	::SetWindowLong(cmd, GWL_STYLE, GetWindowLong(cmd, GWL_STYLE)&~WS_MAXIMIZEBOX);
	::SetWindowLong(cmd, GWL_STYLE, GetWindowLong(cmd, GWL_STYLE)&~WS_SYSMENU);
}

void debugFree()
{
	FreeConsole();
}

//////////////////////////////////////////////////////////////////////////

//基础数据类型格式化输出
void mPuts( char *szFormat, ... )
{
	char szBuffer[10*1024];

	va_list pArgList;
	va_start( pArgList, szFormat );
	_vsnprintf( szBuffer, sizeof(szBuffer)/sizeof(char), szFormat, pArgList );
	va_end(pArgList);

	_cputs( szBuffer );
}

//增强数据类型输出
void mPutsEx( char *szFormat, ... )
{
	va_list pArgList;

	POINT		pt;
	RECT		rect;
	SIZE		size;
	RGBQUAD		rgb;

	HWND hConsole = (HWND)::GetStdHandle( STD_OUTPUT_HANDLE );
	::SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );

	va_start( pArgList, szFormat );
	
	while( *szFormat )
	{
		switch( *szFormat )
		{
		case '%':
			{
				szFormat++;
				//////////////////////////////////////////////////////////////////////////

				//增强型数据类型输出
				if( matchString( "pt", &szFormat ) )			// %pt == POINT Structure
				{
					pt = va_arg( pArgList, POINT );
					printPOINT( pt );
				}
				
				//////////////////////////////////////////////////////////////////////////

				if( matchString( "rect", &szFormat ) )			// %rect == RECT Structure
				{
					rect = va_arg( pArgList, RECT );
					printRECT( rect );
				}
				
				//////////////////////////////////////////////////////////////////////////

				if( matchString( "size", &szFormat ) )			// %size == SIZE Structure
				{
					size = va_arg( pArgList, SIZE );
					printSIZE( size );
				}

				//////////////////////////////////////////////////////////////////////////

				if( matchString( "rgb", &szFormat ) )			// %rgb == RGBQUAD Structure
				{
					rgb = va_arg( pArgList, RGBQUAD );
					printRGBQUAD( rgb );
				}

				//////////////////////////////////////////////////////////////////////////
				if( matchString( "endfr", &szFormat ) )			// %endfr == 结束前景颜色格式化输出
					SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
				
				if( matchString( "endbk", &szFormat ) )			// %endbk == 结束背景颜色格式化输出
				{
					SetConsoleTextAttribute( hConsole, 0 );
					SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
				}
				//--------------------色彩格式化输出---------------------
				//前景颜色
				if( matchString( "frred", &szFormat ) )			// %frred == 前景颜色, 红色
					SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY );

				if( matchString( "frgreen", &szFormat ) )		// %frgreen == 前景颜色, 绿色
					SetConsoleTextAttribute( hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY );

				if( matchString( "frblue", &szFormat ) )		// %frblue == 前景颜色, 蓝色
					SetConsoleTextAttribute( hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY );

				if( matchString( "fryellow", &szFormat ) )		// %fryellow == 前景颜色, 黄色
					SetConsoleTextAttribute( hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY );

				if( matchString( "frcyan", &szFormat ) )		// %frfrcyan == 前景颜色, 青色
					SetConsoleTextAttribute( hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
				
				if( matchString( "frmag", &szFormat ) )			// %frmag == 前景颜色, 品红
					SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY );

				//背景颜色
				if( matchString( "bkred", &szFormat ) )			// %bkred == 背景颜色, 红色
					SetConsoleTextAttribute( hConsole, BACKGROUND_RED );

				if( matchString( "bkgreen", &szFormat ) )		// %bkgreen == 背景颜色, 绿色
					SetConsoleTextAttribute( hConsole, BACKGROUND_GREEN );

				if( matchString( "bkblue", &szFormat ) )		// %bkblue == 背景颜色, 蓝色
					SetConsoleTextAttribute( hConsole, BACKGROUND_BLUE );

				if( matchString( "bkyellow", &szFormat ) )		// %bkyellow == 背景颜色, 黄色
					SetConsoleTextAttribute( hConsole, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY );

				if( matchString( "bkcyan", &szFormat ) )		// %bkfrcyan == 背景颜色, 青色
					SetConsoleTextAttribute( hConsole, BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY );
				
				if( matchString( "bkmag", &szFormat ) )			// %bkmag == 背景颜色, 品红
					SetConsoleTextAttribute( hConsole, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY );
			}
			break;

		case '\n':					//换行 并结束当前前\背景色输出
			SetConsoleTextAttribute( hConsole, 0 );
			SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
			_cputs( "\n" );
			szFormat++;
			break;

		case '\a':					//发出 beep 音
			MessageBeep(0);
			szFormat++;
			break;

		default:
			_cprintf( "%c", (char)*szFormat++ );
		}
	}
	va_end( pArgList );
}

//串匹配
BOOL matchString( const char *szSrc, char **szDest )
{
	char *szTmp = *szDest;

	while( *szSrc )
	{
		if( *szSrc != **szDest  )
		{
			*szDest = szTmp;
			return FALSE;
		}	

		szSrc++;
		(*szDest)++;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

//输出点
int printPOINT( POINT pt )
{
	_cprintf( "(x=%d, y=%d)", pt.x, pt.y );		//输出格式

	return 0;
}

//输出矩形
int printRECT( RECT rect )
{
	_cprintf( "(left=%d, top=%d, right=%d, bottom=%d)", rect.left, rect.top, rect.right, rect.bottom );

	return 0;
}

//输出尺寸
int printSIZE( SIZE size )
{
	_cprintf( "[cx=%d, cy=%d]", size.cx, size.cy );

	return 0;
}

//输出一组RGB值
int printRGBQUAD( RGBQUAD rgb )
{
	_cprintf( "(blue=%d, green=%d, red=%d, reserved=%d)", rgb.rgbBlue, rgb.rgbGreen, rgb.rgbRed, rgb.rgbReserved );

	return 0;
}

//////////////////////////////////////////////////////////////////////////
