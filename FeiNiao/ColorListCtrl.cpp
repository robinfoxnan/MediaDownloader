// Author:      SFr frydaysoft@gmx.de

#include "pch.h"
#include "ColorListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CColorListCtrl

CColorListCtrl::CColorListCtrl()
{
    m_columnSize = 0;               // number of columns
    m_fullColumnLines = FALSE;      // paint vert. lines | | |
    m_withSingleRect = FALSE;       // paint single rect for all sub-items
#ifdef _DEBUG
    m_BigFocusRect = FALSE;         // focus-rect with small/big dots
#else
    m_BigFocusRect = FALSE;         // focus-rect with small/big dots
#endif
    IsCreated = FALSE;
}

//***************************************************************
CColorListCtrl::~CColorListCtrl()
{
}

//***************************************************************
BEGIN_MESSAGE_MAP(CColorListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CColorListCtrl)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// 
void CColorListCtrl::OnDestroy() 
{
	DeleteAllItems();	
	CListCtrl::OnDestroy();
}

//***************************************************************
void CColorListCtrl::SetColumnType(BOOL bNew)
{
    m_fullColumnLines=bNew;     // paint no/vert. lines | | |
    if (IsCreated) 
    {
        RedrawWindow();
    }
}
void CColorListCtrl::SetRectType(BOOL bNew)
{
    m_withSingleRect=bNew;     // paint no/single rect for all sub-items
    if (IsCreated)
    {
        RedrawWindow();
    }
}
void CColorListCtrl::SetFocusType(BOOL bNew)
{
    m_BigFocusRect=bNew;       // focus-rect with small/big dots
    if (IsCreated)
    {
        RedrawWindow();
    }
}

//***************************************************************
CMyLParam*	CColorListCtrl::GetMyLParam(int Index)
{
//    LVITEM	Item;
//	Item.mask		= LVIF_PARAM;
//	Item.iItem		= Index;
//	Item.iSubItem	= 0;
//	CListCtrl::GetItem(&Item);
	CMyLParam * p = (CMyLParam *)CListCtrl::GetItemData(Index);
	return p;
}

//***************************************************************
CRect CColorListCtrl::GetFullCellRect(int nItem, BOOL outside)
{
	static CRect rectCell;
    CListCtrl::GetItemRect(nItem, rectCell, LVIR_BOUNDS);
    if (!outside)
    {   //decrease 1 pixel
        rectCell.left++;    rectCell.right--;
        rectCell.top++;     rectCell.bottom--;
    }
	return rectCell;
}

//***************************************************************
CRect CColorListCtrl::GetCellRect(int nItem, int nColumn, BOOL outside)
{
	static CRect rectCell;

	// Get the height
    CListCtrl::GetItemRect(nItem, rectCell, LVIR_BOUNDS);
    LV_COLUMN column;
	column.mask = LVCF_WIDTH;        // Set the mask to only return the width

    for (int x=0; x<nColumn; x++)
    {
        column.iSubItem=1;
        CListCtrl::GetColumn(x, &column); // Get rect-edge (left/top)
        rectCell.left+=column.cx;
    }
    column.iSubItem=1;
    CListCtrl::GetColumn(nColumn, &column); // Get rect-edge (right/bottom)
    rectCell.right = rectCell.left + column.cx;

    if (!outside)
    {   //decrease 1 pixel
        rectCell.left++;    rectCell.right--;
        rectCell.top++;     rectCell.bottom--;
    }
	return rectCell;
}

//***************************************************************
void CColorListCtrl::InvalidateFullItem(int nItem, BOOL bErase)
{
    CRect rcBounds = GetFullCellRect(nItem, TRUE);
	InvalidateRect(rcBounds, bErase);

}

//***************************************************************
BOOL CColorListCtrl::DeleteAllItems()
{
	while (CListCtrl::GetItemCount())
	{
        CColorListCtrl::DeleteItem(0);
	}

    return TRUE;
}

//***************************************************************
BOOL CColorListCtrl::DeleteItem(int nItem )
{
    if (CListCtrl::GetItemCount() <= nItem) 
		return FALSE;

    CMyLParam *p = (CMyLParam*)CListCtrl::GetItemData(nItem);
    if (p)
    {
        CListCtrl::SetItemData(nItem, p->GetData());
        delete p;
        return CListCtrl::DeleteItem(nItem);
    }
    return TRUE;
}


