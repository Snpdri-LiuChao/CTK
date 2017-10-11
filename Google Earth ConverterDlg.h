
// Google Earth ConverterDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <cstring>
#include <stdlib.h>
#include <stdio.h>

// CGoogleEarthConverterDlg 对话框
class CGoogleEarthConverterDlg : public CDialogEx
{
// 构造
public:
	CGoogleEarthConverterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GOOGLEEARTHCONVERTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox PointColor;
	CComboBox TrackColor;
	CEdit InPutFile;
	CEdit OutPutFile;
	CEdit GoogleEarthExeFile;
	afx_msg void OnBnClickedBtnInputfile();

	CString KmlFile;
	afx_msg void OnBnClickedBtnGoogleearthfile();
	afx_msg void OnBnClickedBtnConvert();
	CString InputFile;
//	CString OutputFile;
	afx_msg void OnCbnSelchangeCombPointscolor();
	afx_msg void OnCbnSelchangeCombTrackcolor();
	int tcolor;
	int pcolor;
	afx_msg void OnBnClickedBtnGoogleearth();
//	CStatic Message;
//	void ShowMsg(CString str);
	afx_msg void OnBnClickedBtnHelp();
//	CComboBox Formate;
//	afx_msg void OnCbnSelchangeComboFormate();
//	int m_formate;

// 20170927 增加不同角度格式输入方式
	CComboBox Ddmmss;
	afx_msg void OnCbnSelchangeCombDdmmss();
	int m_ddmmss;
};
