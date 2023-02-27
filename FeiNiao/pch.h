// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"
#include <string>
#include <string_view>
#include <map>
#include <vector>
#include <thread>
#include <memory>
#include <mutex>

using namespace std;

// win7 style
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

// editor
#pragma warning(push)
#pragma warning(disable: 26812)
#pragma warning(pop)
#include <ScintillaTypes.h>
#include <ScintillaCall.h>
#include <ScintillaMessages.h>
#include <ScintillaStructures.h>
#include <ILexer.h>
#include <Lexilla.h>
#include <SciLexer.h>

#include <Scintilla.h>

#define IDC_REGULAR_EXPRESSION          2000
#define ID_LANGUAGE_VBSCRIPT            32771
#define ID_OPTIONS_VIEW_LINENUMBERS     32771
#define ID_OPTIONS_SELECTION_MARGIN     32779
#define ID_OPTIONS_FOLD_MARGIN          32780
#define ID_OPTIONS_ADDMARKER            32781
#define ID_OPTIONS_DELETEMARKER         32782
#define ID_OPTIONS_FIND_NEXTMARKER      32783
#define ID_OPTIONS_FIND_PREVMARKER      32784
#define ID_INDICATOR_LINE               61446
#define IDS_LINE_INDICATOR              61447
#define ID_INDICATOR_STYLE              61448
#define IDS_STYLE_INDICATOR             61449
#define ID_INDICATOR_FOLD               61450
#define IDS_FOLD_INDICATOR              61451

#define WM_NOTINY_USER_DATA WM_USER + 101
#define MSG_SCRIPT_CHANGE   WM_USER + 102
#define MSG_SCRITP_START    WM_USER + 103
#define MSG_SCRITP_STOP     WM_USER + 104


#endif //PCH_H
