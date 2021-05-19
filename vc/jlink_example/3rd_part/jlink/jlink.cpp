#include "jlink.h"
#include "StdAfx.h"

void  (WINAPI *JLINK_SetLogFile)(char *file);
DWORD (WINAPI *JLINK_GetDLLVersion)(void);
DWORD (WINAPI *JLINK_GetHardwareVersion)(void);
DWORD (WINAPI *JLINK_GetFirmwareString)(char *buff, int count);
DWORD (WINAPI *JLINK_GetSN)(void);
 
BOOL  (WINAPI *JLINK_ExecCommand)(char* cmd, int a, int b);
DWORD (WINAPI *JLINK_TIF_Select)(int type);
void  (WINAPI *JLINK_SetSpeed)(int speed);
DWORD (WINAPI *JLINK_GetSpeed)(void);
DWORD (WINAPI *JLINK_GetId)(void);
DWORD (WINAPI *JLINK_GetDeviceFamily)(void);
 
BOOL  (WINAPI *JLINK_Open)(void);
void  (WINAPI *JLINK_Close)(void);
BOOL  (WINAPI *JLINK_IsOpen)(void);
 
BOOL  (WINAPI *JLINK_Connect)(void);
BOOL  (WINAPI *JLINK_IsConnected)(void);
int   (WINAPI *JLINK_Halt)(void);
BOOL  (WINAPI *JLINK_IsHalted)(void);
void  (WINAPI *JLINK_SetResetType)(int type);
void  (WINAPI *JLINK_Reset)(void);
void  (WINAPI *JLINK_Go)(void);
void  (WINAPI *JLINK_GoIntDis)(void);
DWORD (WINAPI *JLINK_ReadReg)(int index);
int   (WINAPI *JLINK_WriteReg)(int index, DWORD data);
 
int   (WINAPI *JLINK_ReadMem)(DWORD addr, int len, void *buf);
int   (WINAPI *JLINK_WriteMem)(DWORD addr, int len, void *buf);
int   (WINAPI *JLINK_WriteU8)(DWORD addr, BYTE data);
int   (WINAPI *JLINK_WriteU16)(DWORD addr, WORD data);
int   (WINAPI *JLINK_WriteU32)(DWORD addr, DWORD data);
 
int   (WINAPI *JLINK_EraseChip)(void);
int   (WINAPI *JLINK_DownloadFile)(LPCSTR file, DWORD addr);
void  (WINAPI *JLINK_BeginDownload)(int index);
void  (WINAPI *JLINK_EndDownload)(void);

void jlink_dll_import(HMODULE hModule)
{
	JLINK_GetDLLVersion      = (void*)GetProcAddress(hModule, "JLINK_GetDLLVersion");
	JLINK_GetHardwareVersion = (void*)GetProcAddress(hModule, "JLINK_GetHardwareVersion");
	JLINK_GetFirmwareString  = (void*)GetProcAddress(hModule, "JLINK_GetFirmwareString");
	JLINK_GetSN              = (void*)GetProcAddress(hModule, "JLINK_GetSN");
	JLINK_GetId              = (void*)GetProcAddress(hModule, "JLINK_GetId");
	JLINK_GetDeviceFamily    = (void*)GetProcAddress(hModule, "JLINK_GetDeviceFamily");

	JLINK_SetLogFile         = (void*)GetProcAddress(hModule, "JLINK_SetLogFile");
	JLINK_ExecCommand        = (void*)GetProcAddress(hModule, "JLINK_ExecCommand");

	JLINK_GetSpeed           = (void*)GetProcAddress(hModule, "JLINK_GetSpeed");
	JLINK_SetSpeed           = (void*)GetProcAddress(hModule, "JLINK_SetSpeed");
	JLINK_TIF_Select         = (void*)GetProcAddress(hModule, "JLINK_TIF_Select");

	JLINK_Open               = (void*)GetProcAddress(hModule, "JLINK_Open");
	JLINK_Close              = (void*)GetProcAddress(hModule, "JLINK_Close");
	JLINK_IsOpen             = (void*)GetProcAddress(hModule, "JLINK_IsOpen");

	JLINK_Connect            = (void*)GetProcAddress(hModule, "JLINK_Connect");
	JLINK_IsConnected        = (void*)GetProcAddress(hModule, "JLINK_IsConnected");
	JLINK_SetResetType       = (void*)GetProcAddress(hModule, "JLINK_SetResetType");
	JLINK_Reset              = (void*)GetProcAddress(hModule, "JLINK_Reset");
	JLINK_Halt               = (void*)GetProcAddress(hModule, "JLINK_Halt");
	JLINK_IsHalted           = (void*)GetProcAddress(hModule, "JLINK_IsHalted");
	JLINK_Go                 = (void*)GetProcAddress(hModule, "JLINK_Go");
	JLINK_GoIntDis           = (void*)GetProcAddress(hModule, "JLINK_GoIntDis");

	JLINK_ReadReg            = (void*)GetProcAddress(hModule, "JLINK_ReadReg");
	JLINK_WriteReg           = (void*)GetProcAddress(hModule, "JLINK_WriteReg");

	JLINK_ReadMem            = (void*)GetProcAddress(hModule, "JLINK_ReadMem");
	JLINK_WriteMem           = (void*)GetProcAddress(hModule, "JLINK_WriteMem");
	JLINK_WriteU8            = (void*)GetProcAddress(hModule, "JLINK_WriteU8");
	JLINK_WriteU16           = (void*)GetProcAddress(hModule, "JLINK_WriteU16");
	JLINK_WriteU32           = (void*)GetProcAddress(hModule, "JLINK_WriteU32");

	JLINK_EraseChip          = (void*)GetProcAddress(hModule, "JLINK_EraseChip");
	JLINK_DownloadFile       = (void*)GetProcAddress(hModule, "JLINK_DownloadFile");
	JLINK_BeginDownload      = (void*)GetProcAddress(hModule, "JLINK_BeginDownload");
	JLINK_EndDownload        = (void*)GetProcAddress(hModule, "JLINK_EndDownload");
}



