
// Google Earth ConverterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Google Earth Converter.h"
#include "Google Earth ConverterDlg.h"
#include "afxdialogex.h"
#include "convkml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGoogleEarthConverterDlg �Ի���
CGoogleEarthConverterDlg::CGoogleEarthConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGoogleEarthConverterDlg::IDD, pParent)
	, KmlFile(_T(""))
	, InputFile(_T(""))
	, tcolor(0)
	, pcolor(0)
//	, m_formate(0)
	, m_ddmmss(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGoogleEarthConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMB_PointsColor, PointColor);
	DDX_Control(pDX, IDC_COMB_TrackColor, TrackColor);
	DDX_Control(pDX, IDC_EDIT_InputFile, InPutFile);
	DDX_Control(pDX, IDC_EDIT_OutputFile, OutPutFile);
	DDX_Control(pDX, IDC_EDIT_GoogleEarthExeFile, GoogleEarthExeFile);
//  DDX_Control(pDX, IDC_STATIC_Message, Message);
//	DDX_Control(pDX, IDC_COMBO_Formate, Formate);
	DDX_Control(pDX, IDC_COMB_DDMMSS, Ddmmss);
}

BEGIN_MESSAGE_MAP(CGoogleEarthConverterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_INPUTFILE, &CGoogleEarthConverterDlg::OnBnClickedBtnInputfile)
	ON_BN_CLICKED(IDC_BTN_GoogleEarthFile, &CGoogleEarthConverterDlg::OnBnClickedBtnGoogleearthfile)
	ON_BN_CLICKED(ID_BTN_Convert, &CGoogleEarthConverterDlg::OnBnClickedBtnConvert)
	ON_CBN_SELCHANGE(IDC_COMB_PointsColor, &CGoogleEarthConverterDlg::OnCbnSelchangeCombPointscolor)
	ON_CBN_SELCHANGE(IDC_COMB_TrackColor, &CGoogleEarthConverterDlg::OnCbnSelchangeCombTrackcolor)
	ON_BN_CLICKED(IDC_BTN_GoogleEarth, &CGoogleEarthConverterDlg::OnBnClickedBtnGoogleearth)
	ON_BN_CLICKED(IDC_BTN_Help, &CGoogleEarthConverterDlg::OnBnClickedBtnHelp)
//	ON_CBN_SELCHANGE(IDC_COMBO_Formate, &CGoogleEarthConverterDlg::OnCbnSelchangeComboFormate)
	ON_CBN_SELCHANGE(IDC_COMB_DDMMSS, &CGoogleEarthConverterDlg::OnCbnSelchangeCombDdmmss)
END_MESSAGE_MAP()


// CGoogleEarthConverterDlg ��Ϣ�������

