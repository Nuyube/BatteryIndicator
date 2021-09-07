#include<Windows.h>
#include"Window.h"
#include"WinHelper.h"
#include"Graphics.h"

using namespace WINE;
using namespace WINE::WinHelper;
using namespace WINE::WINDOW;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow) {
	Window window;
	RECT clientRect;
	RECT windowRect;
	CreateDirectory("C:/ProgramData/Nuyube", NULL);
	int i = CreateDirectory("C:/ProgramData/Nuyube/BatteryIndicator", NULL);
	if (i != 0) {
		std::fstream f;
		f.open("C:/ProgramData/Nuyube/BatteryIndicator/settings.txt", std::ios::out);
		f << "#Battery Indicator Settings File\n"
			<< "#The first token on each line must remain the same.\n"
			<< "#Lines preceded with '#' are comments.\n"
			<< "#Crit < Warn < Low.\n"
			<< "CRIT 10% 0xFF0000\n"
			<< "WARN 15% 0xFF0088\n"
			<< "LOW 25% 0xFF00FF\n"
			<< "CHARGE 0x6666FF\n"
			<< "FULL 0x00FF00\n"
			<< "EMPTY 0x000000\n";
		f.close();
	}
	GetClientRect(GetDesktopWindow(), &clientRect);
	windowRect.right = clientRect.right;
	if (atoi(cmdLine) != 0 && atoi(cmdLine) <= clientRect.bottom) {
		windowRect.bottom = atoi(cmdLine);
	}
	else windowRect.bottom = 4;
	CreateClassEx("BatteryIndicatorWindowClass", DefaultWindowProc, 0);
	window.Create("BatteryIndicatorWindowClass", "Battery Indicator", WS_POPUP & (~WS_OVERLAPPED), { 0,0 }, { windowRect.right, windowRect.bottom }, 0, 0);
	window.Show(1);
	Graphics* graphics = new Graphics(window.handle, { windowRect.right, windowRect.bottom });
	MSG message;
	DisableProcessWindowsGhosting();
	while (true) {
		MessageLoop(message);
		if (IsExiting()) break;
		graphics->update();
		if (GetForegroundWindow() == window.handle && GetAsyncKeyState(VK_ESCAPE)) break;
		SetWindowPos(window.handle, HWND_TOPMOST, 0, 0, windowRect.right, windowRect.bottom, 0);

		Sleep(1000);
	}
	graphics->~Graphics();
	window.~Window();
	clientRect = { 0 };
	return message.wParam;
}