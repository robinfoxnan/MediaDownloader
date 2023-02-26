#pragma once


// CPageVideo 对话框

class CPageVideo : public CDialog
{
	DECLARE_DYNAMIC(CPageVideo)

public:
	CPageVideo(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPageVideo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIDEO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
