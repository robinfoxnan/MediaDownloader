#include "pch.h"
#include "RobinEditor.h"


/* XPM */
/* XPM */
static const char* minus_xpm[] = {
	/* width height num_colors chars_per_pixel */
	"     9     9       16            1",
	/* colors */
	"` c #8c96ac",
	". c #c4d0da",
	"# c #daecf4",
	"a c #ccdeec",
	"b c #eceef4",
	"c c #e0e5eb",
	"d c #a7b7c7",
	"e c #e4ecf0",
	"f c #d0d8e2",
	"g c #b7c5d4",
	"h c #fafdfc",
	"i c #b4becc",
	"j c #d1e6f1",
	"k c #e4f2fb",
	"l c #ecf6fc",
	"m c #d4dfe7",
	/* pixels */
	"hbc.i.cbh",
	"bffeheffb",
	"mfllkllfm",
	"gjkkkkkji",
	"da`````jd",
	"ga#j##jai",
	"f.k##k#.a",
	"#..jkj..#",
	"hemgdgc#h"
};

/* XPM */
static const char* plus_xpm[] = {
	/* width height num_colors chars_per_pixel */
	"     9     9       16            1",
	/* colors */
	"` c #242e44",
	". c #8ea0b5",
	"# c #b7d5e4",
	"a c #dcf2fc",
	"b c #a2b8c8",
	"c c #ccd2dc",
	"d c #b8c6d4",
	"e c #f4f4f4",
	"f c #accadc",
	"g c #798fa4",
	"h c #a4b0c0",
	"i c #cde5f0",
	"j c #bcdeec",
	"k c #ecf1f6",
	"l c #acbccc",
	"m c #fcfefc",
	/* pixels */
	"mech.hcem",
	"eldikille",
	"dlaa`akld",
	".#ii`ii#.",
	"g#`````fg",
	".fjj`jjf.",
	"lbji`ijbd",
	"khb#idlhk",
	"mkd.ghdkm"
};

void RobinEditor::DefineMarker(int marker, Scintilla::MarkerSymbol markerType, COLORREF fore, COLORREF back)
{

	this->MarkerDefine(marker, markerType);
	this->MarkerSetFore(marker, fore);
	this->MarkerSetBack(marker, back);
}

void RobinEditor::SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char* face)
{
	this->StyleSetFore(style, fore);
	this->StyleSetBack(style, back);
	if (size >= 1)
		this->StyleSetSize(style, size);
	if (face)
		this->StyleSetFont(style, face);
}