BOOL CGoogleEarthConverterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	// SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
//	SetIcon(m_hIcon, TRUE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	SetDlgItemText(IDC_STATIC_Message,"wellcom && thanks");

	//��ȡ��ǰĿ¼�°�ά��ͼ·��
	CString Omappath;

	GetModuleFileName(NULL, Omappath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	Omappath.ReleaseBuffer();                     // �ͷ�Path�ĳ���������ʹ����Ӧ������
	Omappath = Omappath.Left(Omappath.ReverseFind('\\'));   // ȥ����ִ�г����ļ���
	Omappath = Omappath + _T("\\omap.exe");  // �洢�ļ�·����һ��һ������

    //��ʾ��ά��ͼ·��
	GoogleEarthExeFile.SetWindowText(Omappath);

	PointColor.SetCurSel(PointColor.GetCount()-1);
	TrackColor.SetCurSel(TrackColor.GetCount()/2);
//	Formate.SetCurSel(0);
	Ddmmss.SetCurSel(0);

	pcolor=PointColor.GetCurSel();
	tcolor=TrackColor.GetCurSel();
//	m_formate=Formate.GetCurSel();
	m_ddmmss = Ddmmss.GetCurSel();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGoogleEarthConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGoogleEarthConverterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGoogleEarthConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGoogleEarthConverterDlg::OnBnClickedBtnInputfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int li;

	li=-1;
	
	//�ļ��Ի���
	CFileDialog cfd(true,_T(".dat"),_T(""),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Dat Files (*.dat)|*.dat|All Files (*.*)|*.*||"), NULL);    

	if (IDOK==cfd.DoModal()) {     //�ļ��Ի���ѡ��ť
		InPutFile.SetReadOnly(FALSE);
		InPutFile.SetWindowText(cfd.GetPathName());
		InputFile=cfd.GetPathName();
		InPutFile.SetReadOnly(TRUE);
	}

	li = InputFile.Find('.');
	KmlFile = InputFile.Left(li)+_T(".kml");

	OutPutFile.SetReadOnly(FALSE);
	OutPutFile.SetWindowText(KmlFile);
	OutPutFile.SetReadOnly(TRUE);
}


void CGoogleEarthConverterDlg::OnBnClickedBtnGoogleearthfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�ļ��Ի���
	CFileDialog cfd(true,_T(""),_T(""),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("All Files (*.*)|*.*||"), NULL);    

	if (IDOK==cfd.DoModal()) {     //�ļ��Ի���ѡ��ť
		GoogleEarthExeFile.SetReadOnly(FALSE);
		GoogleEarthExeFile.SetWindowText(cfd.GetPathName());
		GoogleEarthExeFile.SetReadOnly(TRUE);
	}
}


void CGoogleEarthConverterDlg::OnBnClickedBtnConvert()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char InFile [1024]="";
	char OutFile[1024]="";

	int i;

	if(""==InputFile || ""==KmlFile ){
		SetDlgItemText(IDC_STATIC_Message,"No Input Data");
//		MyBreak("No Input Data!");
		return;
	}

	for(i=0;i<InputFile.GetLength();i++){
		InFile[i]=InputFile[i];
	}

	for(i=0;i<KmlFile.GetLength();i++){
		OutFile[i]=KmlFile[i];
	}

	SetDlgItemText(IDC_STATIC_Message,"Covernt to KML, waitting...");

	if (!convkml(InFile,OutFile,tcolor,pcolor,m_ddmmss)) 
		/*MyBreak("Data writed failed!");*/
		SetDlgItemText(IDC_STATIC_Message,"Failed");
	else SetDlgItemText(IDC_STATIC_Message,"Done");
}

void CGoogleEarthConverterDlg::OnCbnSelchangeCombPointscolor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	pcolor=PointColor.GetCurSel();
	
}

void CGoogleEarthConverterDlg::OnCbnSelchangeCombTrackcolor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	tcolor=TrackColor.GetCurSel();
}

static int ExecCmd(char *cmd)
{
	STARTUPINFO si={0};
	PROCESS_INFORMATION info;
	si.cb=sizeof(si);
	if (!CreateProcess(NULL,cmd,NULL,NULL,false,CREATE_NO_WINDOW,NULL,NULL,&si,
		&info)) return 0;
	CloseHandle(info.hProcess);
	CloseHandle(info.hThread);
	return 1;
}
void CGoogleEarthConverterDlg::OnBnClickedBtnGoogleearth()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char cmd[1024];
	CString GooglExe;
	GetDlgItemText(IDC_EDIT_GoogleEarthExeFile,GooglExe);
	sprintf(cmd,"\"%s\" \"%s\"",GooglExe,KmlFile);
	if (!ExecCmd(cmd)) SetDlgItemText(IDC_STATIC_Message,"cmd error"); 
}


void CGoogleEarthConverterDlg::OnBnClickedBtnHelp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAboutDlg Help;
	Help.DoModal();
}

/*
void CGoogleEarthConverterDlg::OnCbnSelchangeComboFormate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_formate = Formate.GetCurSel();
}
*/

void CGoogleEarthConverterDlg::OnCbnSelchangeCombDdmmss()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_ddmmss = Ddmmss.GetCurSel();

}
