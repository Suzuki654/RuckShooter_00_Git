//=============================================================================
//
//タイトル画面の処理実装ファイル[title.cpp]
//Author:鈴木哲汰
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "bg.h"
#include "score.h"

#define NUM_TITLE (7)//タイトルの画像の数
//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTitle[NUM_TITLE] = {};//テクスチャーへのポインター
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;//頂点バッファへのポインタ
int g_nSelectTitle = 0;// 0 = スタート// 1 = 終わる
int g_nTitleBlinkCount = 0;
bool g_nPushTitle = false;
//=============================================================================
//タイトル画面の初期化
//=============================================================================
void InitTitle(void)
{
	int nCntTitle;
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();
	//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(pDevice,
		"data\\TEXTURE\\BG03.png",
		&g_pTextureTitle[0]);
	//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(pDevice,
		"data\\TEXTURE\\BG03_03.png",
		&g_pTextureTitle[1]);
	//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(pDevice,
		"data\\TEXTURE\\RyuckShooter_Title.png",
		&g_pTextureTitle[2]);
	//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(pDevice,
		"data\\TEXTURE\\Start.png",
		&g_pTextureTitle[3]);
	//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(pDevice,
		"data\\TEXTURE\\Ranking_Title.png",
		&g_pTextureTitle[4]);
	//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(pDevice,
		"data\\TEXTURE\\End.png",
		&g_pTextureTitle[5]);
	
	//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(pDevice,
		"data\\TEXTURE\\Select_Accept.png",
		&g_pTextureTitle[6]);

	g_nPushTitle = false;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(sizeof(VERTEX_2D) * 4 * NUM_TITLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{
		if (nCntTitle == 0)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		}
		if (nCntTitle == 1)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(GAME_BG_LEFT, GAME_BG_TOP, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(GAME_BG_RIGHT, GAME_BG_TOP, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(GAME_BG_LEFT, GAME_BG_UNDER, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(GAME_BG_RIGHT, GAME_BG_UNDER, 0.0f);
		}
		if (nCntTitle == 2)
		{
			// タイトルロゴ
			pVtx[0].pos = D3DXVECTOR3(438.0f, 50.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(838.0f, 50.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(438.0f, 200.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(838.0f, 200.0f, 0.0f);
		}
		if (nCntTitle == 3)
		{
			// スタート
			pVtx[0].pos = D3DXVECTOR3(500.0f, 300.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(780.0f, 300.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(500.0f, 380.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(780.0f, 380.0f, 0.0f);
		}
		if (nCntTitle == 4)
		{
			//ランキング
			pVtx[0].pos = D3DXVECTOR3(500.0f, 400.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(780.0f, 400.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(500.0f, 480.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(780.0f, 480.0f, 0.0f);
		}
		if (nCntTitle == 5)
		{
			// 終わる
			pVtx[0].pos = D3DXVECTOR3(500.0f, 500.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(780.0f, 500.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(500.0f, 580.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(780.0f, 580.0f, 0.0f);
		}
		if (nCntTitle == 6)
		{
			//操作方法
			pVtx[0].pos = D3DXVECTOR3(500.0f, 600.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(780.0f, 600.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(500.0f, 680.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(780.0f, 680.0f, 0.0f);
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//頂点カラーの設定
		pVtx[0].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//テクスチャー座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをあんろっくする
	g_pVtxBuffTitle->Unlock();

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM000);
}
//=============================================================================
//タイトル画面の終了処理
//=============================================================================
void UninitTitle(void)
{
	//サウンドの停止
	StopSound();
	int nCntTitle;
	for (nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{
		//テクスチャーの破棄
		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}
//=============================================================================
//タイトル画面の更新処理
//=============================================================================
void UpdateTitle(void)
{
	int nCntTitle;
	SHORT stickY = GetJoypadLeftStickY();

	// Wで上を選択
	if (GetKeyboardRepeat(DIK_W) || GetKeyboardRepeat(DIK_UP) || GetJoypadRepeat(JOYKEY_UP) || GetJoypadLeftStickUpRepeat())
	{
		g_nSelectTitle--;

		if (g_nSelectTitle < 0)
		{
			g_nSelectTitle = 2;
		}
		//SE
		PlaySound(SOUND_LABEL_SE_CURSOR000);
	}

	// Sで下を選択
	if (GetKeyboardRepeat(DIK_S) || GetKeyboardRepeat(DIK_DOWN) || GetJoypadRepeat(JOYKEY_DOWN) || GetJoypadLeftStickDownRepeat())
	{
		g_nSelectTitle++;

		if (g_nSelectTitle > 2)
		{
			g_nSelectTitle = 0;
		}
		//SE
		PlaySound(SOUND_LABEL_SE_CURSOR000);
	}

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	g_nTitleBlinkCount++;

	if (g_nTitleBlinkCount >= 30)
	{
		g_nTitleBlinkCount = 0;
	}

	for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{
		if (nCntTitle == 3 || nCntTitle == 4 || nCntTitle == 5)
		{
			if ((nCntTitle == 3 && g_nSelectTitle == 0) || (nCntTitle == 4 && g_nSelectTitle == 1) || (nCntTitle == 5 && g_nSelectTitle == 2))
			{
				if (g_nPushTitle == false)
				{
					// 選択中
					if ((g_nTitleBlinkCount / 15) % 2 == 0)
					{
						// 明るい色
						pVtx[0].Col = D3DCOLOR_RGBA(255, 255, 0, 255);
						pVtx[1].Col = D3DCOLOR_RGBA(255, 255, 0, 255);
						pVtx[2].Col = D3DCOLOR_RGBA(255, 50, 0, 255);
						pVtx[3].Col = D3DCOLOR_RGBA(255, 50, 0, 255);
					}
					else
					{
						// 暗い色
						pVtx[0].Col = D3DCOLOR_RGBA(100, 100, 100, 255);
						pVtx[1].Col = D3DCOLOR_RGBA(100, 100, 100, 255);
						pVtx[2].Col = D3DCOLOR_RGBA(100, 100, 100, 255);
						pVtx[3].Col = D3DCOLOR_RGBA(100, 100, 100, 255);
					}
				}
				else
				{
					// 選択中
					if ((g_nTitleBlinkCount / 5) % 2 == 0)
					{
						// 明るい色
						pVtx[0].Col = D3DCOLOR_RGBA(255, 255, 0, 255);
						pVtx[1].Col = D3DCOLOR_RGBA(255, 255, 0, 255);
						pVtx[2].Col = D3DCOLOR_RGBA(255, 50, 0, 255);
						pVtx[3].Col = D3DCOLOR_RGBA(255, 50, 0, 255);
					}
					else
					{
						// 暗い色
						pVtx[0].Col = D3DCOLOR_RGBA(100, 100, 100, 255);
						pVtx[1].Col = D3DCOLOR_RGBA(100, 100, 100, 255);
						pVtx[2].Col = D3DCOLOR_RGBA(100, 100, 100, 255);
						pVtx[3].Col = D3DCOLOR_RGBA(100, 100, 100, 255);
					}
				}
				
			}
			else
			{
				//選択されていない
				pVtx[0].Col = D3DCOLOR_RGBA(150, 150, 180, 255);
				pVtx[1].Col = D3DCOLOR_RGBA(150, 150, 180, 255);
				pVtx[2].Col = D3DCOLOR_RGBA(150, 150, 180, 255);
				pVtx[3].Col = D3DCOLOR_RGBA(150, 150, 180, 255);
			}
		}

		// 次の画像の頂点へ
		pVtx += 4;
	}

	g_pVtxBuffTitle->Unlock();


	if (GetKeyboardTrigger(DIK_ESCAPE))
	{
		PostQuitMessage(0);
	}


	//Spaceで決定
	if (g_nPushTitle == false)
	{
		if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A))
		{
			if (g_nSelectTitle == SELECT_START)
			{

				//スタート
				SetFade(MODE_TUTORIAL);
				g_nPushTitle = true;
			}
			if (g_nSelectTitle == SELECT_RANKING)
			{
				//ランキング
				SetFade(MODE_RESULT);
				g_nPushTitle = true;
			}
			else if (g_nSelectTitle == SELECT_END)
			{
				//終わる
				PostQuitMessage(0);
			}
			PlaySound(SOUND_LABEL_SE_DECISION000);
		}
	}
	
}
//=============================================================================
//タイトル画面の描画処理
//=============================================================================
void DrawTitle(void)
{
	int nCntTitle;
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);



	for (nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{
		//テクスチャーの設定
		pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
	}

}