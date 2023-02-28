// CPageEditor.cpp: 实现文件
//

#include "pch.h"
#include "FeiNiao.h"
#include "CPageEditor.h"
#include "afxdialogex.h"
#include "./include/FileUtil.h"

using namespace bird2fish;
// CPageEditor 对话框

IMPLEMENT_DYNAMIC(CPageEditor, CDialog)

CPageEditor::CPageEditor(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_EDITOR, pParent)
{

}

CPageEditor::~CPageEditor()
{
}

void CPageEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageEditor, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPageEditor 消息处理程序


BOOL CPageEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_hLexilla = LoadLibrary("Lexilla.dll");
	if (m_hLexilla == nullptr)
	{
		AfxMessageBox(_T("can't load Lexilla.dll"));
		return FALSE;
	}

	m_hScintilla = LoadLibrary("Scintilla.dll");
	if (m_hScintilla == nullptr)
	{
		AfxMessageBox(_T("can't load Scintilla.dll"));
		return FALSE;
	}

	m_pCreateLexer = reinterpret_cast<Lexilla::CreateLexerFn>(GetProcAddress(m_hLexilla, LEXILLA_CREATELEXER));
	if (m_pCreateLexer == nullptr)
	{
		AfxMessageBox(_T("Could not find the Lexilla CreateLexer function"));
		return FALSE;
	}

	CRect rect;
	this->GetClientRect(&rect);
	CRect rt(0, 0, rect.Width() - 50, rect.Height() - 50);
	pEditor = NewEditor(IDS_CODE_EDITOR, rt, 0);

	Scintilla::ILexer5* iLex = m_pCreateLexer("lua");
	if (iLex == nullptr)
	{
		AfxMessageBox(_T("load scheme  error"));
	}
	pEditor->SetILexer(iLex);
	pEditor->setup();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


RobinEditor* CPageEditor::NewEditor(int nID, CRect rect, int nStyle)
{
	CString m_Caption;
	m_Caption.LoadString(nID); //取按钮标题

	RobinEditor* mpEditor = new RobinEditor();
	mpEditor->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rect, this, 0);
	mpEditor->SetParent(this);

	pEditor = mpEditor;

	return mpEditor;
}

BOOL CPageEditor::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pEditor)
		pEditor->OnNotify(wParam, lParam, pResult);
	return CDialog::OnNotify(wParam, lParam, pResult);
}


void CPageEditor::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CRect rect;
	GetClientRect(&rect);
	if (pEditor)
		pEditor->MoveWindow(&rect);
	// TODO: 在此处添加消息处理程序代码
}


void CPageEditor::OnSetScript(const string& filePath, const string& content)
{
	if (pEditor)
	{
		scriptPath = filePath;
		pEditor->SetText(content.c_str());
	}
}

void CPageEditor::OnSaveScript(const string& filePath)
{
	if (pEditor)
	{
		auto pos = pEditor->GetTextLength();
		string content = pEditor->GetText(pos);
		FileUtil::writeFile(content, filePath.c_str());
	}
}

void OnCopy(const string& str)
{
	if (OpenClipboard(NULL))
	{
		EmptyClipboard();
		size_t cbStr = (str.length() + 1) * sizeof(TCHAR);
		HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, cbStr);
		memcpy_s(GlobalLock(hData), cbStr, str.c_str(), cbStr);
		GlobalUnlock(hData);
		SetClipboardData(CF_TEXT, hData);
		CloseClipboard();
	}
}
string onPaste()
{
	string str;
	if (OpenClipboard(NULL))
	{
		// 获取剪贴板中的字符串数据
		HANDLE hData = GetClipboardData(CF_TEXT);
		if (hData != NULL)
		{
			LPCTSTR lpdata = (LPCTSTR)GlobalLock(hData);
			str = lpdata;
			GlobalUnlock(hData);
		}
		// 关闭剪贴板
		CloseClipboard();
	}
	return str;
}

BOOL CPageEditor::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	// 检查消息是否是键盘消息
	if (pMsg->message == WM_KEYDOWN)
	{
		// 检查是否按下了Ctrl + S
		if (GetKeyState(VK_CONTROL) & 0x8000)
		{
			if (pMsg->wParam == 'S')
			{
				auto pos = pEditor->GetTextLength();
				string content = pEditor->GetText(pos);
				FileUtil::writeFile(content, scriptPath.c_str());
				pEditor->MarkerDeleteAll( (int)Scintilla::MarkerOutline::HistoryModified);
			}
			else if (pMsg->wParam == 'C')
			{
				string str = pEditor->getSelection();
				OnCopy(str);
			}
			else if (pMsg->wParam == 'V')
			{
				string str = onPaste();
				pEditor->setPaste(str);
			}
			return TRUE; // 表示消息已经处理过了
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}








