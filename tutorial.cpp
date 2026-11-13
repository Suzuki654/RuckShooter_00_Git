//=============================================================================
//
//チュートリアル画面の処理実装ファイル[tutorial.cpp]
//Author:鈴木哲汰
//
//=============================================================================
#include "tutorial.h"
#include "player.h"
#include "bg.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "effect.h"
#include "sound.h"
#include "fade.h"
#include "gun.h"
#include "ui.h"
#include "game.h"

//マクロ定義
#define NUM_TUTORIAL (3)        //タイトルの画像の数

#define TUTORIAL_RIGHT (800.0f) //チュートリアル画像右
#define TUTORIAL_LEFT (500.0f)  //チュートリアル画像左
#define TUTORIAL_TOP (200.0f)   //チュートリアル画像上
#define TUTORIAL_UNDER (400.0f) //チュートリアル画像下

#define SUCCESS_WIDTH  (100.0f)//成功画像の幅
#define SUCCESS_HEIGHT (100.0f)//成功画像の高さ

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTutorial[NUM_TUTORIAL] = {};//テクスチャーへのポインター
LPDIRECT3DTEXTURE9 g_pTextureTutorialSuccess = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;//頂点バッファへのポインタ
bool g_bTutorialPause = false;//ポーズ状態のON/OFF
bool g_bTutorialDispDebug = false;//デバック表示のON/OFF
bool g_bTutorialSuccess = false;//操作が成功したかどうか
bool g_bTutorialPower = false;//操作が成功したかどうか
int g_nTutorialStep = 0;//ステップ
int g_nTutorialTimer = 0;//タイマー
bool g_PushTutorial = false;
//=============================================================================
//チュートリアル画面の初期化処理
//=============================================================================
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();
	//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(pDevice,
		"data\\TEXTURE\\WASDkey.png",
		&g_pTextureTutorial[0]);
	//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(pDevice,
		"data\\TEXTURE\\SpaceKey.png",
		&g_pTextureTutorial[1]);
	//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(pDevice,
		"data\\TEXTURE\\Tutorial_01.png",
		&g_pTextureTutorial[2]);

	//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(pDevice,
		"data\\TEXTURE\\Success.png",
		&g_pTextureTutorialSuccess);

	g_PushTutorial = false;
	g_bTutorialPause = false;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(sizeof(VERTEX_2D) * 4 * (NUM_TUTORIAL + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);


	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTutorial = 0; nCntTutorial < NUM_TUTORIAL; nCntTutorial++)
	{
		pVtx[0].pos = D3DXVECTOR3(TUTORIAL_LEFT, TUTORIAL_TOP, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(TUTORIAL_RIGHT, TUTORIAL_TOP, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(TUTORIAL_LEFT, TUTORIAL_UNDER, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(TUTORIAL_RIGHT, TUTORIAL_UNDER, 0.0f);

		// RHW
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 色
		pVtx[0].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].Col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 次の4頂点へ
		pVtx += 4;
	}
	// 成功画像用の4頂点
	pVtx[0].pos = D3DXVECTOR3(TUTORIAL_RIGHT - SUCCESS_WIDTH, TUTORIAL_UNDER - SUCCESS_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TUTORIAL_RIGHT, TUTORIAL_UNDER - SUCCESS_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TUTORIAL_RIGHT - SUCCESS_WIDTH, TUTORIAL_UNDER, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TUTORIAL_RIGHT, TUTORIAL_UNDER, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].Col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();

	//タイマーの初期化
	g_nTutorialTimer = 180;
	//ステップの初期化
	g_nTutorialStep = 0;


	//BGの初期化処理
	InitBG();
	//プレイヤーの初期化処理
	InitPlayer();
	//弾の初期化処理
	InitBullet();
	//銃の初期化処理
	InitGun();
	//エフェクトの初期化処理
	InitEffect();
	//爆発の初期化処理
	InitExplosion();
	//敵の初期化処理
	InitEnemy();
	//スコアの初期化処理
	InitScore();
	//UIの初期化処理
	InitUI();

	//サウンドの再生
	PlaySound(SOUND_LABEL_TUTORIAL000);

}
//=============================================================================
//チュートリアル画面の終了処理
//=============================================================================
void UninitTutorial(void)
{
	//頂点テクスチャーの破棄
	for (int nCntTutorial = 0; nCntTutorial < NUM_TUTORIAL; nCntTutorial++)
	{
		if (g_pTextureTutorial[nCntTutorial] != NULL)
		{
			g_pTextureTutorial[nCntTutorial]->Release();
			g_pTextureTutorial[nCntTutorial] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

	//頂点テクスチャーの破棄
	if (g_pTextureTutorialSuccess != NULL)
	{
		g_pTextureTutorialSuccess->Release();
		g_pTextureTutorialSuccess = NULL;
	}

	//サウンドの停止
	StopSound();

	//BGの終了処理
	UninitBG();
	//プレイヤーの終了処理
	UninitPlayer();
	//弾の終了処理
	UninitBullet();
	//銃の終了処理
	UninitGun();
	//エフェクトの終了処理
	UninitEffect();
	//爆発の終了処理
	UninitExplosion();
	//敵の終了処理
	UninitEnemy();
	//スコアの終了処理
	UninitScore();
	//UIの終了処理
	UninitUI();
}
//=============================================================================
//チュートリアル画面の更新処理
//=============================================================================
void UpdateTutorial(void)
{
	Player* pPlayer = GetPlayer();
	//ポーズ処理
	if (g_bTutorialPause == false)
	{

		//BGの更新処理
		UpdateBG();
		//プレイヤーの更新処理
		UpdatePlayer();
		//弾の更新処理
		UpdateBullet();
		//銃の更新処理
		UpdateGun();
		//エフェクトの更新処理
		UpdateEffect();
		//爆発の更新処理
		UpdateExplosion();
		//敵の更新処理
		UpdateEnemy();
		//スコアの更新処理
		UpdateScore();
		//UIの更新処理
		UpdateUI();

		// タイマー処理
		if (g_bTutorialSuccess)
		{
			g_nTutorialTimer--;

			if (g_nTutorialTimer <= 0)
			{
				g_bTutorialSuccess = false;
				g_nTutorialStep++;
			}
			//成功中は操作を受けない
			return;
		}
		else
		{
			//チュートリアルステップ
			switch (g_nTutorialStep)
			{
			case 0:
				// WASD操作ができた？
				if (GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_UP) || GetJoypadPress(JOYKEY_DOWN) || GetJoypadPress(JOYKEY_LEFT) || GetJoypadPress(JOYKEY_RIGHT) || GetJoypadLeftStickX() || GetJoypadLeftStickY())
				{
					// 操作成功
					g_bTutorialSuccess = true;
					PlaySound(SOUND_LABEL_SE_SUCCSESS);
					g_nTutorialTimer = 240; // 4秒
				}
				break;
			case 1:
				//SPACEで撃った？
				if (pPlayer->fPower > 30.0f)
				{
					g_bTutorialPower = true;
				}
				if (g_bTutorialPower && (GetKeyboardRelease(DIK_SPACE) || GetJoypadRelease(JOYKEY_A)))
				{
					//操作成功
					g_bTutorialSuccess = true;
					PlaySound(SOUND_LABEL_SE_SUCCSESS);
					g_nTutorialTimer = 180; // 3秒

					g_bTutorialPower = false;
				}


				break;
			case 2:
				//反動で移動した？
				if (pPlayer->fPower > 70.0f)
				{
					g_bTutorialPower = true;
				}
				if (g_bTutorialPower && (GetKeyboardRelease(DIK_SPACE) || GetJoypadRelease(JOYKEY_A)))
				{
					//操作成功
					g_bTutorialSuccess = true;
					PlaySound(SOUND_LABEL_SE_SUCCSESS);
					g_nTutorialTimer = 180; // 3秒

					g_bTutorialPower = false;
				}

				break;
			case 3:
				// チュートリアル終了
				if (GetFade() == FADE_NONE)
				{
					SetFade(MODE_GAME);
				}
				break;
			}
		}
		

	}
	if (GetKeyboardTrigger(DIK_P) || GetKeyboardTrigger(DIK_ESCAPE)|| GetJoypadTrigger(JOYKEY_START))
	{   //Pキーを押した
		g_bTutorialPause = !g_bTutorialPause;
		PlaySound(SOUND_LABEL_SE_DECISION000);
	}

	// チュートリアルスキップ
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_BACK))
	{
		if (GetFade() == FADE_NONE)
		{
			SetFade(MODE_GAME);
		}
		PlaySound(SOUND_LABEL_SE_DECISION000);
	}

	//ポーズ画面
	if (g_bTutorialPause == true)
	{
		// Wキーで上
		if (GetKeyboardRepeat(DIK_W) || GetKeyboardRepeat(DIK_UP) || GetJoypadRepeat(JOYKEY_UP) || GetJoypadLeftStickUpRepeat())
		{
			g_nGameEndSelect--;

			if (g_nGameEndSelect < 0)
			{
				g_nGameEndSelect = 2;
			}
			PlaySound(SOUND_LABEL_SE_CURSOR000);
		}

		// Sキーで下
		if (GetKeyboardRepeat(DIK_S) || GetKeyboardRepeat(DIK_DOWN) || GetJoypadRepeat(JOYKEY_DOWN) || GetJoypadLeftStickDownRepeat())
		{
			g_nGameEndSelect++;

			if (g_nGameEndSelect > 2)
			{
				g_nGameEndSelect = 0;
			}
			PlaySound(SOUND_LABEL_SE_CURSOR000);
		}

		if (g_PushTutorial == false)
		{
			//決定
			if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A))
			{
				switch (g_nGameEndSelect)
				{
				case 0:
					//リザルト
					SetFade(MODE_RESULT);
					g_PushTutorial = true;
					break;

				case 1:
					//リトライ
					SetFade(MODE_GAME);
					g_PushTutorial = true;
					break;

				case 2:
					//タイトルに戻る
					SetFade(MODE_TITLE);
					g_PushTutorial = true;
					break;
				}
				PlaySound(SOUND_LABEL_SE_DECISION000);
			}
		}
		
	}

}


