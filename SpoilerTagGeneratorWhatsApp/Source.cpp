#include <windows.h>
#include <iostream>

int main()
{
	std::wstring s = L"[SPOILER AHEAD]\n";
	wchar_t wc = L'\u200Bc';

	for (int i = 0; i < 6000; i++)
		s = s + wc;

	const wchar_t* cwc = s.data();
	const size_t len = wcslen(cwc) + 1;

	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(hMem), cwc, len);
	GlobalUnlock(hMem);
	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, hMem);
	CloseClipboard();

	return 0;
}