//***************************************************************
DWORD CColorListCtrl::GetItemData( int nItem ) const
{
    CMyLParam *p = (CMyLParam*)CListCtrl::GetItemData(nItem);
    if (p)
    {
        return p->GetData();
        return TRUE;
    }
    return 0;
}

//***************************************************************
BOOL  CColorListCtrl::SetItemData( int nItem, DWORD dwData )
{
    CMyLParam *p = (CMyLParam*)CListCtrl::GetItemData(nItem);
    if (p)
    {
        p->SetData(dwData);
        return TRUE;
    }
    return FALSE;
}

//***************************************************************
BOOL CColorListCtrl::SetItemBackgndColor(COLORREF color, int Index, int iSub)	
{
    CMyLParam *p = GetMyLParam(Index);
    if (!p) return FALSE;
    if (p->SetBackColor(color, iSub))
    {
        RedrawItems(Index, Index);
        return TRUE;
    }
    return FALSE;
}

//***************************************************************
BOOL CColorListCtrl::SetItemTextColor(COLORREF color, int Index, int iSub)	
{
    CMyLParam *p = GetMyLParam(Index);
    if (!p) 
		return FALSE;
    if (p->SetTextColor(color, iSub))
    {
        RedrawItems(Index, Index);
        return TRUE;
    }
    return FALSE;
}

//***************************************************************
COLORREF CColorListCtrl::GetItemBackgndColor(int Index, int iSub)
{
    CMyLParam *p = GetMyLParam(Index);
    if (!p) 
		return 0;
    return p->GetBackColor(iSub);
}

//***************************************************************
COLORREF CColorListCtrl::GetItemTextColor(int Index, int iSub)
{
    CMyLParam *p = GetMyLParam(Index);
    if (!p) 
		return 0;
    return p->GetTextColor(iSub);
}

//***************************************************************
void CColorListCtrl::OnPaint() 
{
	// First let the control do its default drawing.
	const MSG *msg = GetCurrentMessage();
	DefWindowProc(msg->message, msg->wParam, msg->lParam);

    if (!m_fullColumnLines) return;

	// Draw the lines only for LVS_REPORT mode
	if((GetStyle() & LVS_TYPEMASK) == LVS_REPORT)
	{
		// Get the number of columns
		CClientDC dc(this);
		CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
		int nColumnCount = pHeader->GetItemCount();

		// The bottom of the header corresponds to the top of the line 
		RECT rect;
		pHeader->GetClientRect(&rect);
		int top = rect.bottom;

		// Now get the client rect so we know the line length and
		// when to stop
		GetClientRect(&rect);

		// The border of the column is offset by the horz scroll
		int borderx = 0 - GetScrollPos(SB_HORZ);

        CPen       *pOldPen;
        CPen        pen;
        CGdiObject *pOldBrush;
        pen.CreatePen( PS_DOT, 0, ::GetSysColor(COLOR_HIGHLIGHTTEXT));
        pOldPen  = dc.SelectObject(&pen);
        pOldBrush = dc.SelectStockObject(NULL_BRUSH);

		for (int i=0; i<nColumnCount; i++)
		{
			// Get the next border
			borderx += GetColumnWidth(i);

			// if next border is outside client area, break out
			if(borderx >= rect.right) 
				break;

			// Draw the line.
			dc.MoveTo(borderx-1, top);
			dc.LineTo(borderx-1, rect.bottom);
		}
        dc.SelectObject(pOldPen);
        dc.SelectObject(pOldBrush);

	}
	// Do not call CListCtrl::OnPaint() for painting messages
}

//***************************************************************
void CColorListCtrl::RepaintSelectedItems()
{
	// Invalidate focused item so it can repaint 
	int nItem = GetNextItem(-1, LVNI_FOCUSED);

	if(nItem != -1)
	{
        InvalidateFullItem(nItem, FALSE);
	}

	// Invalidate selected items depending on LVS_SHOWSELALWAYS
	if((GetStyle() & LVS_SHOWSELALWAYS))
	{
		for(nItem = GetNextItem(-1, LVNI_SELECTED);
			nItem != -1; nItem = GetNextItem(nItem, LVNI_SELECTED))
		{
            InvalidateFullItem(nItem, FALSE);
		}
	}
	UpdateWindow();
}

