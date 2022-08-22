#include"OneKey.h"
int main()
{
	system("color e0");
	HWND hwnd = GetConsoleWindow();
	SetWindowPos(hwnd, HWND_TOPMOST, 1640, 300, 300, 450, 0);
	//SetConsoleTitle(L"指令输入姬");	//C++ 14
	SetConsoleTitle("指令输入姬");  //gcc 4.9.2
	long dwNewLong = GetWindowLong(hwnd, GWL_EXSTYLE);
	dwNewLong |= WS_EX_TRANSPARENT | WS_EX_LAYERED;
	SetWindowLong(hwnd, GWL_STYLE,
		GetWindowLong(hwnd, GWL_STYLE) - WS_CAPTION);
	SetWindowLong(hwnd, GWL_EXSTYLE, dwNewLong);
	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 120, 2);
	OneKey::ShowHelp();
	HINSTANCE hK = GetModuleHandle(NULL);
	HHOOK g_Hook = SetWindowsHookEx(WH_KEYBOARD_LL, OneKey::KeyboardProc, hK, 0);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(g_Hook);
	return 0;
}
