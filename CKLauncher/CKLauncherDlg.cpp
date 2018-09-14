// CKLauncherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CKLauncher.h"
#include "CKLauncherDlg.h"
#include "..\CKCommon\CKCommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SEPARATE_MODULE

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCKLauncherDlg dialog

CCKLauncherDlg::CCKLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCKLauncherDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCKLauncherDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCKLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCKLauncherDlg)
	DDX_Control(pDX, IDC_LIST1, m_ItemList);
	DDX_Control(pDX, IDC_COMBO1, m_GroupCombo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCKLauncherDlg, CDialog)
	//{{AFX_MSG_MAP(CCKLauncherDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnGroupComboSelChange)
	ON_LBN_DBLCLK(IDC_LIST1, OnItemListDblClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCKLauncherDlg message handlers

BOOL CCKLauncherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//Detection of multiple instance
	HANDLE hMutex;
	#ifdef SEPARATE_MODULE
	hMutex = CreateMutex(NULL, FALSE, "MutexCKLauncherSeparate");
	#else
	hMutex = CreateMutex(NULL, FALSE, "MutexCKLauncher");
	#endif
    if(GetLastError()==ERROR_ALREADY_EXISTS)
	{
		OnCancel();
	}

	char currentPath[256];
	GetCurrentDirectory(256,currentPath);
	//Register the config file so that it can be opened with notepad.
	CKRegisterFile(currentPath,".ck","ckconfig","CK Config File",0);

	RECT DialogBox={0, 0, 215, 250};
	RECT GroupCombo={10, 10, 200, 100};
	RECT ItemList={10, 40, 200, 200};

	MoveWindow(&DialogBox);
	m_GroupCombo.MoveWindow(&GroupCombo);
	m_ItemList.MoveWindow(&ItemList);
#if 0
	//Get Installed Path from Registry
	HKEY SubKey=NULL;
	DWORD size=256L;
	char KeyName[256];
	LONG RegStatus;
	
	//Search for installed winzip self-extractor
	sprintf( KeyName, "Software\\CK Utilities" );
	RegStatus=RegOpenKeyEx(HKEY_LOCAL_MACHINE, KeyName, 0, KEY_ALL_ACCESS, &SubKey);
	if(RegStatus==ERROR_SUCCESS)
	{
		RegStatus=RegQueryValueEx(SubKey,"InstallPath",0,NULL,(LPBYTE)m_ProgramPath,&size);
		//printf("%s\n",extractor);
		RegCloseKey(SubKey);
	}

	if(RegStatus!=ERROR_SUCCESS)
	{
		MessageBox("CK Utilities has not been installed properly. Please reinstall.","",MB_OK);
		OnCancel();
	}
#endif

	//GetCurrentDirectory(256,m_ProgramPath);
	//CKGetProgramPath(m_ProgramPath);

	//Init all the shortcuts
	FILE *inFile;
	char str[1280], tmp[256];
	char ch=',';
	char *pdest;
	int i=0, j=0, numuniquegroup=0, len1, len2;

	m_numshortcut=0;

	for(i=0;i<MAX_SHORTCUT;i++)
	{
		m_shortcut[i].cmdStr=(char*)malloc(MAX_CMD_CHAR*sizeof(char));
		if(!m_shortcut[i].cmdStr)
		{
			MessageBox("Not enough memory","Error",MB_OK);
			for(j=0;j<i;j++)
				free(m_shortcut[j].cmdStr);
			OnCancel();
		}
	}
	i=0;
	j=0;

	//strcpy(tmp,m_ProgramPath);
#ifdef _DEBUG
	strcpy(tmp,"CKLauncher.ck");
#else
	strcpy(tmp,"..\\config\\CKLauncher.ck");
#endif

#ifdef SEPARATE_MODULE
	strcpy(tmp,"CKLauncher.ck");
#endif

	_ConvertConfigFile(tmp);

	inFile = fopen( tmp, "r" );
	if( inFile != NULL )
    {
		fseek( inFile, 0, SEEK_CUR );
#if 1
		while(fgets(str,1280,inFile)!=NULL)
		{
			m_shortcut[i].shortcutItem[0]=0;
			m_shortcut[i].cmdStr[0]=0;
			m_shortcut[i].startIn[0]=0;
			CKRemoveNewLine(str);
			//strcpy(str, filestr);
			//Use the first field as shortcutGroup
			pdest = strchr( str, ch );
			if(pdest==NULL)
				break;

			len1 = pdest - str;
			len2 = strlen(str) - (len1 + 1);

			strncpy(m_shortcut[i].shortcutGroup.str, str, len1);
			m_shortcut[i].shortcutGroup.str[len1]=0;

			//Use the second field as shortcut title
			strncpy( str, str+len1+1, len2+1 );//+1 is for the null terminator			

			pdest = strchr( str, ch );
			if(pdest==NULL)
				break;

			len1 = pdest - str;
			len2 = strlen(str) - (len1 + 1);

			strncpy(m_shortcut[i].shortcutItem, str, len1);
			m_shortcut[i].shortcutItem[len1]=0;

			//Use the third field as executables
			strncpy( str, str+len1+1, len2+1 );//+1 is for the null terminator			

			pdest = strchr( str, ch );
			if(pdest==NULL)
				break;

			len1 = pdest - str;
			len2 = strlen(str) - (len1 + 1);

			strncpy(m_shortcut[i].cmdStr, str, len1);
			m_shortcut[i].cmdStr[len1]=0;

			//Use the fourth field as StartIn
			strncpy( m_shortcut[i].startIn, str+len1+1, len2+1 );//+1 is for the null terminator
			m_numshortcut++;
			i++;
		}
#else
		while(fgets( str, 256, inFile)!=NULL)
		{
			if((str[strlen(str)-1])=='\n') str[strlen(str)-1]='\0';
			if(i%3==0)
			{
				strcpy(m_shortcut[i/3].shortcutGroup.str,str);
			}
			else if(i%3==1)
			{
				strcpy(m_shortcut[i/3].shortcutItem,str);
			}
			else
			{
				strcpy(m_shortcut[i/3].cmdStr,str);
				m_numshortcut++;
			}
			i++;
		}
#endif
		fclose(inFile);

		//Populate unique groups
		for(i=0;i<m_numshortcut;i++)
		{
			int found=0;
			for(j=0;j<numuniquegroup;j++)
			{
				if(strcmp(m_shortcut[i].shortcutGroup.str,m_uniquegroup[j].str)==0)
					found=1;
			}
			if(found==0)
			{
				strcpy(m_uniquegroup[numuniquegroup].str,m_shortcut[i].shortcutGroup.str);
				m_GroupCombo.AddString(m_uniquegroup[numuniquegroup].str);
				numuniquegroup++;
			}
		}

		for(i=0;i<m_numshortcut;i++)
		{
			if(strcmp(m_uniquegroup[0].str, m_shortcut[i].shortcutGroup.str)==0)
				m_ItemList.AddString(m_shortcut[i].shortcutItem);
		}
	}

	m_GroupCombo.SetCurSel(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

//Clean Up
void CCKLauncherDlg::Destroy()
{
	int i;

	for(i=0;i<MAX_SHORTCUT;i++)
	{
		free(m_shortcut[i].cmdStr);
	}
}

void CCKLauncherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCKLauncherDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCKLauncherDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCKLauncherDlg::OnGroupComboSelChange() 
{
	int i;

	m_ItemList.ResetContent();
	for(i=0;i<m_numshortcut;i++)
	{
		if(strcmp(m_uniquegroup[m_GroupCombo.GetCurSel()].str, m_shortcut[i].shortcutGroup.str)==0)
			m_ItemList.AddString(m_shortcut[i].shortcutItem);
	}	
}

void CCKLauncherDlg::OnItemListDblClick() 
{
	char temp[256];
	char temp2[256];
	int i;
	
	m_ItemList.GetText( m_ItemList.GetCurSel(), &temp[0] );
	m_GroupCombo.GetLBText(m_GroupCombo.GetCurSel(), temp2);
	for(i=0;i<m_numshortcut;i++)
	{
		if((strcmp(m_shortcut[i].shortcutItem, temp)==0)&&(strcmp(m_shortcut[i].shortcutGroup.str, temp2)==0))
		{
			OutputDebugString(m_shortcut[i].cmdStr);
			OutputDebugString(m_shortcut[i].startIn);
			//WinExec(m_shortcut[i].cmdStr, SW_SHOWNORMAL);
			if(m_shortcut[i].startIn[0]==0)
				CreateProcess(NULL,m_shortcut[i].cmdStr,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
			else
				CreateProcess(NULL,m_shortcut[i].cmdStr,NULL,NULL,FALSE,0,NULL,m_shortcut[i].startIn,&si,&pi);
			break;
		}
	}	
}

void CCKLauncherDlg::_ConvertConfigFile(char *configFile)
{
	FILE *inFile, *outFile;
	char ch=',';
	char *pdest;
	char str[512], dest[256], newstr[256];
	int ctr=0, len1, len2;

	//Detect if the file format is old
	inFile = fopen( configFile, "r" );
	if(inFile)
	{
		if(fgets(str,512,inFile)!=NULL)
		{
			while((pdest = strchr( str, ch ))!=NULL)
			{
				ctr++;
				len1 = pdest - str;
				len2 = strlen(str) - (len1 + 1);
				strncpy( str, str+len1+1, len2+1 );
			}		
		}
		fclose(inFile);

		//If new file format, return
		if(ctr!=0)
			return;
		
		//Backup the file first
		strcpy(dest, configFile);
		strcat(dest, ".old");
		//MessageBox(dest, configFile, MB_OK);
		if(!CopyFile(configFile,dest,FALSE)) //always overwrite
		{
			CKGetLastError(str);
			MessageBox(str, "Error backing up file...", MB_OK);
		}
		else
		{
			inFile = fopen(dest, "r");
			outFile = fopen(configFile, "w");
			ctr=0;
			if((inFile)&&(outFile))
			{
				while(fgets(str,512,inFile)!=NULL)
				{
					CKRemoveNewLine(str);
					if(ctr==0)
					{
						strcpy(newstr, str);
					}
					else if(ctr==1)
					{
						strcat(newstr,",");
						strcat(newstr, str);
					}
					else if(ctr==2)
					{
						strcat(newstr,",");
						strcat(newstr, str);
						strcat(newstr,",");
						fprintf(outFile,"%s\n",newstr);
					}					
					ctr=(ctr+1)%3;
				}
				fclose(outFile);
				fclose(inFile);
			}
		}
	}
}
