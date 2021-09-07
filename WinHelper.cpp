#include "WinHelper.h"
namespace WINE {
	namespace WinHelper {
		bool exiting = false;
		void MessageLoop(MSG& m) {
			while (PeekMessage(&m, 0, 0, 0, PM_REMOVE)) {
				TranslateMessage(&m);
				DispatchMessage(&m);
			}
		}
		bool IsExiting() {
			return exiting;
		}
		void Exit() {
			exiting = true;
		}
		POINT GetDesktopSize() {
			RECT r;
			GetClientRect(GetDesktopWindow(), &r);
			return Point(r.right - r.left, r.bottom - r.top);
		}
		POINT GetDesktopPoint() {
			RECT r;
			GetClientRect(GetDesktopWindow(), &r);
			return Point(r.left, r.top);
		}
		LRESULT CALLBACK DefaultWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
			if (message == WM_DESTROY)
			{
				PostQuitMessage(0);
				exiting = true;
				return 0;
			}
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
}