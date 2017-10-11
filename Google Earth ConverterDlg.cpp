
// Google Earth ConverterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Google Earth Converter.h"
#include "Google Earth ConverterDlg.h"
#include "afxdialogex.h"
#include "convkml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CGoogleEarthConverterDlg 对话框
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


// CGoogleEarthConverterDlg 消息处理程序

BOOL CGoogleEarthConverterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	// SetIcon(m_hIcon, TRUE);			// 设置大图标
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
//	SetIcon(m_hIcon, TRUE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	SetDlgItemText(IDC_STATIC_Message,"wellcom && thanks");

	//获取当前目录下奥维地图路径
	CString Omappath;

	GetModuleFileName(NULL, Omappath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	Omappath.ReleaseBuffer();                     // 释放Path的长度锁定，使其适应自增长
	Omappath = Omappath.Left(Omappath.ReverseFind('\\'));   // 去掉可执行程序文件名
	Omappath = Omappath + _T("\\omap.exe");  // 存储文件路径，一级一级创建

    //显示奥维地图路径
	GoogleEarthExeFile.SetWindowText(Omappath);

	PointColor.SetCurSel(PointColor.GetCount()-1);
	TrackColor.SetCurSel(TrackColor.GetCount()/2);
//	Formate.SetCurSel(0);
	Ddmmss.SetCurSel(0);

	pcolor=PointColor.GetCurSel();
	tcolor=TrackColor.GetCurSel();
//	m_formate=Formate.GetCurSel();
	m_ddmmss = Ddmmss.GetCurSel();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGoogleEarthConverterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGoogleEarthConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGoogleEarthConverterDlg::OnBnClickedBtnInputfile()
{
	// TODO: 在此添加控件通知处理程序代码
	int li;

	li=-1;
	
	//文件对话框
	CFileDialog cfd(true,_T(".dat"),_T(""),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Dat Files (*.dat)|*.dat|All Files (*.*)|*.*||"), NULL);    

	if (IDOK==cfd.DoModal()) {     //文件对话框选择按钮
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
	// TODO: 在此添加控件通知处理程序代码
	//文件对话框
	CFileDialog cfd(true,_T(""),_T(""),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("All Files (*.*)|*.*||"), NULL);    

	if (IDOK==cfd.DoModal()) {     //文件对话框选择按钮
		GoogleEarthExeFile.SetReadOnly(FALSE);
		GoogleEarthExeFile.SetWindowText(cfd.GetPathName());
		GoogleEarthExeFile.SetReadOnly(TRUE);
	}
}


void CGoogleEarthConverterDlg::OnBnClickedBtnConvert()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	pcolor=PointColor.GetCurSel();
	
}

void CGoogleEarthConverterDlg::OnCbnSelchangeCombTrackcolor()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	char cmd[1024];
	CString GooglExe;
	GetDlgItemText(IDC_EDIT_GoogleEarthExeFile,GooglExe);
	sprintf(cmd,"\"%s\" \"%s\"",GooglExe,KmlFile);
	if (!ExecCmd(cmd)) SetDlgItemText(IDC_STATIC_Message,"cmd error"); 
}


void CGoogleEarthConverterDlg::OnBnClickedBtnHelp()
{
	// TODO: 在此添加控件通知处理程序代码
	CAboutDlg Help;
	Help.DoModal();
}

/*
void CGoogleEarthConverterDlg::OnCbnSelchangeComboFormate()
{
	// TODO: 在此添加控件通知处理程序代码
	m_formate = Formate.GetCurSel();
}
*/

void CGoogleEarthConverterDlg::OnCbnSelchangeCombDdmmss()
{
	// TODO:  在此添加控件通知处理程序代码
	m_ddmmss = Ddmmss.GetCurSel();

}
