#pragma once

//////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <conio.h>
#include <stdio.h>

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow();	//δ����
//////////////////////////////////////////////////////////////////////////

void debugInit();		//��ʼ�����Կ�
void debugFree();		//�رյ���

//����ຯ��
void mPuts( char *szFormat, ... );		//�����������͸�ʽ�����
void mPutsEx( char *szFormat, ... );	//��ǿ���������͸�ʽ�����

//����ຯ��
BOOL matchString( const char *szSrc, char **szDest );		//��ƥ��


//////////////////////////////////////////////////////////////////////////

int printPOINT( POINT );				//���һ�����������Ϣ, �����ʽ (x, y)
int printRECT( RECT );					//���һ�����ε�������Ϣ, �����ʽ (left=left, top=top, right=right, bottom=bottom)
int printSIZE( SIZE );					//���һ���ߴ��������Ϣ, �����ʽ [cx=cx, cy=cy]
int printRGBQUAD( RGBQUAD );			//���һ�� RGB ֵ, �����ʽ, (r=r, g=g, b=b)

//////////////////////////////////////////////////////////////////////////
