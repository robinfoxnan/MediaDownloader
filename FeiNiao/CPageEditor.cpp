// CPageEditor.cpp: 实现文件
//

#include "pch.h"
#include "FeiNiao.h"
#include "CPageEditor.h"
#include "afxdialogex.h"


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


void CPageEditor::OnSetScript(const string& content)
{
	if (pEditor)
	{
		pEditor->SetText(content.c_str());
	}
}