void RobinEditor::setup()
{

	const TCHAR* luaKeyWords0
	{ _T("and break do else elseif ")
		_T("end for function goto if in ")
			_T("local not or repeat ")
		_T("return then until while ")
	};

	const TCHAR* luaKeyWords1
	{ _T("HttpHeader  HttpClient")
	};

	const TCHAR* luaKeyWords2
	{ _T("isNull isNumber isString isArray")
	};

	/*const TCHAR* g_cppKeyWords
	{ _T("alignas alignof and and_eq asm atomic_cancel atomic_commit atomic_noexcept auto bitand bitor bool break ")
	 _T("case catch char char8_t char16_t char32_t class compl concept const consteval constexpr constinit const_cast continue ")
	 _T("co_await co_return co_yield decltype default delete do double dynamic_cast else enum explicit export extern false float for ")
	 _T("friend goto if inline int long mutable namespace new noexcept not not_eq nullptr ")
	 _T("operator or or_eq private protected public ")
	 _T("reflexpr register reinterpret_cast requires return short signed sizeof static static_assert static_cast struct switch synchronized ")
	 _T("template this thread_local throw true try typedef typeid typename union unsigned using ")
	 _T("virtual void volatile wchar_t while xor xor_eq") };

	 */
	this->SetKeyWords(0, luaKeyWords0);
	this->SetKeyWords(1, luaKeyWords1);
	this->SetKeyWords(2, luaKeyWords2);


	//_setmbcp(932);

	UINT codepage = _getmbcp(); // GetConsoleOutputCP();
	this->SetCodePage(codepage);


	//Setup styles
	SetAStyle(static_cast<int>(Scintilla::StylesCommon::Default), RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 11, "Verdana");

	

	this->StyleClearAll();
	SetAStyle(SCE_C_DEFAULT, RGB(0, 0, 0));
	SetAStyle(SCE_C_COMMENT, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTLINE, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTDOC, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTLINEDOC, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTDOCKEYWORD, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTDOCKEYWORDERROR, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_NUMBER, RGB(0, 0x80, 0x80));
	SetAStyle(SCE_C_WORD, RGB(0, 0, 0x80));
	this->StyleSetBold(SCE_C_WORD, 1);
	SetAStyle(SCE_C_STRING, RGB(0x80, 0, 0x80));
	SetAStyle(SCE_C_IDENTIFIER, RGB(0, 0xaa, 0));
	SetAStyle(SCE_C_PREPROCESSOR, RGB(0x80, 0, 0));
	SetAStyle(SCE_C_OPERATOR, RGB(0x80, 0x80, 0));

	SetAStyle(SCE_LUA_DEFAULT, RGB(0, 0, 0));
	SetAStyle(SCE_LUA_COMMENT, RGB(0, 0x80, 0));
	SetAStyle(SCE_LUA_COMMENTLINE, RGB(0, 0x80, 0));
	SetAStyle(SCE_LUA_COMMENTDOC, RGB(0, 0x80, 0));
	SetAStyle(SCE_LUA_NUMBER, RGB(0, 0x80, 0x80));
	SetAStyle(SCE_LUA_WORD, RGB(0, 0, 0xFF));
	this->StyleSetBold(SCE_LUA_WORD, 1);
	SetAStyle(SCE_LUA_STRING, RGB(0x80, 0, 0x80));
	SetAStyle(SCE_LUA_IDENTIFIER, RGB(0xaa, 0, 0));
	SetAStyle(SCE_LUA_PREPROCESSOR, RGB(0x80, 0, 0));
	SetAStyle(SCE_LUA_OPERATOR, RGB(0x80, 0x80, 0));

	SetAStyle(SCE_LUA_WORD2, RGB(0, 0, 0x80));
	SetAStyle(SCE_LUA_WORD3, RGB(0, 0, 0x80));
	SetAStyle(SCE_LUA_WORD4, RGB(0, 0, 0x80));
	SetAStyle(SCE_LUA_WORD5, RGB(0, 0, 0x80));
	SetAStyle(SCE_LUA_WORD6, RGB(0, 0, 0x80));
	SetAStyle(SCE_LUA_WORD7, RGB(0, 0, 0x80));
	SetAStyle(SCE_LUA_WORD8, RGB(0, 0, 0x80));

	//Setup folding
	this->SetMarginTypeN(0, MarginType::Number);
	this->SetMarginWidthN(0, 45);
	this->SetMarginSensitiveN(0, TRUE);
	SetAStyle(static_cast<int>(Scintilla::StylesCommon::LineNumber), RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 8, "Verdana");

	this->SetMarginTypeN(2, Scintilla::MarginType::Symbol);
	this->SetMarginWidthN(2, 20);
	this->SetMarginSensitiveN(2, TRUE);
	this->SetMarginMaskN(2, Scintilla::MaskFolders);
	this->SetSCIProperty(_T("fold"), _T("1"));
	// 设置页边显示的文本颜色-红色(注意颜色值顺序是BGR，而不是RGB，要不然颜色就反了)
	this->StyleSetFore(STYLE_LINENUMBER, RGB(0x00, 0x77, 0xAA));

	// 设置页边背景色-绿色
	//mpEditor->StyleSetBack(STYLE_LINENUMBER, 0x0000FF00);

	//Setup markers
	/*DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderOpen), Scintilla::MarkerSymbol::Minus, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0xFF));
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::Folder), Scintilla::MarkerSymbol::Plus, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderSub), Scintilla::MarkerSymbol::Empty, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderTail), Scintilla::MarkerSymbol::Empty, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderEnd), Scintilla::MarkerSymbol::Empty, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderOpenMid), Scintilla::MarkerSymbol::Empty, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderMidTail), Scintilla::MarkerSymbol::Empty, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));*/

	
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::HistoryModified), Scintilla::MarkerSymbol::Circle, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0xFF));

	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderOpen), Scintilla::MarkerSymbol::Pixmap, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0xFF));
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::Folder), Scintilla::MarkerSymbol::Pixmap, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderSub), Scintilla::MarkerSymbol::VLine, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));	
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderEnd), Scintilla::MarkerSymbol::Empty, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderTail), Scintilla::MarkerSymbol::LCornerCurve, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));  // end

	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderOpenMid), Scintilla::MarkerSymbol::Pixmap, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderMidTail), Scintilla::MarkerSymbol::LCornerCurve, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));  // mid end



	MarkerDefinePixmap(static_cast<int>(Scintilla::MarkerOutline::Folder), (const char *)plus_xpm);
	MarkerDefinePixmap(static_cast<int>(Scintilla::MarkerOutline::FolderOpen), (const char*)minus_xpm);
	//MarkerDefinePixmap(static_cast<int>(Scintilla::MarkerOutline::FolderTail), (const char*)plus_xpm);
	MarkerDefinePixmap(static_cast<int>(Scintilla::MarkerOutline::FolderOpenMid), (const char*)minus_xpm);

	//Setup auto completion
	this->AutoCSetSeparator(10); //Use a separator of line feed

	//Setup call tips
	this->SetMouseDwellTime(1000);

	//Enable Multiple selection
	this->SetMultipleSelection(TRUE);

	//this->SetIndent(4);
	//this->SetWrapStartIndent(4);
	//this->SetWrapIndentMode(WrapIndentMode::Indent);

	this->SetTabWidth(4);
	this->SetUseTabs(true);
	// 索引指引虚线
	this->SetIndentationGuides(Scintilla::IndentView::LookBoth);

}


