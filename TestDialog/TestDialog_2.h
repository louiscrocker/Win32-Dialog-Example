#pragma once
#include "afxwin.h"
#include "SerialMFC.h"
#include "shockwaveflash1.h"


// TestDialog_2 dialog

class TestDialog_2 : public CDialog
{
	DECLARE_DYNAMIC(TestDialog_2)

public:
	TestDialog_2(CWnd* pParent = NULL);   // standard constructor
	virtual ~TestDialog_2();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CSerialMFC serial;

	CEdit m_Edit1;
	CEdit m_Edit2;
	CString m_Edit_1;
	CString m_Edit_2;

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();

protected:
	afx_msg LRESULT OnSerialMsg (WPARAM wParam, LPARAM lParam);

protected:
	// Implementation
	void DisplayEvent(LPCTSTR lpszMsg);
	void DisplayEventSetting(LPCTSTR lpszMsg, LPCTSTR lpszSetting, bool fOn);
	void DisplayData(LPCTSTR pszData);
	
private:
	int SetupSerialPort();
	void CloseSerialPort();
	void SendSerialData();
	int ShowError(LONG lError, LPCTSTR lptszMessage);
public:
	CShockwaveflash1 m_1;
};