//=============================================================================
//チュートリアル画面の描画処理
//=============================================================================
void DrawTutorial(void)
{
	//BGの描画処理
	DrawBG();
	//プレイヤーの描画処理
	DrawPlayer();
	//弾の描画処理
	DrawBullet();
	//銃の描画処理
	DrawGun();
	//エフェクトの描画処理
	DrawEffect();
	//爆発の描画処理
	DrawExplosion();
	//敵の描画処理
	DrawEnemy();
	//スコアの描画処理
	DrawScore();
	//UIの描画処理
	DrawUI();
	


	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	// チュートリアル画像
	if (g_nTutorialStep < NUM_TUTORIAL)
	{
		//テクスチャーの設定
		pDevice->SetTexture(0, g_pTextureTutorial[g_nTutorialStep]);

		// 成功したら暗くする
		VERTEX_2D* pVtx;
		//頂点バッファをロックし、頂点データポインタを取得
		g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += g_nTutorialStep * 4;
		if (g_bTutorialSuccess)
		{
			// 暗くする
			pVtx[0].Col = D3DCOLOR_RGBA(100, 100, 100, 255);
			pVtx[1].Col = D3DCOLOR_RGBA(100, 100, 100, 255);
			pVtx[2].Col = D3DCOLOR_RGBA(100, 100, 100, 255);
			pVtx[3].Col = D3DCOLOR_RGBA(100, 100, 100, 255);
		}
		else
		{
			// 通常
			pVtx[0].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		//頂点バッファをアンロック
		g_pVtxBuffTutorial->Unlock();
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, g_nTutorialStep * 4, 2);
	}

	// 成功画像
	if (g_bTutorialSuccess)
	{
		//テクスチャーの設定
		pDevice->SetTexture(0, g_pTextureTutorialSuccess);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, NUM_TUTORIAL * 4, 2);


	}

	if (g_bTutorialPause == true)
	{
		DrawGamePause();
	}
	//チュートリアルUIの描画処理
	DrawTutorialUI();
}