//***************************************************************
BOOL CColorListCtrl::SetSel(int pos, BOOL bSelect)
{
    // LVS_ES_FULLROWSELECT only works with newer version of Comctl32
    // This style and some of the other new extended styles 
    // only work with version 4.72 (or higer) of COMCTL32.DLL 

    UINT nState = 0;
    if (bSelect) 
		nState = LVIS_SELECTED | LVS_EX_FULLROWSELECT; 
	else 
		nState=0;

    if (SetItemState(pos, nState, LVIS_SELECTED | LVS_EX_FULLROWSELECT))
    {
        if((GetStyle() & LVS_SHOWSELALWAYS))
        {
            InvalidateFullItem(pos, TRUE);
        }
        return TRUE;
    }
    return FALSE;
}
//***************************************************************
void CColorListCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	SendMessage(WM_PAINT);
    CListCtrl::OnKillFocus(pNewWnd);
	// check if we are losing focus
	if(pNewWnd != NULL && pNewWnd->GetParent() == this)
		return;

	// repaint items that should change appearance
	if((GetStyle() & LVS_TYPEMASK) == LVS_REPORT)
		RepaintSelectedItems();
}

//***************************************************************
void CColorListCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CListCtrl::OnSetFocus(pOldWnd);
	
	// check if we are getting focus
	if(pOldWnd!=NULL && pOldWnd->GetParent() == this)
		return;

	// repaint items that should change appearance
	if((GetStyle() & LVS_TYPEMASK) == LVS_REPORT)
		RepaintSelectedItems();
}

//***************************************************************
void CColorListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    if (!lpDrawItemStruct) return;
    IsCreated = TRUE;
	CDC * pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CMyLParam *	pMyLParam = GetMyLParam(lpDrawItemStruct->itemID);
	CRect rcItem;

    if (!pMyLParam) 
		return;

    int x = 0;
   
    redrawmode redraw = R_NORMAL;

	// Get item image and state info
	LV_ITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = lpDrawItemStruct->itemID;
	lvi.iSubItem = 0;
	lvi.stateMask = 0xFFFF;		// get all state flags
	GetItem(&lvi);

	BOOL bHighlight = ((lvi.state & LVIS_DROPHILITED) ||
				            ((lvi.state & LVIS_SELECTED) && 
                                ((GetFocus() == this ) || (GetStyle() & LVS_SHOWSELALWAYS))
					        )
				        );

    BOOL bLostFocus = (GetFocus() != this);


    if ((lpDrawItemStruct->itemAction & ODA_FOCUS) == ODA_FOCUS)
    {   //the control gains or loses input focus
        if (bLostFocus) 
			redraw = R_SEL_NOFOCUS;
        else if ((lvi.state & LVIS_FOCUSED) == LVIS_FOCUSED) 
			redraw = R_FOCUS;
        else 
        {
            if (bHighlight) 
				redraw = R_SELECT;
            else 
				redraw = R_NORMAL;
        }
    }
    else if ((lpDrawItemStruct->itemAction & ODA_SELECT) == ODA_SELECT)
    {   //the selection status has changed
            if (bHighlight)
            {
                if (bLostFocus) 
					redraw = R_SEL_NOFOCUS;
				else 
					redraw = R_SELECT;
            }
            else redraw = R_NORMAL;
    }
    else
    {   //redraw the item
        if (bLostFocus)
        {
            if (bHighlight) 
				redraw = R_SEL_NOFOCUS;
			else
				redraw = R_NORMAL;
        }
        else 
        {
            if ((lvi.state & LVIS_FOCUSED) == LVIS_FOCUSED) 
				redraw = R_FOCUS;
            else 
            {
                if (bHighlight) 
					redraw = R_SELECT;
				else redraw = R_NORMAL;
            }
        }
    }

    CPen       *pOldPen;
    CPen        pen;
    CGdiObject *pOldBrush;

    switch(redraw)
    {

        case R_FOCUS:
            rcItem = GetFullCellRect(lpDrawItemStruct->itemID, TRUE);
            pDC->FillSolidRect(rcItem, ::GetSysColor(COLOR_ACTIVECAPTION));
            pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
            if (m_BigFocusRect)
            {
                pen.CreatePen( PS_DOT, 0, ::GetSysColor(COLOR_HIGHLIGHTTEXT) );
                pOldPen  =pDC->SelectObject(&pen);
                pOldBrush=pDC->SelectStockObject(NULL_BRUSH);
                pDC->Rectangle(rcItem);
                pDC->SelectObject(pOldPen);
                pDC->SelectObject(pOldBrush);
            }
            else pDC->DrawFocusRect(rcItem);
            for (x=0; x<m_columnSize; x++)
            {
                rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, FALSE);
				rcItem.left += 3;
                pDC->DrawText(CListCtrl::GetItemText(lpDrawItemStruct->itemID,x), rcItem, DT_SINGLELINE | DT_LEFT | DT_VCENTER );
            }
            break;
        case R_SELECT:
            rcItem=GetFullCellRect(lpDrawItemStruct->itemID, TRUE);
            pDC->FillSolidRect(rcItem, ::GetSysColor(COLOR_ACTIVECAPTION));
            pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
            for (x=0; x<m_columnSize; x++)
            {
                rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, FALSE);
				rcItem.left += 3;
                pDC->DrawText(CListCtrl::GetItemText(lpDrawItemStruct->itemID,x), rcItem, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
            }
            break;
        case R_SEL_NOFOCUS:
            rcItem=GetFullCellRect(lpDrawItemStruct->itemID, TRUE);
            pDC->FillSolidRect(rcItem, ::GetSysColor(COLOR_INACTIVECAPTION));
            pDC->SetTextColor(pMyLParam->GetTextColor(5));
            pen.CreatePen( PS_DOT, 0, ::GetSysColor(COLOR_INACTIVECAPTION) );
            pOldPen  =pDC->SelectObject(&pen);
            pOldBrush=pDC->SelectStockObject(NULL_BRUSH);
            if (!m_withSingleRect)
                pDC->Rectangle(rcItem);
            for (x=0; x<m_columnSize; x++)
            {
                rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, FALSE);
				rcItem.left += 3;
                pDC->DrawText(CListCtrl::GetItemText(lpDrawItemStruct->itemID,x), rcItem, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
            }
            pDC->SelectObject(pOldPen);
            pDC->SelectObject(pOldBrush);
            break;
        case R_NORMAL:
            for (x=0; x<m_columnSize; x++)
            {
	            rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, TRUE);
                pDC->FillSolidRect(rcItem, pMyLParam->GetBackColor(x));
                pDC->SetTextColor(pMyLParam->GetTextColor(x));
                if (m_withSingleRect)
                {
                    pOldBrush=pDC->SelectStockObject(NULL_BRUSH);
                    pDC->Rectangle(rcItem);
                    pDC->SelectObject(pOldBrush);
                    
                }
                rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, FALSE);
				rcItem.left += 3;
                pDC->DrawText(CListCtrl::GetItemText(lpDrawItemStruct->itemID,x), rcItem, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
            }
            break;
    }

    return;
}

void CColorListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);
    if (IsCreated) 
    {
        RedrawWindow();
    }
}

//////////////////////////////////////////////////////////////////////////

int CColorListCtrl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem)
{
	m_columnSize++;
	return CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
}

int CColorListCtrl::InsertItem(int nItem, LPCTSTR lpszItem)
{
	int ret = CListCtrl::InsertItem(nItem, lpszItem);
	if (ret != -1)
	{
		CMyLParam  * pColors = new CMyLParam(m_columnSize);
		CListCtrl::SetItemData(ret, (DWORD_PTR)pColors);
	}
	return ret;
}


BOOL CColorListCtrl::DeleteColumn(int nCol)
{
	if (nCol >= m_columnSize)
		return FALSE;
	if (GetItemCount() > 0)
		return FALSE;

	BOOL ret = CListCtrl::DeleteColumn(nCol);
	if (ret)
	{
		m_columnSize --;
		return TRUE;
	}
	return FALSE;
}

BOOL CColorListCtrl::SetItemColor(COLORREF crText, COLORREF crBack, int iRow, int iCol)
{
	CMyLParam *p = GetMyLParam(iRow);
    if (!p) 
		return FALSE;
    
	BOOL ret = p->SetItemColor(crText, crBack, iRow, iCol);
	if (ret)
		RedrawItems(iRow, iRow);
	return ret;
}

BOOL CColorListCtrl::DeleteAllColumn()
{
	DeleteAllItems();
	BOOL ret;
	for (int i=m_columnSize; i>0; i--)
	{
		ret = DeleteColumn(i-1);
		if (!ret)
			return FALSE;
	}
	return TRUE;
}
