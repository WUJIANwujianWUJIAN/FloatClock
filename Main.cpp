#include<iostream>
#include<windows.h>
#include<time.h>

using namespace std;


#define IDT_TIMER1 120

#define W 60
#define H 20

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	//MessageBoxA(NULL, "Happened", "", 0);
	switch (message)
	{
	case WM_CREATE:
	{
		CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 30, 73, 20, window, NULL, NULL, NULL);
		break;
	}

	case WM_TIMER:
	{

		InvalidateRect(window, NULL, TRUE);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(window, &ps);

		time_t rawtime;
		struct tm timeinf;

		time(&rawtime);
		localtime_s(&timeinf, &rawtime);

		wstring str =L"";
		wsprintf((LPWSTR)str.c_str(), L"%02d:%02d:%02d", timeinf.tm_hour, timeinf.tm_min, timeinf.tm_sec); //产生：" 3.142"
		//DrawText(hdc, (LPCWSTR)str.c_str(), 8, &ps.rcPaint, DT_CENTER| DT_SINGLELINE );
		TextOut(hdc, 7, 4, (LPCWSTR)str.c_str(), 8);
		EndPaint(window, &ps);
		break;
	}
	case WM_NCHITTEST:
	{
		//MessageBox(NULL, L"", L"", 0);
		LRESULT hit = DefWindowProc(window, message, wparam, lparam);
		if (hit == HTCLIENT)
		{
			hit = HTCAPTION;
		}
		return hit;
	}
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProcA(window, message, wparam, lparam);
	}
	return 0;
}

int WINAPI WinMain(
	HINSTANCE instance,
	HINSTANCE previous_instance,
	LPSTR cmd_line,
	int show_cmd
)
{
	WNDCLASSA window_class = { 0 };
	window_class.lpfnWndProc = WindowProc;
	window_class.lpszClassName = "d3d12test";
	window_class.hbrBackground = (HBRUSH)BACKGROUND_BLUE;
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.hIcon = LoadIcon(NULL, IDI_SHIELD);

	RegisterClassA(&window_class);

	RECT R = { 0, 0, W, H };
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	HWND hh = CreateWindowExW(
		WS_EX_TOPMOST,
		L"d3d12test",
		L"FloatClock",
		WS_POPUPWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		0, 0,
		NULL,instance
	);

	LONG lStyle = GetWindowLong(hh, GWL_STYLE);
	lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
	SetWindowLong(hh, GWL_STYLE, lStyle);

	SetTimer(hh,             // handle to main window 
		IDT_TIMER1,            // timer identifier 
		100,                // five-minute interval 
		(TIMERPROC)NULL);

	ShowWindow(hh, SW_SHOW);
	UpdateWindow(hh);

	MSG message = { 0 };
	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			Sleep(100);
		}
	}
