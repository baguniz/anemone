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

#pragma region [ Includes ]
#include "stdafx.h"
#include "Anemone.h"
#include "MainDlg.h"
#include "SettingDlg.h"
#include "BaseFrame.h"
#include "CaptionFrame.h"
#include "ClipboardFrame.h"
#include "TranslateFrame.h"
#include "SkGraphics.h"
#pragma endregion

#pragma region [ Application Extern Variables ]
#define MAX_LOADSTRING 100
HINSTANCE g_hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
ConfigManager CfgMgr;
#pragma endregion

int text_speed = 25;
int guide_opacity = 0;
double guide_opacity2 = 0;
bool mouse_evt = false;

std::wstring strBuff;
std::vector<std::wstring> vecBuff;
int n_selLine = 0;

WCHAR g_AnemoneInfo[100];
WCHAR g_Version[100];
WCHAR g_BuildInfo[100];

#pragma region -- WinAPI Skeleton Functions --
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

	SkGraphics::Init();

	swprintf_s(g_Version, WIDEN(ANEMONE_VER));
	swprintf_s(g_BuildInfo, L"%s - %s", WIDEN(TXT(GIT_CUR_COMMIT)), WIDEN(__TIMESTAMP__));
	swprintf_s(g_AnemoneInfo, L"%s -- %s", g_Version, g_BuildInfo);

	g_hInst = hInstance;
	while (1)
	{
		int nMode = DialogBox(g_hInst, MAKEINTRESOURCE(IDD_MAIN), 0, MainDlg::DlgProc);
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
		case ID_SETTING:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_SETTING), 0, SettingDlg::DlgProc);
			break;

		default:
			return 0;
		}
	}
	LockSetForegroundWindow(LSFW_UNLOCK);
	return 0;
}

#pragma endregion