BOOL RobinEditor::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
#pragma warning(suppress: 26490)
	const NotifyHeader* pNMHdr{ reinterpret_cast<NotifyHeader*>(lParam) };
#pragma warning(suppress: 26496)
	AFXASSUME(pNMHdr != nullptr);

	//Is it a notification from the embedded control
#pragma warning(suppress: 26489)
	if (pNMHdr->hwndFrom == this->GetSafeHwnd())
	{
#pragma warning(suppress: 26490)
		auto pSCNotification{ reinterpret_cast<NotificationData*>(lParam) };
		switch (pNMHdr->code)
		{
//		case Notification::StyleNeeded:
//		{
//#pragma warning(suppress: 26486)
//			OnStyleNeeded(pSCNotification);
//			break;
//		}
		case Notification::CharAdded:
		{
#pragma warning(suppress: 26486)
			OnCharAdded(pSCNotification);
			break;
		}
		case Notification::SavePointReached:
		{
#pragma warning(suppress: 26486)
			OnSavePointReached(pSCNotification);
			break;
		}
//		case Notification::SavePointLeft:
//		{
//#pragma warning(suppress: 26486)
//			OnSavePointLeft(pSCNotification);
//			break;
//		}
//		case Notification::ModifyAttemptRO:
//		{
//#pragma warning(suppress: 26486)
//			OnModifyAttemptRO(pSCNotification);
//			break;
//		}
//		case Notification::DoubleClick:
//		{
//#pragma warning(suppress: 26486)
//			OnDoubleClick(pSCNotification);
//			break;
//		}
		case Notification::UpdateUI:
		{
#pragma warning(suppress: 26486)
			OnUpdateUI(pSCNotification);
			break;
		}
//		case Notification::Modified:
//		{
//#pragma warning(suppress: 26486)
//			OnModified(pSCNotification);
//			break;
//		}
//		case Notification::MacroRecord:
//		{
//#pragma warning(suppress: 26486)
//			OnMacroRecord(pSCNotification);
//			break;
//		}
		case Notification::MarginClick:
		{
#pragma warning(suppress: 26486)
			OnMarginClick(pSCNotification);
			break;
		}
		case Notification::MarginRightClick:
		{
#pragma warning(suppress: 26486)
			OnMarginRightClick(pSCNotification);
			break;
		}
//		case Notification::NeedShown:
//		{
//#pragma warning(suppress: 26486)
//			OnNeedShown(pSCNotification);
//			break;
//		}
//		case Notification::Painted:
//		{
//#pragma warning(suppress: 26486)
//			OnPainted(pSCNotification);
//			break;
//		}
//		case Notification::UserListSelection:
//		{
//#pragma warning(suppress: 26486)
//			OnUserListSelection(pSCNotification);
//			break;
//		}
//		case Notification::DwellStart:
//		{
//#pragma warning(suppress: 26486)
//			OnDwellStart(pSCNotification);
//			break;
//		}
//		case Notification::DwellEnd:
//		{
//#pragma warning(suppress: 26486)
//			OnDwellEnd(pSCNotification);
//			break;
//		}
//		case Notification::Zoom:
//		{
//#pragma warning(suppress: 26486)
//			OnZoom(pSCNotification);
//			break;
//		}
//		case Notification::HotSpotClick:
//		{
//#pragma warning(suppress: 26486)
//			OnHotSpotClick(pSCNotification);
//			break;
//		}
//		case Notification::HotSpotDoubleClick:
//		{
//#pragma warning(suppress: 26486)
//			OnHotSpotDoubleClick(pSCNotification);
//			break;
//		}
//		case Notification::HotSpotReleaseClick:
//		{
//#pragma warning(suppress: 26486)
//			OnHotspotReleaseClick(pSCNotification);
//			break;
//		}
//		case Notification::IndicatorClick:
//		{
//#pragma warning(suppress: 26486)
//			OnIndicatorClick(pSCNotification);
//			break;
//		}
//		case Notification::IndicatorRelease:
//		{
//#pragma warning(suppress: 26486)
//			OnIndicatorRelease(pSCNotification);
//			break;
//		}
//		case Notification::CallTipClick:
//		{
//#pragma warning(suppress: 26486)
//			OnCallTipClick(pSCNotification);
//			break;
//		}
//		case Notification::AutoCSelection:
//		{
//#pragma warning(suppress: 26486)
//			OnAutoCSelection(pSCNotification);
//			break;
//		}
//		case Notification::AutoCCancelled:
//		{
//#pragma warning(suppress: 26486)
//			OnAutoCCancelled(pSCNotification);
//			break;
//		}
//		case Notification::AutoCCharDeleted:
//		{
//#pragma warning(suppress: 26486)
//			OnAutoCCharDeleted(pSCNotification);
//			break;
//		}
//		case Notification::FocusIn:
//		{
//#pragma warning(suppress: 26486)
//			OnFocusIn(pSCNotification);
//			break;
//		}
//		case Notification::FocusOut:
//		{
//#pragma warning(suppress: 26486)
//			OnFocusOut(pSCNotification);
//			break;
//		}
//		case Notification::AutoCCompleted:
//		{
//#pragma warning(suppress: 26486)
//			OnAutoCCompleted(pSCNotification);
//			break;
//		}
//		case Notification::AutoCSelectionChange:
//		{
//#pragma warning(suppress: 26486)
//			OnAutoCSelectionChange(pSCNotification);
//			break;
//		}
		default:
		{
			break;
		}
		}
		return TRUE; // we processed the message
	}
	else
	{
		//let the base class do its thing
		return __super::OnNotify(wParam, lParam, pResult);
	}

