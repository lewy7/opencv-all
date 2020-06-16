// opencvMFCDlg.h : header file
//

#if !defined(AFX_OPENCVMFCDLG_H__A665CABC_4434_49F5_B4AE_5937F43E03F6__INCLUDED_)
#define AFX_OPENCVMFCDLG_H__A665CABC_4434_49F5_B4AE_5937F43E03F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COpencvMFCDlg dialog

class COpencvMFCDlg : public CDialog
{
// Construction
public:
	COpencvMFCDlg(CWnd* pParent = NULL);	// standard constructor
// Dialog Data
	//{{AFX_DATA(COpencvMFCDlg)
	enum { IDD = IDD_OPENCVMFC_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpencvMFCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(COpencvMFCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAction();
	afx_msg void OnDisplay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENCVMFCDLG_H__A665CABC_4434_49F5_B4AE_5937F43E03F6__INCLUDED_)
