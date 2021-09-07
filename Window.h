#pragma once
#include<Windows.h>
#include"WinHelper.h"
 
namespace WINE {
	namespace WINDOW {
		void CreateClassEx(const char* className, WNDPROC winproc, HINSTANCE hInstance);
		void CreateClassEx(const char* className, WNDPROC winproc, HINSTANCE hInstance, UINT cb, UINT styl, HCURSOR cur, HBRUSH hbrBG);
		class Window {
		public:
			WNDCLASSEX clas;
			HWND handle;
			HINSTANCE handleinstance;
			Window();
			Window(HWND h);
			Window(HWND h, HINSTANCE hi);
			~Window();
			void Show(int flags);
			HWND CreateEx(const char* lpszClassName, const char* WindowName, unsigned long style, POINT location, POINT size, HINSTANCE hInstance, int show);
			HWND Create(const char* lpszClassName, const char* WindowName, unsigned long style, POINT location, POINT size, HINSTANCE hInstance, int show);
			HWND Create(const char* lpszClassName, const char* WindowName, unsigned long style, POINT location, POINT size, HINSTANCE hInstance);
			HWND Create(const char* lpszClassName, const char* WindowName, unsigned long style, HINSTANCE hInstance, int show);
			HWND Create(const char* lpszClassName, const char* WindowName, unsigned long style, HINSTANCE hInstance);
			HWND Create(const char* lpszClassName, const char* WindowName, HINSTANCE hInstance, int show);
			HWND Create(const char* lpszClassName, const char* WindowName, HINSTANCE hInstance);
			HWND Create(const char* lpszClassName, const char* WindowName, int show);
			HWND Create(const char* lpszClassName, const char* WindowName);
			void SetStyle(LONG Style);
			void SetStyleEx(LONG Style);
		};
	}
}
