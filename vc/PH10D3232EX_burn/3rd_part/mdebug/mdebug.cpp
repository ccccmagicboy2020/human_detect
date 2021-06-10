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

	::system("title DEBUG���");
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

//�����������͸�ʽ�����
void mPuts( char *szFormat, ... )
{
	char szBuffer[10*1024];

	va_list pArgList;
	va_start( pArgList, szFormat );
	_vsnprintf( szBuffer, sizeof(szBuffer)/sizeof(char), szFormat, pArgList );
	va_end(pArgList);

	_cputs( szBuffer );
}

//��ǿ�����������
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

				//��ǿ�������������
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
				if( matchString( "endfr", &szFormat ) )			// %endfr == ����ǰ����ɫ��ʽ�����
					SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
				
				if( matchString( "endbk", &szFormat ) )			// %endbk == ����������ɫ��ʽ�����
				{
					SetConsoleTextAttribute( hConsole, 0 );
					SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
				}
				//--------------------ɫ�ʸ�ʽ�����---------------------
				//ǰ����ɫ
				if( matchString( "frred", &szFormat ) )			// %frred == ǰ����ɫ, ��ɫ
					SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY );

				if( matchString( "frgreen", &szFormat ) )		// %frgreen == ǰ����ɫ, ��ɫ
					SetConsoleTextAttribute( hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY );

				if( matchString( "frblue", &szFormat ) )		// %frblue == ǰ����ɫ, ��ɫ
					SetConsoleTextAttribute( hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY );

				if( matchString( "fryellow", &szFormat ) )		// %fryellow == ǰ����ɫ, ��ɫ
					SetConsoleTextAttribute( hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY );

				if( matchString( "frcyan", &szFormat ) )		// %frfrcyan == ǰ����ɫ, ��ɫ
					SetConsoleTextAttribute( hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
				
				if( matchString( "frmag", &szFormat ) )			// %frmag == ǰ����ɫ, Ʒ��
					SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY );

				//������ɫ
				if( matchString( "bkred", &szFormat ) )			// %bkred == ������ɫ, ��ɫ
					SetConsoleTextAttribute( hConsole, BACKGROUND_RED );

				if( matchString( "bkgreen", &szFormat ) )		// %bkgreen == ������ɫ, ��ɫ
					SetConsoleTextAttribute( hConsole, BACKGROUND_GREEN );

				if( matchString( "bkblue", &szFormat ) )		// %bkblue == ������ɫ, ��ɫ
					SetConsoleTextAttribute( hConsole, BACKGROUND_BLUE );

				if( matchString( "bkyellow", &szFormat ) )		// %bkyellow == ������ɫ, ��ɫ
					SetConsoleTextAttribute( hConsole, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY );

				if( matchString( "bkcyan", &szFormat ) )		// %bkfrcyan == ������ɫ, ��ɫ
					SetConsoleTextAttribute( hConsole, BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY );
				
				if( matchString( "bkmag", &szFormat ) )			// %bkmag == ������ɫ, Ʒ��
					SetConsoleTextAttribute( hConsole, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY );
			}
			break;

		case '\n':					//���� ��������ǰǰ\����ɫ���
			SetConsoleTextAttribute( hConsole, 0 );
			SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
			_cputs( "\n" );
			szFormat++;
			break;

		case '\a':					//���� beep ��
			MessageBeep(0);
			szFormat++;
			break;

		default:
			_cprintf( "%c", (char)*szFormat++ );
		}
	}
	va_end( pArgList );
}

//��ƥ��
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

//�����
int printPOINT( POINT pt )
{
	_cprintf( "(x=%d, y=%d)", pt.x, pt.y );		//�����ʽ

	return 0;
}

//�������
int printRECT( RECT rect )
{
	_cprintf( "(left=%d, top=%d, right=%d, bottom=%d)", rect.left, rect.top, rect.right, rect.bottom );

	return 0;
}

//����ߴ�
int printSIZE( SIZE size )
{
	_cprintf( "[cx=%d, cy=%d]", size.cx, size.cy );

	return 0;
}

//���һ��RGBֵ
int printRGBQUAD( RGBQUAD rgb )
{
	_cprintf( "(blue=%d, green=%d, red=%d, reserved=%d)", rgb.rgbBlue, rgb.rgbGreen, rgb.rgbRed, rgb.rgbReserved );

	return 0;
}

//////////////////////////////////////////////////////////////////////////
