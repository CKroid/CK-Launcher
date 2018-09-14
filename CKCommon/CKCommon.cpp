#include "stdafx.h"
#include "CKCommon.h"
//#include <string.h>

void CKGetProgramPath(char *inputStr)
{
	char str[256], ch='\\', temp[256],ProgramPath[256];
	char *pdest;
	int len1,len2;
	
	strcpy(str, inputStr);
	ProgramPath[0]=0;
	
	while(1)
	{
		pdest = strchr( str, ch );
		if(pdest==NULL)
			break;

		len1 = pdest - str;
		len2 = strlen(str) - (len1 + 1);

		strncpy(temp, str, len1);
		temp[len1]=ch;
		temp[len1+1]=0;
		strcat(ProgramPath,temp);

		strncpy( str, str+len1+1, len2+1 );//+1 is for the null terminator
	}
	if(strcmp(str,"WinApps")==0)//it is run by itself
	{
		ProgramPath[strlen(ProgramPath)-1]=0;
		strcpy(inputStr, ProgramPath);
	}
}

void CKRemoveNewLine(char *String)
{
	if(String[strlen(String)-1]==10)
		String[strlen(String)-1]=0;
	if(String[strlen(String)-1]==13)
		String[strlen(String)-1]=0;
}

void CKGetLastError(char* Message)
{
	LPVOID lpMsgBuf;
	DWORD length;
	char tmp[256];
	tmp[0]=0;

	length = FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		0,//MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	strcpy(Message,(char*)lpMsgBuf);
	CKRemoveNewLine(Message);
	LocalFree( lpMsgBuf );
}

void CKRegisterFile(char *path, char *extension, char *key, char *description, int icon)
{
	//Search Registry to see if it has been registered.
	HKEY SubKey=NULL, SubKey2=NULL;
	LONG RegStatus;
	DWORD dwValue;
	char tempPath[256], szValue[256];

	strcpy(tempPath, path);
	CKGetProgramPath(tempPath);
	
	//Search for any previous regstration of config file
	RegStatus=RegOpenKey(HKEY_CLASSES_ROOT, extension, &SubKey);
	
	if(RegStatus==ERROR_SUCCESS)
	{
		//If found, we don't need to do anything
		RegCloseKey(SubKey);
	}
	else
	{
		//If not found, we will patch it
		RegCreateKey(HKEY_CLASSES_ROOT, extension, &SubKey);
		RegSetValue(SubKey,"",REG_SZ,key,strlen(key));
		RegCloseKey(SubKey);

		RegCreateKey(HKEY_CLASSES_ROOT, key, &SubKey);
		RegSetValue(SubKey,"",REG_SZ,description,strlen(description));
		dwValue=0;
		RegSetValueEx(SubKey, "EditFlags", NULL, REG_DWORD, (LPBYTE)&dwValue, sizeof(DWORD));
		dwValue=8;
		RegSetValueEx(SubKey, "BrowserFlags", NULL, REG_DWORD, (LPBYTE)&dwValue, sizeof(DWORD));

		RegCreateKey(SubKey, "DefaultIcon", &SubKey2);
		sprintf(szValue,"%s\\CK Utilities.exe,%d",tempPath,icon);
		//strcpy(szValue,tempPath);
		//strcat(szValue,"\\CK Utilities.exe,0");
		RegSetValue(SubKey2,"",REG_SZ,szValue,strlen(szValue));
		RegCloseKey(SubKey2);

		RegCreateKey(SubKey, "shell", &SubKey2);
		RegSetValue(SubKey2,"",REG_SZ,"open",4);
		RegCloseKey(SubKey2);

		RegCreateKey(SubKey, "shell\\open\\command", &SubKey2);
		RegSetValue(SubKey2,"",REG_SZ,"notepad.exe %1",14);
		RegCloseKey(SubKey2);
		
		RegCloseKey(SubKey);
	}
}

// str			: Remaining of the string
// delimiter	: The delimiter of the string
// substr		: The extracted string
void CKExtractSubstring(char *str, char delimiter, char *substr)
{
	char *pdest;
	int len1, len2;
	
	pdest = strchr( str, delimiter );
	if(pdest==NULL)
	{
		strcpy(substr, str);
		return;
	}
	
	len1 = pdest - str;
	len2 = strlen(str) - (len1 + 1);

	strncpy(substr, str, len1);
	substr[len1]=0;
	strncpy( str, str+len1+1, len2+1 );//+1 is for the null terminator
}

//fullfilename	:Full filename with path
//filepath		:path of the file
//filename		:filename without path
void CKGetFilePathAndName(char* fullfilename, char* filepath, char* filename)
{
	char cFilename[256];
	char ctemp[256];
	int a=0;

	strcpy(cFilename,fullfilename);
	
	while(1)
	{
		CKExtractSubstring(cFilename, '\\', ctemp);

		if(strcmp(cFilename,ctemp)==0)
			break;

		if(a==0)
		{
			strcpy(filepath,ctemp);
			a++;
		}
		else
		{
			strcat(filepath,"\\");
			strcat(filepath,ctemp);
		}
	}
	strcpy(filename,cFilename);
}

BOOL CKFindFile(char *filename)
{
	CFileFind finder;
	//Search for the file
	return finder.FindFile(filename);
}
