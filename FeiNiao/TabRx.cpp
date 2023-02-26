// TabRx.cpp : implementation file
//

#include "pch.h"
#include "TabRx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabRx

CTabRx::CTabRx()
{
	ItemArray.SetSize(10, -1);
	index = 0;
	bMove = FALSE;
	tickDelay = 15;
	pix = 50;
}

CTabRx::~CTabRx()
{
	ItemArray.RemoveAll();
}


BEGIN_MESSAGE_MAP(CTabRx, CTabCtrl)
	//{{AFX_MSG_MAP(CTabRx)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabRx message handlers

void CTabRx::InsertItem(LPCTSTR str, CWnd *pPage)
{
	pPage->SetOwner(this);
	ItemArray.SetAt(index, pPage);
	CTabCtrl::InsertItem(index, str);
	index ++;
	SetSize();
	ChangeTab(0);
}

BOOL CTabRx::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TCN_SELCHANGE
	
	return CTabCtrl::OnNotify(wParam, lParam, pResult);
}

void CTabRx::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int t = CTabCtrl::GetCurSel();
	ChangeTab(t);
	*pResult = 0;
}

void CTabRx::ChangeTab(int t)
{
	KillTimer(81);
	SetSize();

	CWnd * p;
	for (int i=0; i<index; i++)
	{
		p = (CWnd *)ItemArray.GetAt(i);
		p->ShowWindow(SW_HIDE);	
	}
	p = (CWnd *)ItemArray.GetAt(t);

	if (!bMove)
		p->ShowWindow(SW_SHOW);
	else
		MoveTab(p);

}

void CTabRx::MoveTab(CWnd *p)
{

	page = p;
	CRect rt;
	GetWorkRect(rt);
	left = rt.right - pix;
	rt.left = left;
	page->MoveWindow(&rt, TRUE);
	page->ShowWindow(SW_SHOW);
	SetTimer(81, tickDelay, NULL);
}

void CTabRx::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 81)
	{
		CRect rt;
		GetWorkRect(rt);
		left = left - pix;
		if (left <= rt.left)
		{
			KillTimer(81);
			left = rt.left;
		}
		rt.left = left;
		page->MoveWindow(&rt, TRUE);
	}
	
	CTabCtrl::OnTimer(nIDEvent);
}

void CTabRx::OnSize(UINT nType, int cx, int cy) 
{
	CTabCtrl::OnSize(nType, cx, cy);
	SetSize();

}


void CTabRx::SetSize()
{
	CRect rt;
	this->GetClientRect(&rt);

	CRect rtItem;
	int height = CTabCtrl::GetItemRect(0, &rtItem);
	rt.top += rtItem.Height() + 2;
	rt.bottom -= 4;
	rt.left += 2;
	rt.right -= 4;
	CWnd * p;
	for (int i=0; i<index; i++)
	{
		p = (CWnd *)ItemArray.GetAt(i);
		p->MoveWindow(&rt, TRUE);
	}	
}


void CTabRx::GetWorkRect(CRect &rt)
{
	this->GetClientRect(&rt);

	CRect rtItem;
	int height = CTabCtrl::GetItemRect(0, &rtItem);
	rt.top += rtItem.Height() + 2;
	rt.bottom -= 4;
	rt.left += 2;
	rt.right -= 4;
}


