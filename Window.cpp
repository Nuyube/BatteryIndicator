#pragma once
#include<Windows.h>
#include"Window.h"
#include<exception>
namespace WINE {
	namespace WINDOW {

		//Constructors and Destructors

		//Initialize a window with a handle and instance.
		Window::Window(HWND h, HINSTANCE hInstance) {
			handle = h;
			handleinstance = hInstance;
		}

		//Initialize a window with a handle.
		Window::Window(HWND h) {
			handle = h;
			handleinstance = 0;
		}
		
		//Initialize a window with nothing.
		Window::Window() {
			handle = 0;
			handleinstance = 0;
		}

		//Reset the handle and handleinstance(hInstance) to zero.
		Window::~Window() {
			handle = 0;
			handleinstance = 0;
		}

		


		
		//Shows the window with int flags. Typically you'd use nCmdShow here.
		void Window::Show(int flags = 0) {
			ShowWindow(handle, flags);
		}
		//Create a window. The resulting handle is both returned and assigned to handle. It also runs ShowWindow with the 'show' variable.
		HWND Window::CreateEx(const char* lpszClassName, const char* WindowName, unsigned long style, POINT location, POINT size, HINSTANCE hInstance, int show) {
			handleinstance = hInstance;
			handle = CreateWindow(lpszClassName, WindowName, style, location.x, location.y, size.x, size.y, 0, 0, hInstance, 0);
			Show(show);
			return handle;
		}
		//Create a window. The resulting handle is both returned and assigned to handle. It also runs ShowWindow with the 'show' variable.
		HWND Window::Create(const char* lpszClassName, const char* WindowName, unsigned long style, POINT location, POINT size, HINSTANCE hInstance, int show) {
			handleinstance = hInstance;
			handle = CreateWindow(lpszClassName, WindowName, style, location.x, location.y, size.x, size.y, 0, 0, hInstance, 0);
			Show(show);
			return handle;
		}
		
		//Create a window. The resulting handle is both returned and assigned to handle.
		HWND Window::Create(const char* lpszClassName, const char* WindowName, unsigned long style, POINT location, POINT size, HINSTANCE hInstance) {
			handleinstance = hInstance;
			handle = CreateWindow(lpszClassName, WindowName, style, location.x, location.y, size.x, size.y, 0, 0, hInstance, 0);
			return handle;
		}

		//Create a window at location GetDesktopPoint (likely 0,0) with size GetDesktopSize. It also runs ShowWindow with the 'show' variable.
		HWND Window::Create(const char* lpszClassName, const char* WindowName, unsigned long style, HINSTANCE hInstance, int show) {
			handleinstance = hInstance;
			Create(lpszClassName, WindowName, style, WinHelper::GetDesktopPoint(), WinHelper::GetDesktopSize(), hInstance);
			Show(show);
			return handle;
		}

		//Create a window at location GetDesktopPoint (likely 0,0) with size GetDesktopSize.
		HWND Window::Create(const char* lpszClassName, const char* WindowName, unsigned long style, HINSTANCE hInstance) {
			handleinstance = hInstance;
			return Create(lpszClassName, WindowName, style, WinHelper::GetDesktopPoint(), WinHelper::GetDesktopSize(), hInstance);
		}

		//Create a window as a WS_OVERLAPPEDWINDOW at location GetDesktopPoint (likely 0,0) with size GetDesktopSize. It also runs ShowWindow with the 'show' variable.
		HWND Window::Create(const char* lpszClassName, const char* WindowName, HINSTANCE hInstance, int show) {
			handleinstance = hInstance;
			Create(lpszClassName, WindowName, WS_OVERLAPPEDWINDOW, WinHelper::GetDesktopPoint(), WinHelper::GetDesktopSize(), hInstance);
			Show(show);
			return handle;
		}

		//Create a window as a WS_OVERLAPPEDWINDOW at location GetDesktopPoint (likely 0,0) with size GetDesktopSize.
		HWND Window::Create(const char* lpszClassName, const char* WindowName, HINSTANCE hInstance) {
			handleinstance = hInstance;
			return Create(lpszClassName, WindowName, WS_OVERLAPPEDWINDOW, WinHelper::GetDesktopPoint(), WinHelper::GetDesktopSize(), hInstance);
		}

		//Create the window as a WS_OVERLAPPEDWINDOW at location GetDesktopPoint (likely 0,0) with size GetDesktopSize 
		//under handleinstance (which is saved if you've created a window with any function using it, or if you supplied it in the constructor.
		//It also runs ShowWindow with the 'show' variable.
		HWND Window::Create(const char* lpszClassName, const char* WindowName, int show) {
			if (handleinstance == 0) throw new std::exception("hInstance was null!");
			else Create(lpszClassName, WindowName, WS_OVERLAPPEDWINDOW, WinHelper::GetDesktopPoint(), WinHelper::GetDesktopSize(), handleinstance);
			Show(show);
			return handle;
		}

		//Create the window as a WS_OVERLAPPEDWINDOW at location GetDesktopPoint (likely 0,0) with size GetDesktopSize 
		//under handleinstance (which is saved if you've created a window with any function using it, or if you supplied it in the constructor.
		HWND Window::Create(const char* lpszClassName, const char* WindowName) {
			if (handleinstance == 0) throw new std::exception("hInstance was null!");
			else return Create(lpszClassName, WindowName, WS_OVERLAPPEDWINDOW, WinHelper::GetDesktopPoint(), WinHelper::GetDesktopSize(), handleinstance);
		}

		void Window::SetStyle(LONG Style)
		{
			SetWindowLong(handle, GWL_STYLE, Style);
		}

		void Window::SetStyleEx(LONG Style)
		{
			SetWindowLong(handle, GWL_EXSTYLE, Style);
		}


		//Create a class and register it automatically.
		void CreateClassEx(const char* className, WNDPROC winproc, HINSTANCE hInstance, UINT cb = 0, UINT styl = 0, HCURSOR cur = 0, HBRUSH hbrBG = 0) {
			WNDCLASSEX wc;
			ZeroMemory(&wc, sizeof(WNDCLASSEX));
			if (cb == 0) cb = sizeof(WNDCLASSEX);
			wc.cbSize = cb;
			
			if (styl == 0) styl = CS_HREDRAW | CS_VREDRAW;
			wc.style = styl;

			wc.lpfnWndProc = winproc;
			wc.hInstance = hInstance;
			
			if (cur == 0) cur = LoadCursor(NULL, IDC_ARROW);
			wc.hCursor = cur;

			if (hbrBG == 0) hbrBG = (HBRUSH)COLOR_WINDOW;
			wc.hbrBackground = hbrBG;

			wc.lpszClassName = className;
			RegisterClassEx(&wc);
		}

		//Create a class assuming 
		//style = CS_HREDRAW | CS_VREDRAW, 
		//cursor = LoadCursor(NULL, IDC_ARROW), 
		//cbSize = sizeof(WNDCLASSEX), 
		//hbrushBackground = (HBRUSH)COLOR_WINDOW.
		void CreateClassEx(const char* className, WNDPROC winproc, HINSTANCE hInstance) {
			CreateClassEx(className, winproc, hInstance, 0, 0, 0, 0);
		}

	}
}
