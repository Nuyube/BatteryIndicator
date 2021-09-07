#pragma once
#include<Windows.h>
#define Point(x,y) {x,y}
namespace WINE {
	namespace WinHelper {
		POINT GetDesktopSize();
		POINT GetDesktopPoint();
		void MessageLoop(MSG& msg);
		bool IsExiting();
		void Exit();
		LRESULT CALLBACK DefaultWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	}
}