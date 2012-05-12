#include <windows.h>

#include "engine.hpp"

#include "../pong/resource.h"

#include <nil/ini.hpp>

int const window_width = 320;
int const window_height = 240;

int const fps = 100;

engine game;
bool run = true;

LRESULT CALLBACK window_procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
		{
			game.create_dc(GetDC(hWnd));

			int
				resolution_x = GetSystemMetrics(SM_CXSCREEN),
				resolution_y = GetSystemMetrics(SM_CYSCREEN);
			RECT window_rectangle;
			GetWindowRect(hWnd, &window_rectangle);
			int
				width = window_rectangle.right - window_rectangle.left,
				height = window_rectangle.bottom - window_rectangle.top;

			SetWindowPos(hWnd, HWND_TOP, (resolution_x - width) / 2, (resolution_y - height) / 2, 0, 0, SWP_NOSIZE);
			break;
		}

		case WM_ERASEBKGND:
			return 1;

		case WM_PAINT:
			game.process_frame(hWnd);
			break;

		/*
		case WM_TIMER:
			game.invalidate(hWnd);
			break;

		case WM_ERASEBKGND:
			return 1;

		case WM_PAINT:
			try
			{
				game_mutex.lock();
				game.process_frame(hWnd);
				game_mutex.unlock();
			}
			catch(std::exception const & exception)
			{
				MessageBox(hWnd, (std::string("The following exception has occured:\n\n\t") + exception.what() + "\n\nTerminating program.").c_str(), "Exception", MB_OK | MB_ICONERROR);
				run_timer = false;
				PostQuitMessage(WM_QUIT);
			}
			break;
			*/

		case WM_CLOSE:
		case WM_DESTROY:
			run = false;
			PostQuitMessage(WM_QUIT);
			break;

		case WM_LBUTTONDOWN:
			return DefWindowProc(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);

		case WM_KEYDOWN:
			game.process_key_down(wParam);
			break;

		case WM_KEYUP:
			game.process_key_up(wParam);
			break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char const * class_name = "Pong";
	char const * window_title = "Adolf Hitler Pong";

	WNDCLASSEX window_class;
	window_class.cbSize = sizeof(window_class);
	window_class.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	window_class.lpfnWndProc = &window_procedure;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = hInstance;
	window_class.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HITLER));
	window_class.hCursor = LoadCursor(0, IDC_ARROW);
	window_class.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	window_class.lpszMenuName = 0;
	window_class.lpszClassName = class_name;
	window_class.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HITLER));
	RegisterClassEx(&window_class);

	HWND window_handle = CreateWindowEx(0, class_name, window_title, WS_POPUP | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, window_width, window_height, 0, 0, hInstance, 0);
	//SetTimer(window_handle, 0, 1000 / fps, 0);

	std::string arguments(lpCmdLine);
	if(!arguments.empty())
	{
		if(arguments[0] == '"' && arguments.length() >= 2)
		{
			arguments.erase(arguments.begin());
			arguments.erase(arguments.end() - 1);
		}
		bool success = game.load_demo(arguments);
		if(!success)
		{
			MessageBox(window_handle, "Unable to load demo file", "Demo playback error", MB_OK | MB_ICONERROR);
			SendMessage(window_handle, WM_DESTROY, 0, 0);
			return 0;
		}
	}
	else
		game.start_game();

	nil::ini ini;
	if(ini.load("pong.ini"))
	{
		game.set_demo_level_limit(ini.number<unsigned>("demo_level_limit", 3));
		SetThreadPriority(GetCurrentThread(), ini.number<int>("thread_priority", 0));
	}

	LARGE_INTEGER frequency_struct;
	QueryPerformanceFrequency(&frequency_struct);
	unsigned long long divisor = frequency_struct.QuadPart / fps;
	unsigned long long last_state = 0ull;
	while(run)
	{
		MSG message;
		while(PeekMessage(&message, window_handle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		unsigned long long current_state = counter.QuadPart / divisor;
		if(last_state < current_state)
		{
			game.invalidate(window_handle);
			game.process_frame(window_handle);
			last_state = current_state;
		}
		//Sleep(20);
	}

	return 0;
}
