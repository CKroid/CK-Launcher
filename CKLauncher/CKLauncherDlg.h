// CKLauncherDlg.h : header file
//

#if !defined(AFX_CKLAUNCHERDLG_H__41C080C7_84B6_4C87_88A0_E9655A161C12__INCLUDED_)
#define AFX_CKLAUNCHERDLG_H__41C080C7_84B6_4C87_88A0_E9655A161C12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_SHORTCUT		1024

#define MAX_SHORTCUT_CHAR	128
#define MAX_CMD_CHAR		512
#define MAX_STARTIN_CHAR	512

typedef struct _SHORTCUTGROUP
{
	char str[128];
} SHORTCUTGROUP;

typedef struct _SHORTCUT
{
	SHORTCUTGROUP shortcutGroup;
	char shortcutItem[MAX_SHORTCUT_CHAR];
	//char cmdStr[MAX_CMD_CHAR];
	char *cmdStr;
	char startIn[MAX_STARTIN_CHAR];
} SHORTCUT;

/////////////////////////////////////////////////////////////////////////////
// CCKLauncherDlg dialog

class CCKLauncherDlg : public CDialog
{
// Construction
public:
	CCKLauncherDlg(CWnd* pParent = NULL);	// standard constructor
	void Destroy();

// Dialog Data
	//{{AFX_DATA(CCKLauncherDlg)
	enum { IDD = IDD_CKLAUNCHER_DIALOG };
	CListBox	m_ItemList;
	CComboBox	m_GroupCombo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCKLauncherDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	STARTUPINFO si;
    PROCESS_INFORMATION pi;

	SHORTCUT m_shortcut[MAX_SHORTCUT];
	SHORTCUTGROUP m_uniquegroup[256];

	char m_ProgramPath[256];

	int m_numshortcut;

	// Generated message map functions
	//{{AFX_MSG(CCKLauncherDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGroupComboSelChange();
	afx_msg void OnItemListDblClick();
	afx_msg void _ConvertConfigFile(char *configFile);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CKLAUNCHERDLG_H__41C080C7_84B6_4C87_88A0_E9655A161C12__INCLUDED_)
