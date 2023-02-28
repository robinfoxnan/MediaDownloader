// CSettingDialog.cpp: 实现文件
//

#include "pch.h"
#include "FeiNiao.h"
#include "CSettingDialog.h"
#include "afxdialogex.h"
#include "GlobalData.h"

// CSettingDialog 对话框

IMPLEMENT_DYNAMIC(CSettingDialog, CDialogEx)

CSettingDialog::CSettingDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SETTING, pParent)
{

}

CSettingDialog::~CSettingDialog()
{
}

void CSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DIR, mDirEdit);
}


BEGIN_MESSAGE_MAP(CSettingDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BROSWER, &CSettingDialog::onBrowseDir)
	ON_BN_CLICKED(IDOK, &CSettingDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CSettingDialog 消息处理程序


void CSettingDialog::onBrowseDir()
{
	char buffer[256];
	BROWSEINFOA    bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = buffer;
	bi.lpszTitle = "选择路径";
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	bi.lpfn = NULL;
	bi.iImage = 0;
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);
	if (pIDList)
	{
		SHGetPathFromIDList(pIDList, buffer);
		//取得文件夹路径
		//m_listPath = buffer;
		mDirEdit.SetWindowTextA(buffer);

	}
	UpdateData(FALSE);
}


BOOL CSettingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	string dir = GlobalData::instance().getDefaultDir();
	mDirEdit.SetWindowText(dir.c_str());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSettingDialog::OnBnClickedOk()
{
	CString dir;
	mDirEdit.GetWindowText(dir);
	GlobalData::instance().setDefaultDir((LPCTSTR)dir);
	CDialogEx::OnOK();
}
