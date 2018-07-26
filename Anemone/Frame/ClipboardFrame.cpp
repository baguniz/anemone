#include "stdafx.h"
#include "ClipboardFrame.h"

ClipboardFrame::ClipboardFrame()
{
	bFirst = true;
	bStart = false;

	m_mode = CLIPBOARD_MODE;
	advTitle = L"(Clipboard Mode)";
	typeNum = 0;
	strText = L"�Ƴ׸�� v2.0 by ������";
}

ClipboardFrame::~ClipboardFrame()
{
}

bool ClipboardFrame::OnRender()
{
	context->clear(SK_ColorTRANSPARENT);
	if (!typeNum)
	{
		lastTime = system_clock::now();
		typeNum++;
	}
	
	if (typeNum <= strText.length())
	{
		milli_duration duration = system_clock::now() - lastTime;
		OutputDebugString(std::to_wstring(duration.count()).c_str());
		if (duration.count() > 30)
		{
			strBuff = strText.substr(0, typeNum++);
			lastTime = system_clock::now();
		}
	}

	LayeredWindow::OnRender();
	DrawSysMenu();
	DrawContent();
	return true;
}

bool ClipboardFrame::OnCreate()
{
	BaseFrame::OnCreate();

	HWND hwndNextViewer = SetClipboardViewer(handle);
	return true;
}

bool ClipboardFrame::OnChangeCbChain(WPARAM wParam, LPARAM lParam)
{
	if ((HWND)wParam == hwndNextViewer)
		hwndNextViewer = (HWND)lParam;

	else if (hwndNextViewer != NULL)
		SendMessage(hwndNextViewer, WM_CHANGECBCHAIN, wParam, lParam);

	return true;
}

bool ClipboardFrame::OnDrawClipboard()
{
	bool success = false;
	int nRetryCount = 0;

	while (nRetryCount < 5)
	{
		if (OpenClipboard(handle))
		{
			success = true;
			break;
		}
		nRetryCount++;
		Sleep(10);
	}

	if (!success)
	{
		wchar_t str[255];
		swprintf_s(str, L"cannot open clipboard: %d\n", GetLastError());
		OutputDebugString(str);
		return false;
	}

	auto hglb = GetClipboardData(CF_UNICODETEXT);
	auto pchData = reinterpret_cast<wchar_t *>(GlobalLock(hglb));
	if (bFirst)
	{
		bFirst = false;
	}
	else
	{
		bStart = true;
		strText = pchData;
	}
	typeNum = 0;
	GlobalUnlock(hglb);
	CloseClipboard();
	return true;
}

bool ClipboardFrame::OnDestroy()
{
	ChangeClipboardChain(handle, hwndNextViewer);
	return BaseFrame::OnDestroy();
}