#ifndef _CKCOMMON_H_
#define _CKCOMMON_H_
#define CKMSG(_msg_) MessageBox(_msg_,"",MB_OK)
//-----------------------------------------
//Share by CKAudioTester and CKAudioScriptor
//Script Commands
enum{
	SCRIPT_OPEN = 0,
	SCRIPT_PLAY,
	SCRIPT_DELAY,
	SCRIPT_PAUSE,
	SCRIPT_STOP,
	SCRIPT_PRINT,
	SCRIPT_LOG,
	SCRIPT_LOOPSTART,
	SCRIPT_LOOPEND,
	MAX_SCRIPT
};

//maximum number of stereo stream
#define MAX_NUM_STEREO_STREAM				32+1 //additonal mpeg

//Script defines
#define NUM_SCRIPT_CMDS	256

typedef struct _SCRIPTCMD {
	DWORD dwCmd;
	DWORD dwParam1;
	char cParam2[256];
	DWORD dwParam3;
	bool bParam4;
	char cParam5[256];
	DWORD dwParam6;
	bool bEnabled;
} SCRIPTCMD, *PSCRIPTCMD;
//-----------------------------------------

void CKGetProgramPath(char *inputStr);
void CKRemoveNewLine(char *String);
void CKGetLastError(char* Message);
void CKRegisterFile(char *path, char *extension, char *key, char *description, int icon);
void CKExtractSubstring(char *str, char delimiter, char *substr);
void CKGetFilePathAndName(char* fullfilename, char* filepath, char* filename);
BOOL CKFindFile(char *filename);

#endif
