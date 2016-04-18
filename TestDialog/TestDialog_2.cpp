//==================================================================================================
// TestDialog_2.cpp : implementation file
//
//==================================================================================================

#include "stdafx.h"
#include "TestDialog.h"
#include "TestDialog_2.h"
#include "TestDialogDlg.h"

//==================================================================================================
// TestDialog_2 dialog

IMPLEMENT_DYNAMIC(TestDialog_2, CDialog)

//==================================================================================================
TestDialog_2::TestDialog_2(CWnd* pParent /*=NULL*/)
	: CDialog(TestDialog_2::IDD, pParent)
	, m_Edit_1(_T(""))
{
	
}

//==================================================================================================
TestDialog_2::~TestDialog_2()
{
	serial.Close();
}

//==================================================================================================
void TestDialog_2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Text(pDX, IDC_EDIT1, m_Edit_1);

	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Text(pDX, IDC_EDIT2, m_Edit_2);
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH1, m_1);
}

//==================================================================================================
BEGIN_MESSAGE_MAP(TestDialog_2, CDialog)
	ON_BN_CLICKED(IDCANCEL, &TestDialog_2::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &TestDialog_2::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &TestDialog_2::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &TestDialog_2::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &TestDialog_2::OnBnClickedButton4)
	ON_WM_SERIAL(OnSerialMsg)
END_MESSAGE_MAP()

//==================================================================================================
// TestDialog_2 message handlers
//==================================================================================================
void TestDialog_2::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

//==================================================================================================
void TestDialog_2::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	CTestDialogDlg t = new CTestDialogDlg();
	t.DoModal();
}

//==================================================================================================
void TestDialog_2::OnBnClickedButton2()
{
	SendSerialData();
}

//==================================================================================================
void TestDialog_2::OnBnClickedButton3()
{
	SetupSerialPort();
}

//==================================================================================================
void TestDialog_2::OnBnClickedButton4()
{
	CloseSerialPort();
}

//==================================================================================================
int TestDialog_2::SetupSerialPort()
{
	serial.Open(_T("COM8"), this);

	serial.Setup(CSerial::EBaud9600, CSerial::EData8, CSerial::EParNone, CSerial::EStop1);
	serial.SetupHandshaking(CSerial::EHandshakeHardware);

	return 0;
}

//==================================================================================================
void TestDialog_2::CloseSerialPort()
{
	serial.Close();
}

//==================================================================================================
void TestDialog_2::SendSerialData()
{
	UpdateData(true);
	
	//CString str_value;
	//GetDlgItemText(IDC_EDIT1, str_value);
	
	CStringA str_convert(m_Edit_1);
	LPCSTR s = str_convert;

	//LPCSTR s = (LPCSTR)(LPCTSTR)str_value;
	
	serial.Write(s);
	//serial.Write("Hello From C++ Socket");
}

//==================================================================================================
LRESULT TestDialog_2::OnSerialMsg (WPARAM wParam, LPARAM /*lParam*/)
{
	CSerial::EEvent eEvent = CSerial::EEvent(LOWORD(wParam));
	CSerial::EError eError = CSerial::EError(HIWORD(wParam));

	if (eError)
		DisplayEvent(_T("An internal error occurred."));

	if (eEvent & CSerial::EEventBreak)
		DisplayEvent(_T("Break detected on input."));

	if (eEvent & CSerial::EEventError)
		DisplayEvent(_T("A line-status error occurred."));
	
	if (eEvent & CSerial::EEventRcvEv)
		DisplayEvent(_T("Event character has been received."));

	if (eEvent & CSerial::EEventRing)
		DisplayEvent(_T("Ring detected"));
	
	if (eEvent & CSerial::EEventSend)
		DisplayEvent(_T("All data is send"));
	
	if (eEvent & CSerial::EEventCTS)
		DisplayEventSetting(_T("CTS signal change"), _T("CTS"), serial.GetCTS());
	
	if (eEvent & CSerial::EEventDSR)
		DisplayEventSetting(_T("DSR signal change"), _T("DSR"), serial.GetDSR());
	
	if (eEvent & CSerial::EEventRLSD)
		DisplayEventSetting(_T("RLSD signal change"), _T("RLSD"), serial.GetRLSD());
	
	if (eEvent & CSerial::EEventRecv)
	{
		// Create a clean buffer
		DWORD dwRead;
		char szData[101];
		const int nBuflen = sizeof(szData)-1;

		// Obtain the data from the serial port
		do
		{
			serial.Read(szData,nBuflen,&dwRead);
			szData[dwRead] = '\0';

			// Scan the string for unwanted characters
			for (DWORD dwChar=0; dwChar<dwRead; dwChar++)
			{
				if (!isprint(szData[dwChar]) && !isspace(szData[dwChar]))
				{
					szData[dwChar] = '.';
				}
			}

#ifdef _UNICODE
			// Convert the ANSI data to Unicode
			LPTSTR lpszData = LPTSTR(_alloca((dwRead+1)*sizeof(TCHAR)));
			if (!::MultiByteToWideChar(CP_ACP, 0, szData, -1, lpszData, dwRead+1))
				return 0;

			// Display the fetched string
			DisplayData(lpszData);
#else
			// Display the fetched string
			DisplayData(szData);
#endif
		} while (dwRead == nBuflen);
	}

	return 0;
}

//==================================================================================================
void TestDialog_2::DisplayEvent(LPCTSTR lpszMsg)
{
	
}

//==================================================================================================
void TestDialog_2::DisplayEventSetting (LPCTSTR lpszMsg, LPCTSTR lpszSetting, bool fOn)
{
	TCHAR tszMsg[200];
	wsprintf(tszMsg,_T("%s (%s=%s)"), lpszMsg, lpszSetting, (fOn?_T("on"):_T("off")));
	DisplayEvent(tszMsg);
}

//==================================================================================================
void TestDialog_2::DisplayData (LPCTSTR pszData)
{
	m_Edit_2 = pszData;

	UpdateData(false);
}

//==================================================================================================
// end of file
//==================================================================================================

