//インクルード
#include <Windows.h>
#include <tchar.h>
#include "Engine/Direct3D.h"
#include "Engine/Camera.h"
#include "Engine/Transform.h"
#include"RootJob.h"

//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")

//定数宣言
const wchar_t* WIN_CLASS_NAME = L"SampleGame";  //ウィンドウクラス名
const wchar_t* APP_NAME = L"サンプルゲーム";
const int WINDOW_WIDTH = 800;  //ウィンドウの幅
const int WINDOW_HEIGHT = 600; //ウィンドウの高さ

RootJob* pRootJob = nullptr;

//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//エントリーポイント
//アプリケーション　プログラミング　インターフェース = api
//何らかのアプリケーションを作る際に便利機能をまとめたもの
//ソフトウェア　デベロップメント　キット = sdk
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	HRESULT hr;
	//ウィンドウクラス（設計図）を作成
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);             //この構造体のサイズ
	wc.hInstance = hInstance;                   //インスタンスハンドル
	wc.lpszClassName = WIN_CLASS_NAME;            //ウィンドウクラス名
	wc.lpfnWndProc = WndProc;                   //ウィンドウプロシージャ
	wc.style = CS_VREDRAW | CS_HREDRAW;         //スタイル（デフォルト）
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //アイコン
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);   //小さいアイコン
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //マウスカーソル
	wc.lpszMenuName = NULL;                     //メニュー（なし）
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //背景（白）
	RegisterClassEx(&wc); //クラスを登録

	//ウィンドウサイズの計算(+（表示領域をWINDOW_WIDT*WINDOW_HEIGHT
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	int winW = winRect.right - winRect.left;     //ウィンドウ幅
	int winH = winRect.bottom - winRect.top;     //ウィンドウ高さ

	//ウィンドウを作成
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,         //ウィンドウクラス名
		APP_NAME,     //タイトルバーに表示する内容
		WS_OVERLAPPEDWINDOW, //スタイル（普通のウィンドウ）
		CW_USEDEFAULT,       //表示位置左（おまかせ）
		CW_USEDEFAULT,       //表示位置上（おまかせ）
		winW,                //ウィンドウ幅
		winH,                //ウィンドウ高さ
		NULL,                //親ウインドウ（なし）
		NULL,                //メニュー（なし）
		hInstance,           //インスタンス
		NULL                 //パラメータ（なし）
	);

	//ウィンドウを表示
	ShowWindow(hWnd, nCmdShow);

	//Direct3D初期化
	HRESULT hr = Direct3D::Initialize(winW, winH, hWnd);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"DirextXの作成に失敗", NULL, MB_OK);
		return 0;
	}

	Camera::Initialize();

	pRootJob = new RootJob;
	pRootJob->Initialize();

	if (FAILED(hr))
	{
		MessageBox(NULL, L"初期化に失敗", NULL, MB_OK);
		return 0;
	}

	//メッセージループ（何か起きるのを待つ）
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		//メッセージあり
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//メッセージなし
		else
		{//このカッコ内が１フレーム
			timeBeginPeriod(1);
			static DWORD startTime = timeGetTime();
			DWORD nowTime = timeGetTime();
			static DWORD lastUpdateTime = nowTime;
			static DWORD countFps = 0;
			timeEndPeriod(1);

			
			if (nowTime - startTime >= 1000)
			{
				std::wstring str;
				wsprintf(str.data(), L"%u", countFps);
				SetWindowTextW(hWnd, str.c_str());

				countFps = 0;
				startTime = nowTime;
			}
			if (nowTime - lastUpdateTime <= 1000.0f / 60.0f)
			{
				continue;//1/60秒立っていないのでスルー
			}
			//1/60秒立っていたので更新＝更新時間を今の時間に更新
			lastUpdateTime = nowTime;
			countFps++;

			//カメラの更新
			Camera::Update();
			//ルートジョブから
			//アップデートする
			pRootJob->Update();
			pRootJob->UpdateSub();
			//ゲームの処理
			Direct3D::BeginDraw();
			pRootJob->DrawSub();
			//描画処理
			Direct3D::EndDraw();

		}
	}
	pRootJob->Release();
	Direct3D::Release();
	
	return 0;
}

//ウィンドウプロシージャ（何かあった時によばれる関数）
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);  //プログラム終了
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
