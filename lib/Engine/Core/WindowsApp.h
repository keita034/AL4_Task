#pragma once
#include"ErrorException.h"

class WindowsApp
{
private:
	WNDCLASSEX wndclassex{};
	HWND hwnd;
	MSG massege{};//メッセージ
	int windowWidth = 1280;//横幅
	int windowHeight = 720;//縦幅



public:
	//シングルトンインスタンスの取得
	static WindowsApp* GetInstance();

	/// <summary>
	/// ウィンドウ生成
	/// </summary>
	/// <param name="title">タイトル(ワイド文字)</param>
	/// <param name="width">ウィンドウ横幅</param>
	/// <param name="height">ウィンドウ横幅</param>
	void CreatWindow(const wchar_t* title = L"DirectXGame", int width = 1280, int height = 720);
	bool MessageWindow();
	void Break();

	//セッター
	void SetW(WNDCLASSEX W);
	void SetHwnd(HWND Hwnd);

	//ゲッター
	WNDCLASSEX GetW();
	HWND GetHwnd();
	int GetWindowHeight();
	int GetWindowWidth();

private:
	//コンストラクタ・デストラクタ
	WindowsApp() = default;
	~WindowsApp() = default;
};