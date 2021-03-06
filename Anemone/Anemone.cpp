/**
* This file is part of Anemone.
*
* Anemone is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* The Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Anemone is distributed in the hope that it will be useful,
*
* But WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Anemone.
*
*   If not, see <http://www.gnu.org/licenses/>.
*
**/

#include "stdafx.h"
#include "Anemone.h"
#include "BaseFrame.h"
#include "CaptionFrame.h"
#include "ClipboardFrame.h"
#include "TranslateFrame.h"
#include "HookerFrame.h"
#include "SkGraphics.h"
#include "KeyboardHook.h"
#include "MouseHook.h"
#include "gitparams.h"

#define MAX_LOADSTRING 100
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
ConfigManager CfgMgr;

int text_speed = 25;
int guide_opacity = 0;
double guide_opacity2 = 0;
bool mouse_evt = false;

WCHAR g_AnemoneInfo[100];
WCHAR g_Version[100];
WCHAR g_BuildInfo[100];

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ANEMONE, szWindowClass, MAX_LOADSTRING);

	LockSetForegroundWindow(LSFW_LOCK);

	// init skia
	SkGraphics::Init();

	KeyboardHook kh;
	MouseHook mh;

	// install hooks
	kh.Install();
	mh.Install();

	swprintf_s(g_Version, WIDEN(ANEMONE_VER));
	swprintf_s(g_BuildInfo, L"%s - %s", WIDEN(TXT(GIT_CUR_COMMIT)), WIDEN(__TIMESTAMP__));
	swprintf_s(g_AnemoneInfo, L"%s -- %s", g_Version, g_BuildInfo);

	while (1)
	{
		int nMode = DialogBox(nullptr, MAKEINTRESOURCE(IDD_MAIN), 0, MainDlgProc);
		switch (nMode)
		{
		case ID_CAPTIONMODE:
		{
			CaptionFrame win;
			win.Create();
			break;
		}
		case ID_CLIPMODE:
		{
			ClipboardFrame win;
			win.Create();
			break;
		}
		case ID_TRANSMODE:
		{
			TranslateFrame win;
			win.Create();
			break;
		}
		case ID_HOOKERMODE:
		{
			HookerFrame win;
			win.Create();
			break;
		}
		case ID_SETTING:
			DialogBox(nullptr, MAKEINTRESOURCE(IDD_SETTING), 0, SettingDlgProc);
			break;

		case 0x12345678:
			LockSetForegroundWindow(LSFW_UNLOCK);
			return 0;
		}
	}
	return 0;
}

INT_PTR CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);

		MoveWindow(hWnd, (cx - width) / 2, (cy - height) / 2, width, height, false);
		SetWindowText(hWnd, g_AnemoneInfo);
		break;
	}
	case WM_SHOWWINDOW:
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case ID_CAPTIONMODE:
		case ID_CLIPMODE:
		case ID_TRANSMODE:
		case ID_HOOKERMODE:
		case ID_SETTING:
			EndDialog(hWnd, wmId);
			break;
		}
		break;
	}
	case WM_CLOSE:
		EndDialog(hWnd, 0x12345678);
		break;
	}
	return 0;
}

INT_PTR CALLBACK SettingDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int lastSection = 0;

	auto SectionProc = [](HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);
		switch (message)
		{
		case WM_INITDIALOG:
		{
			EnumChildWindows(hDlg, [](HWND hWnd, LPARAM lParam) -> BOOL
			{
				HWND hDlg = (HWND)lParam;

				switch (GetDlgCtrlID(hWnd))
				{
				case IDC_CFG_TEXT_SPEED:
				{
					SendMessage(hWnd, TBM_SETRANGEMAX, 1, 100);
					SendMessage(hWnd, TBM_SETRANGEMIN, 1, 0);
					SendMessage(hWnd, TBM_SETPOS, 1, text_speed);
					break;
				}
				case IDC_CFG_GROUP_TEXT_SPEED:
				{
					wchar_t str[255];
					LoadStringW(nullptr, GetDlgCtrlID(hWnd) + 10000, str, 255);
					auto text = fmt::format(L"{0} ({1})", str, text_speed);
					SetDlgItemText(hDlg, GetDlgCtrlID(hWnd), text.c_str());
					break;
				}
				default:
				{
					wchar_t str[255];
					CheckDlgButton(hDlg, GetDlgCtrlID(hWnd), true);
					LoadStringW(nullptr, GetDlgCtrlID(hWnd) + 10000, str, 255);
					if (str[0])
						SetWindowText(hWnd, str);
					break;
				}
				}
				return true;
			}
			, (LPARAM)hDlg);
			return (INT_PTR)TRUE;
		}
		case WM_HSCROLL:
			if (GetDlgItem(hDlg, IDC_CFG_TEXT_SPEED) == (HWND)lParam)
			{
				switch (LOWORD(wParam))
				{
				case TB_LINEUP:
				case TB_LINEDOWN:
				case TB_PAGEUP:
				case TB_PAGEDOWN:
				case TB_TOP:
				case TB_BOTTOM:
				case TB_ENDTRACK:
				case TB_THUMBTRACK:
				{
					text_speed = static_cast<int>((LOWORD(wParam) == TB_THUMBTRACK) ?
						HIWORD(wParam) :
						SendDlgItemMessage(hDlg, IDC_CFG_TEXT_SPEED, TBM_GETPOS, 0, 0));
					
					wchar_t str[255];
					LoadStringW(nullptr, IDC_CFG_GROUP_TEXT_SPEED + 10000, str, 255);
					auto text = fmt::format(L"{0} ({1})", str, text_speed);
					SetDlgItemText(hDlg, IDC_CFG_GROUP_TEXT_SPEED, text.c_str());
					break;
				}
				}
			}
			break;

		case WM_VSCROLL:
			break;
		case WM_MOUSEWHEEL:
			break;
		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
		}
		return (INT_PTR)FALSE;
	};

	auto InitSection = [&](HWND hWnd, HWND *hDlg)
	{
		for (int i = 0; i < 10; i++)
		{
			*(hDlg + i) = CreateDialog(nullptr, MAKEINTRESOURCE(IDD_SETTING_GENERAL + i), hWnd, (DLGPROC)SectionProc);
			MoveWindow(hDlg[i], 130, 10, 520, 340, false);
		}
		ShowWindow(hDlg[lastSection], true);
	};

	auto SwitchSection = [](HWND *hDlg, int id)
	{
		for (int i = 0; i < 10; i++)
			if (id != i)
				ShowWindow(hDlg[i], false);

		ShowWindow(hDlg[id], true);
		lastSection = id;
	};

	static HWND hDlg[10] = { 0, };
	switch (message)
	{
	case WM_INITDIALOG:
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);

		MoveWindow(hWnd, (cx - width) / 2, (cy - height) / 2, width, height, false);
		InitSection(hWnd, hDlg);
		break;
	}
	case WM_SIZE:
		break;

	case WM_HSCROLL:
		break;

	case WM_VSCROLL:
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
			DestroyWindow(hWnd);
			break;
		case IDC_SECTION_GENERAL:
		case IDC_SECTION_TEXT:
		case IDC_SECTION_TRANSLATE:
		case IDC_SECTION_ETC:
			SwitchSection(hDlg, LOWORD(wParam) - 1000);
			break;
		}
	}
	break;
	case WM_CLOSE:
	case WM_QUIT:
		DestroyWindow(hWnd);
		break;
	}
	return 0;
}