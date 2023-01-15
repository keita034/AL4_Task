#include "WindowsApp.h"
#include"resource.h"
#include"DirectX12Core.h"

//ウインドウプロシージャ
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		//ウインドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WindowsApp* WindowsApp::GetInstance()
{
	//シングルトン
	static	WindowsApp windowsApp_;
	return &windowsApp_;
}

void WindowsApp::CreatWindow(const wchar_t* title,int width, int height)
{
	windowHeight = height;
	windowWidth = width;

	//ウインドウクラスの設定
	wndclassex.cbSize = sizeof(WNDCLASSEX);
	wndclassex.lpfnWndProc = (WNDPROC)WindowProc;//ウインドウプロシージャを設定
	wndclassex.lpszClassName = title;//ウインドウクラス名
	wndclassex.hInstance = GetModuleHandle(nullptr);//ウインドウハンドル
	wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル指定
	wndclassex.hIcon = LoadIcon(GetModuleHandle(nullptr), TEXT("ICO"));
	//ウインドウクラスをOSに登録する
	RegisterClassEx(&wndclassex);

	//ウインドウサイズ{X座標　Y座標 横幅　縦幅}
	RECT wrc = { 0,0,this->windowWidth,this->windowHeight };
	//自動でサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウインドウオブジェクトの生成
	hwnd = CreateWindow(wndclassex.lpszClassName,//クラス名
		title,				//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,		//標準的なウインドウスタイル
		CW_USEDEFAULT,				//表示X座標(OSに任せる)
		CW_USEDEFAULT,				//表示Y座標(OSに任せる)
		wrc.right - wrc.left,		//ウインドウ横幅
		wrc.bottom - wrc.top,		//ウインドウ縦幅
		nullptr,					//親ウインドウハンドル
		nullptr,					//メニューハンドル
		wndclassex.hInstance,				//呼び出しアプリケーションハンドル
		nullptr);					//オプション

	//ウインドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);

}

bool WindowsApp::MessageWindow()
{
	//メッセージがある？
	if (PeekMessage(&massege, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&massege); //キー入力メッセージの処理
		DispatchMessage(&massege); //プロシージャにメッセージを送る
	}

	//×ボタンで終了メッセージが来たらゲームループを抜ける
	if (massege.message == WM_QUIT)
	{
		//DirectX12Core::GetInstance()->GetDevice()->Re
		return false;
	}
	return true;
}

void WindowsApp::Break()
{
	//ウインドウクラスを登録解除
	UnregisterClass(wndclassex.lpszClassName, wndclassex.hInstance);
}

WNDCLASSEX WindowsApp::GetW()
{
	return wndclassex;
}

void WindowsApp::SetW(WNDCLASSEX W)
{
	wndclassex = W;
}

void WindowsApp::SetHwnd(HWND Hwnd)
{
	hwnd = Hwnd;
}

HWND WindowsApp::GetHwnd()
{
	return hwnd;
}

int WindowsApp::GetWindowHeight()
{
	return windowHeight;
}

int WindowsApp::GetWindowWidth()
{
	return windowWidth;
}
