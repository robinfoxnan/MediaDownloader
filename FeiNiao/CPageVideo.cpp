// CPageVideo.cpp: 实现文件
//

#include "pch.h"
#include "FeiNiao.h"
#include "CPageVideo.h"
#include "afxdialogex.h"


// CPageVideo 对话框

IMPLEMENT_DYNAMIC(CPageVideo, CDialog)

CPageVideo::CPageVideo(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_VIDEO, pParent)
{

}

CPageVideo::~CPageVideo()
{
}

void CPageVideo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageVideo, CDialog)
END_MESSAGE_MAP()


// CPageVideo 消息处理程序
