// CPageLog.cpp: 实现文件
//

#include "pch.h"
#include "FeiNiao.h"
#include "CPageLog.h"
#include "afxdialogex.h"


// CPageLog 对话框

IMPLEMENT_DYNAMIC(CPageLog, CDialog)

CPageLog::CPageLog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_LOG, pParent)
{

}

CPageLog::~CPageLog()
{
}

void CPageLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageLog, CDialog)
END_MESSAGE_MAP()


// CPageLog 消息处理程序