//	return CScintillaCtrl::OnNotify(wParam, lParam, pResult);
}

void RobinEditor::OnSavePointReached(_Inout_ NotificationData* pSCNotification)
{

}
void RobinEditor::OnMarginClick(_Inout_ NotificationData* pSCNotification)
{
	//By default get the line where the click occurred and toggle its fold state

	const auto nLine{ this->LineFromPosition(pSCNotification->position) };
	this->ToggleFold(nLine);
}

#pragma warning(suppress: 26440)
void RobinEditor::OnMarginRightClick(_Inout_ NotificationData* /*pSCNotification*/)
{
	//By default do nothing, derived classes may want to do something
}

// 这里设置自动补全功能
void RobinEditor::OnCharAdded(_Inout_ Scintilla::NotificationData* pSCNotification)
{
	//UNREFERENCED_PARAMETER(pSCNotification);

	lstChar = pSCNotification->ch;
	//std::string str = pSCNotification->text;

	auto pos = GetCurrentPos();
	auto line = LineFromPosition(pos);
	//auto  lineLength = LineLength(line);

	MarkerAdd(line, (int)Scintilla::MarkerOutline::HistoryModified);


	//Get the previous 13 characters and if it is "scintilla is " case insensitive
	//then display a list which allows "very cool", "easy" and "way cool!!"
	const Scintilla::Position nStartSel{ this->GetSelectionStart() };
	const Scintilla::Position nEndSel{ this->GetSelectionEnd() };
	if ((nStartSel == nEndSel) && (nStartSel >= 2))
	{
		Scintilla::TextRangeFull tr{};
		tr.chrg.cpMin = nStartSel - 1;
		tr.chrg.cpMax = nEndSel;
		CStringA sText;
		tr.lpstrText = sText.GetBuffer(1);
		this->GetTextRangeFull(&tr);
		sText.ReleaseBuffer();

		//Display the auto completion list
		if (sText.CompareNoCase(":") == 0)
		{
			//Display the auto completion list
			//rCtrl.AutoCSetOrder(Scintilla::Ordering::PerformSort);
			//int nOrder{rCtrl.AutoCGetOrder()};
			this->AutoCShow(0, _T("isNull()\nisString()\nisNumber()"));
		}
	}

	//As another example, get the previous 2 characters and if it is "res" case sensitive
	//then display a list which allows "resize", "restart" and "restore"
	if ((nStartSel == nEndSel) && (nStartSel >= 3))
	{
		Scintilla::TextRangeFull tr{};
		tr.chrg.cpMin = nStartSel - 3;
		tr.chrg.cpMax = nEndSel;
		CStringA sText;
		tr.lpstrText = sText.GetBuffer(3);
		GetTextRangeFull(&tr);
		sText.ReleaseBuffer();

		//Display the auto completion list
		if (sText == "res")
		{
			//Display the auto completion list
			AutoCShow(3, _T("resize\nrestart\nrestore"));
		}
	}

	//As another example, get the previous 16 characters and if it is "SAMPLE_INDICATOR" case insensitive
	//then display that text with a squiggly underline
	if ((nStartSel == nEndSel) && (nStartSel >= 16))
	{
		Scintilla::TextRangeFull tr{};
		tr.chrg.cpMin = nStartSel - 16;
		tr.chrg.cpMax = nEndSel;
		CStringA sText;
		tr.lpstrText = sText.GetBuffer(16);
		GetTextRangeFull(&tr);
		sText.ReleaseBuffer();

		//Display the Sample indicator
		if (sText.CompareNoCase("sample_indicator") == 0)
		{
			//For demonstration purposes lets make the "C" Comment style into hotspots which turn red
			SetIndicatorCurrent(0);
			StyleSetHotSpot(SCE_C_COMMENT, TRUE);
			SetHotspotActiveFore(TRUE, RGB(255, 0, 0));

			//Show the indicator
#pragma warning(suppress: 26472)
			IndicatorFillRange(tr.chrg.cpMin, static_cast<int>(nStartSel));
		}
	}
}

// 计算自动缩进，最好设置使用tab
void RobinEditor::OnUpdateUI(_Inout_ NotificationData* pSCNotification)
{
	auto pos = GetCurrentPos();
	auto line = LineFromPosition(pos);
	auto  lineLength = LineLength(line);

	//MarkerAdd(line, (int)Scintilla::MarkerOutline::HistoryModified);

	if (lstChar == '\r' || lstChar == '\n')
	{
		if (line > 0 && lineLength <= 2)
		{
			auto  prevLineLength = LineLength(line - 1);

			CString str = this->GetLine(line - 1);

			int i = 0;
			for (i = 0; i < str.GetLength(); i++)
			{
				if (str[i] != ' ' && str[i] != '\t')
					break;
			}
			if (i > 0)
			{

				str = str.Left(i);
				this->ReplaceSel(str);
			}
		}
		lstChar = 0;
	}// end \r\n
}

string RobinEditor::getSelection()
{
	CString str = GetSelText();
	return (LPCTSTR)str;
	
}

void   RobinEditor::setPaste(const string& str)
{
	ReplaceSel(str.c_str());
}

