#pragma once
#include <afxwin.h>
#include "ScintillaCtrl.h"

using namespace Scintilla;

class RobinEditor : public CScintillaCtrl
{
public:
	//Notifications
	//void OnStyleNeeded(_Inout_ NotificationData* pSCNotification);
	void OnCharAdded(_Inout_ NotificationData* pSCNotification);
	void OnSavePointReached(_Inout_ NotificationData* pSCNotification);
	/*void OnSavePointLeft(_Inout_ NotificationData* pSCNotification);
	void OnModifyAttemptRO(_Inout_ NotificationData* pSCNotification);
	void OnDoubleClick(_Inout_ NotificationData* pSCNotification);*/
	void OnUpdateUI(_Inout_ NotificationData* pSCNotification);
	/*void OnModified(_Inout_ NotificationData* pSCNotification);
	void OnMacroRecord(_Inout_ NotificationData* pSCNotification);*/
	void OnMarginClick(_Inout_ NotificationData* pSCNotification);
	void OnMarginRightClick(_Inout_ NotificationData* pSCNotification);
	/*void OnNeedShown(_Inout_ NotificationData* pSCNotification);
	void OnPainted(_Inout_ NotificationData* pSCNotification);
	void OnUserListSelection(_Inout_ NotificationData* pSCNotification);
	void OnDwellStart(_Inout_ NotificationData* pSCNotification);
	void OnDwellEnd(_Inout_ NotificationData* pSCNotification);
	void OnZoom(_Inout_ NotificationData* pSCNotification);
	void OnHotSpotClick(_Inout_ NotificationData* pSCNotification);
	void OnHotSpotDoubleClick(_Inout_ NotificationData* pSCNotification);
	void OnCallTipClick(_Inout_ NotificationData* pSCNotification);
	void OnAutoCSelection(_Inout_ NotificationData* pSCNotification);
	void OnIndicatorClick(_Inout_ NotificationData* pSCNotification);
	void OnIndicatorRelease(_Inout_ NotificationData* pSCNotification);
	void OnAutoCCharDeleted(_Inout_ NotificationData* pSCNotification);
	void OnAutoCCancelled(_Inout_ NotificationData* pSCNotification);
	void OnHotspotReleaseClick(_Inout_ NotificationData* pSCNotification);
	void OnFocusIn(_Inout_ NotificationData* pSCNotification);
	void OnFocusOut(_Inout_ NotificationData* pSCNotification);
	void OnAutoCCompleted(_Inout_ NotificationData* pSCNotification);
	void OnAutoCSelectionChange(_Inout_ NotificationData* pSCNotification);
	void OnChange();
	void OnScintillaSetFocus();
	void OnScintillaKillFocus();*/
	//BOOL PreTranslateMessage(MSG* pMsg);

	void SetAStyle(int style, COLORREF fore, COLORREF back = RGB(0xff, 0xff, 0xff), int size = -1, const char* face = nullptr);
	void DefineMarker(int marker, Scintilla::MarkerSymbol markerType, COLORREF fore, COLORREF back);
	string getSelection();
	void   setPaste(const string& str);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
//	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	void setup();
private:
	int lstChar = 0;
};

