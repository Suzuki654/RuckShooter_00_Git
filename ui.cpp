//=============================================================================
//
//UIの処理実装ファイル[ui.cpp]
//Author:鈴木哲汰
//
//=============================================================================
#include "ui.h"
#include "player.h"
#include "game.h"
#include "boss.h"
#include "tutorial.h"
#include "sound.h"
//マクロ定義
#define NUM_UI (UI_TEX_MAX)//UIの数

#define HP_LEFT   (276.0f)
#define HP_TOP    (620.0f)
#define HP_RIGHT  (1000.0f)
#define HP_BOTTOM (670.0f)
#define HP_WIDTH  (680.0f)

#define BOSSHP_LEFT   (376.0f)
#define BOSSHP_TOP    (20.0f)
#define BOSSHP_RIGHT  (900.0f)
#define BOSSHP_BOTTOM (70.0f)
#define BOSSHP_WIDTH  (480.0f)

#define CHARGE_WIDTH   (15.0f)
#define CHARGE_HEIGHT  (100.0f)
#define CHARGE_OFFSET_X (35.0f)

// WAVE
#define WAVE_WIDTH  (300.0f)
#define WAVE_HEIGHT (80.0f)

//ゲーム終了画像
#define GAMEEND_WIDTH  (400.0f)
#define GAMEEND_HEIGHT (100.0f)

//プロトタイプ宣言
void InitHP(void);
void UpdateHP(void);
void DrawHP(void);

void InitCharge(void);
void UpdateCharge(void);
void DrawCharge(void);

void InitWave(void);
void DrawGameWave(int nWave);

void InitGameEnd(void);
void DrawGameEnd(void);

void InitBossHP(void);
void UpdateBossHP(void);
void DrawBossHP(void);

void InitGamePause(void);
void DrawGamePause(void);

void InitTutorialUI(void);
void DrawTutorialUI(void);

void InitGameUI(void);
void DrawGameUI(void);


//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureUI[NUM_UI] = {};//テクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI = NULL;//頂点バッファ
int g_nPatternUI = 0;
//=============================================================================
//UIの初期化処理
//=============================================================================
void InitUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//全体の頂点バッファ
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_UI,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUI,
		NULL
	);
	InitHP();
	InitCharge();
	InitWave();
	InitGameEnd();
	InitBossHP();
	InitGamePause();
	InitTutorialUI();
	InitGameUI();
}
//=============================================================================
//UIの終了処理
//=============================================================================
void UninitUI(void)
{
	for (int nCntUI = 0; nCntUI < NUM_UI; nCntUI++)
	{
		if (g_apTextureUI[nCntUI] != NULL)
		{
			g_apTextureUI[nCntUI]->Release();
			g_apTextureUI[nCntUI] = NULL;
		}
	}

	if (g_pVtxBuffUI != NULL)
	{
		g_pVtxBuffUI->Release();
		g_pVtxBuffUI = NULL;
	}
}
//=============================================================================
//UIの更新処理
//=============================================================================
void UpdateUI(void)
{
	UpdateHP();
	UpdateCharge();
	if (GetBoss()->bUse)
	{
		UpdateBossHP();
	}
}
//=============================================================================
//UIの描画処理
//=============================================================================
void DrawUI(void)
{
	DrawHP();
	DrawCharge();
	if (GetBoss()->bUse)
	{
		DrawBossHP();
	}
}
//=============================================================================
//HP_UIの初期化処理
//=============================================================================
void InitHP(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// テクスチャ読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\HP_Frame.png",
		&g_apTextureUI[UI_TEX_HP_FRAME]
	);
	// テクスチャ読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\HP_Gauge.png",
		&g_apTextureUI[UI_TEX_HP_GAUGE]
	);
	// テクスチャ読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\HP.png",
		&g_apTextureUI[UI_TEX_HP_NAME]
	);



	VERTEX_2D* pVtx;
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	// HPフレーム
	pVtx[UI_VTX_HP_FRAME + 0].pos = D3DXVECTOR3(HP_LEFT, HP_TOP, 0.0f);
	pVtx[UI_VTX_HP_FRAME + 1].pos = D3DXVECTOR3(HP_RIGHT, HP_TOP, 0.0f);
	pVtx[UI_VTX_HP_FRAME + 2].pos = D3DXVECTOR3(HP_LEFT, HP_BOTTOM, 0.0f);
	pVtx[UI_VTX_HP_FRAME + 3].pos = D3DXVECTOR3(HP_RIGHT, HP_BOTTOM, 0.0f);


	// HPゲージ
	pVtx[UI_VTX_HP_GAUGE + 0].pos = D3DXVECTOR3(HP_LEFT + 40, HP_TOP, 0.0f);
	pVtx[UI_VTX_HP_GAUGE + 1].pos = D3DXVECTOR3(HP_RIGHT, HP_TOP, 0.0f);
	pVtx[UI_VTX_HP_GAUGE + 2].pos = D3DXVECTOR3(HP_LEFT + 40, HP_BOTTOM, 0.0f);
	pVtx[UI_VTX_HP_GAUGE + 3].pos = D3DXVECTOR3(HP_RIGHT, HP_BOTTOM, 0.0f);

	//HP名前
	pVtx[UI_VTX_HP_NAME + 0].pos = D3DXVECTOR3(HP_RIGHT, HP_TOP - 10, 0.0f);
	pVtx[UI_VTX_HP_NAME + 1].pos = D3DXVECTOR3(HP_RIGHT + 100, HP_TOP - 10, 0.0f);
	pVtx[UI_VTX_HP_NAME + 2].pos = D3DXVECTOR3(HP_RIGHT, HP_BOTTOM, 0.0f);
	pVtx[UI_VTX_HP_NAME + 3].pos = D3DXVECTOR3(HP_RIGHT + 100, HP_BOTTOM, 0.0f);

	for (int nCntUI = UI_VTX_HP_FRAME; nCntUI < UI_VTX_HP_NAME + 4; nCntUI++)
	{
		pVtx[nCntUI].rhw = 1.0f;
		pVtx[nCntUI].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//テクスチャー座標の設定
	pVtx[UI_VTX_HP_FRAME + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[UI_VTX_HP_FRAME + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[UI_VTX_HP_FRAME + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[UI_VTX_HP_FRAME + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[UI_VTX_HP_GAUGE + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[UI_VTX_HP_GAUGE + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[UI_VTX_HP_GAUGE + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[UI_VTX_HP_GAUGE + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[UI_VTX_HP_NAME + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[UI_VTX_HP_NAME + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[UI_VTX_HP_NAME + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[UI_VTX_HP_NAME + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffUI->Unlock();
}
//=============================================================================
//HP_UIの更新処理
//=============================================================================
void UpdateHP(void)
{
	float fRate;
	Player* pPlayer = GetPlayer();
	fRate = (float)pPlayer->nLife / (float)pPlayer->nMaxLife;

	if (fRate < 0.0f)
	{
		fRate = 0.0f;
	}

	if (fRate > 1.0f)
	{
		fRate = 1.0f;
	}


	VERTEX_2D* pVtx;
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[UI_VTX_HP_GAUGE + 1].pos.x = HP_LEFT + HP_WIDTH * fRate;
	pVtx[UI_VTX_HP_GAUGE + 3].pos.x = HP_LEFT + HP_WIDTH * fRate;

	pVtx[UI_VTX_HP_GAUGE + 1].tex = D3DXVECTOR2(fRate, 0.0f);
	pVtx[UI_VTX_HP_GAUGE + 3].tex = D3DXVECTOR2(fRate, 1.0f);

	g_pVtxBuffUI->Unlock();
}
//=============================================================================
//HP_UIの描画処理
//=============================================================================
void DrawHP(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャをぼかさず表示
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);

	// 4～7番の頂点を使用
	pDevice->SetTexture(0, g_apTextureUI[UI_TEX_HP_GAUGE]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_HP_GAUGE, 2);

	// HP画像
	pDevice->SetTexture(0, g_apTextureUI[UI_TEX_HP_FRAME]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_HP_FRAME, 2);
	// HP画像
	pDevice->SetTexture(0, g_apTextureUI[UI_TEX_HP_NAME]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_HP_NAME, 2);
}
//=============================================================================
//Charge_UIの初期化処理
//=============================================================================
void InitCharge(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// チャージフレーム
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\Charge_Frame.png",
		&g_apTextureUI[UI_TEX_CHARGE_FRAME]
	);

	// チャージゲージ
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\Charge_Gauge.png",
		&g_apTextureUI[UI_TEX_CHARGE_GAUGE]
	);

	VERTEX_2D* pVtx;

	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[UI_VTX_CHARGE_FRAME + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[UI_VTX_CHARGE_FRAME + 1].pos = D3DXVECTOR3(15.0f, 0.0f, 0.0f);
	pVtx[UI_VTX_CHARGE_FRAME + 2].pos = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	pVtx[UI_VTX_CHARGE_FRAME + 3].pos = D3DXVECTOR3(15.0f, 100.0f, 0.0f);

	// チャージゲージ
	pVtx[UI_VTX_CHARGE_GAUGE + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[UI_VTX_CHARGE_GAUGE + 1].pos = D3DXVECTOR3(15.0f, 0.0f, 0.0f);
	pVtx[UI_VTX_CHARGE_GAUGE + 2].pos = D3DXVECTOR3(0.0f, 110.0f, 0.0f);
	pVtx[UI_VTX_CHARGE_GAUGE + 3].pos = D3DXVECTOR3(15.0f, 110.0f, 0.0f);

	for (int nCntCharge = UI_VTX_CHARGE_FRAME; nCntCharge < UI_VTX_CHARGE_GAUGE + 4; nCntCharge++)
	{
		pVtx[nCntCharge].rhw = 1.0f;
		pVtx[nCntCharge].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// フレームのテクスチャー座標
	pVtx[UI_VTX_CHARGE_FRAME + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[UI_VTX_CHARGE_FRAME + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[UI_VTX_CHARGE_FRAME + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[UI_VTX_CHARGE_FRAME + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ゲージのテクスチャー座標
	pVtx[UI_VTX_CHARGE_GAUGE + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[UI_VTX_CHARGE_GAUGE + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[UI_VTX_CHARGE_GAUGE + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[UI_VTX_CHARGE_GAUGE + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffUI->Unlock();
}
//=============================================================================
//Charge_UIの更新処理
//=============================================================================
void UpdateCharge(void)
{
	Player* pPlayer = GetPlayer();

	float fRate;

	fRate = (float)pPlayer->fPower / 100.0f;

	if (fRate < 0.0f)
	{
		fRate = 0.0f;
	}

	if (fRate > 1.0f)
	{
		fRate = 1.0f;
	}
	float fLeft = pPlayer->pos.x - CHARGE_OFFSET_X - CHARGE_WIDTH;
	float fTop = pPlayer->pos.y - CHARGE_HEIGHT * 0.5f;

	VERTEX_2D* pVtx;

	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	// チャージフレーム
	pVtx[UI_VTX_CHARGE_FRAME + 0].pos.x = fLeft;
	pVtx[UI_VTX_CHARGE_FRAME + 0].pos.y = fTop;
	pVtx[UI_VTX_CHARGE_FRAME + 1].pos.x = fLeft + CHARGE_WIDTH;
	pVtx[UI_VTX_CHARGE_FRAME + 1].pos.y = fTop;
	pVtx[UI_VTX_CHARGE_FRAME + 2].pos.x = fLeft;
	pVtx[UI_VTX_CHARGE_FRAME + 2].pos.y = fTop + CHARGE_HEIGHT;
	pVtx[UI_VTX_CHARGE_FRAME + 3].pos.x = fLeft + CHARGE_WIDTH;
	pVtx[UI_VTX_CHARGE_FRAME + 3].pos.y = fTop + CHARGE_HEIGHT;



	// チャージゲージを計算
	float fMarginTop = 20.0f;
	float fMarginBottom = 28.0f;
	float fGaugeTop = fTop + fMarginTop + (CHARGE_HEIGHT - fMarginTop - fMarginBottom) * (1.0f - fRate);
	float fGaugeBottom = fTop + CHARGE_HEIGHT - fMarginBottom;

	//頂点座標を設定
	pVtx[UI_VTX_CHARGE_GAUGE + 0].pos.x = fLeft;
	pVtx[UI_VTX_CHARGE_GAUGE + 0].pos.y = fGaugeTop;
	pVtx[UI_VTX_CHARGE_GAUGE + 1].pos.x = fLeft + CHARGE_WIDTH;
	pVtx[UI_VTX_CHARGE_GAUGE + 1].pos.y = fGaugeTop;
	pVtx[UI_VTX_CHARGE_GAUGE + 2].pos.x = fLeft;
	pVtx[UI_VTX_CHARGE_GAUGE + 2].pos.y = fGaugeBottom;
	pVtx[UI_VTX_CHARGE_GAUGE + 3].pos.x = fLeft + CHARGE_WIDTH;
	pVtx[UI_VTX_CHARGE_GAUGE + 3].pos.y = fGaugeBottom;

	g_pVtxBuffUI->Unlock();
}
//=============================================================================
//Charge_UIの描画処理
//=============================================================================
void DrawCharge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);
	// チャージフレーム
	pDevice->SetTexture(0, g_apTextureUI[UI_TEX_CHARGE_FRAME]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_CHARGE_FRAME, 2);

	// チャージゲージ
	pDevice->SetTexture(0, g_apTextureUI[UI_TEX_CHARGE_GAUGE]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_CHARGE_GAUGE, 2);


}
//=============================================================================
// WAVE表示の初期化処理
//=============================================================================
void InitWave(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// WAVE画像読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\WAVE_01.png",
		&g_apTextureUI[UI_TEX_WAVE_01]
	);
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\WAVE_02.png",
		&g_apTextureUI[UI_TEX_WAVE_02]
	);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\WAVE_03.png",
		&g_apTextureUI[UI_TEX_WAVE_03]
	);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\FINALWAVE.png",
		&g_apTextureUI[UI_TEX_FINAL_WAVE]
	);

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	//画面中央
	float fLeft = 638.0f - WAVE_WIDTH * 0.5f;
	float fTop = 310.0f - WAVE_HEIGHT * 0.5f;

	//頂点座標を設定
	pVtx[UI_VTX_WAVE + 0].pos = D3DXVECTOR3(fLeft, fTop, 0.0f);
	pVtx[UI_VTX_WAVE + 1].pos = D3DXVECTOR3(fLeft + WAVE_WIDTH, fTop, 0.0f);
	pVtx[UI_VTX_WAVE + 2].pos = D3DXVECTOR3(fLeft, fTop + WAVE_HEIGHT, 0.0f);
	pVtx[UI_VTX_WAVE + 3].pos = D3DXVECTOR3(fLeft + WAVE_WIDTH, fTop + WAVE_HEIGHT, 0.0f);

	//頂点設定
	for (int nCntWave = UI_VTX_WAVE; nCntWave < UI_VTX_WAVE + 4; nCntWave++)
	{
		pVtx[nCntWave].rhw = 1.0f;
		pVtx[nCntWave].Col = D3DCOLOR_RGBA(255, 255, 255, 150);
	}

	//テクスチャー座標を設定
	pVtx[UI_VTX_WAVE + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[UI_VTX_WAVE + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[UI_VTX_WAVE + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[UI_VTX_WAVE + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffUI->Unlock();
}
//=============================================================================
// WAVE表示の描画処理
//=============================================================================
void DrawGameWave(int nWave)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 1～4以外なら表示しない
	if (nWave < 1 || nWave > 4)
	{
		return;
	}

	//ぼかさないようにする
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));
	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャーの設定
	switch (nWave)
	{
	case 1:
		pDevice->SetTexture(0, g_apTextureUI[UI_TEX_WAVE_01]);
		break;

	case 2:
		pDevice->SetTexture(0, g_apTextureUI[UI_TEX_WAVE_02]);
		break;

	case 3:
		pDevice->SetTexture(0, g_apTextureUI[UI_TEX_WAVE_03]);
		break;

	case 4:
		pDevice->SetTexture(0, g_apTextureUI[UI_TEX_FINAL_WAVE]);
		StopSound(SOUND_LABEL_BGM001);
		break;
	}
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_WAVE, 2);
}
//=============================================================================
//ゲーム終了UIの初期化処理
//=============================================================================
void InitGameEnd(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\GameOver.png",
		&g_apTextureUI[UI_TEX_GAMEOVER]
	);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\GameClear.png",
		&g_apTextureUI[UI_TEX_GAMECLEAR]
	);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\Result.png",
		&g_apTextureUI[UI_TEX_RESULT]
	);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\Retry.png",
		&g_apTextureUI[UI_TEX_RETRY]
	);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\Title.png",
		&g_apTextureUI[UI_TEX_TITLE]
	);

	VERTEX_2D* pVtx;

	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);


	// GAME OVER / GAME CLEAR
	pVtx[UI_VTX_GAMEEND + 0].pos = D3DXVECTOR3(440.0f, 100.0f, 0.0f);
	pVtx[UI_VTX_GAMEEND + 1].pos = D3DXVECTOR3(840.0f, 100.0f, 0.0f);
	pVtx[UI_VTX_GAMEEND + 2].pos = D3DXVECTOR3(440.0f, 200.0f, 0.0f);
	pVtx[UI_VTX_GAMEEND + 3].pos = D3DXVECTOR3(840.0f, 200.0f, 0.0f);

	// リザルト
	pVtx[UI_VTX_RESULT + 0].pos = D3DXVECTOR3(500.0f, 300.0f, 0.0f);
	pVtx[UI_VTX_RESULT + 1].pos = D3DXVECTOR3(780.0f, 300.0f, 0.0f);
	pVtx[UI_VTX_RESULT + 2].pos = D3DXVECTOR3(500.0f, 360.0f, 0.0f);
	pVtx[UI_VTX_RESULT + 3].pos = D3DXVECTOR3(780.0f, 360.0f, 0.0f);

	// リトライ
	pVtx[UI_VTX_RETRY + 0].pos = D3DXVECTOR3(500.0f, 390.0f, 0.0f);
	pVtx[UI_VTX_RETRY + 1].pos = D3DXVECTOR3(780.0f, 390.0f, 0.0f);
	pVtx[UI_VTX_RETRY + 2].pos = D3DXVECTOR3(500.0f, 450.0f, 0.0f);
	pVtx[UI_VTX_RETRY + 3].pos = D3DXVECTOR3(780.0f, 450.0f, 0.0f);

	// タイトル
	pVtx[UI_VTX_TITLE + 0].pos = D3DXVECTOR3(500.0f, 480.0f, 0.0f);
	pVtx[UI_VTX_TITLE + 1].pos = D3DXVECTOR3(780.0f, 480.0f, 0.0f);
	pVtx[UI_VTX_TITLE + 2].pos = D3DXVECTOR3(500.0f, 540.0f, 0.0f);
	pVtx[UI_VTX_TITLE + 3].pos = D3DXVECTOR3(780.0f, 540.0f, 0.0f);

	// 頂点設定
	for (int nCnt = UI_VTX_GAMEEND; nCnt < UI_VTX_TITLE + 4; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
		pVtx[nCnt].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// テクスチャ座標
	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		int nBase = UI_VTX_GAMEEND + nCnt * 4;

		pVtx[nBase + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nBase + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nBase + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nBase + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	g_pVtxBuffUI->Unlock();
}
//=============================================================================
//ゲーム終了UIの描画処理
//=============================================================================
void DrawGameEnd(bool bGameClear)
{
	int nCntGameEnd;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (bGameClear)
	{
		pDevice->SetTexture(0, g_apTextureUI[UI_TEX_GAMECLEAR]);

		
		
	}
	else
	{
		pDevice->SetTexture(0, g_apTextureUI[UI_TEX_GAMEOVER]);
		
		
	}
	//ゲームオーバ/ゲームクリア
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_GAMEEND, 2);

	VERTEX_2D* pVtx;

	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	//未選択は暗くする
	for (nCntGameEnd = UI_VTX_RESULT; nCntGameEnd < UI_VTX_TITLE + 4; nCntGameEnd++)
	{
		pVtx[nCntGameEnd].Col = D3DCOLOR_RGBA(150, 150, 150, 255);
	}

	// 選択中の項目だけ黄色
	switch (g_nGameEndSelect)
	{
	case 0:
		// リザルト
		for (nCntGameEnd = UI_VTX_RESULT; nCntGameEnd < UI_VTX_RESULT + 4; nCntGameEnd++)
		{
			pVtx[nCntGameEnd].Col = D3DCOLOR_RGBA(255, 255, 0, 255);
		}
		break;

	case 1:
		// リトライ
		for (nCntGameEnd = UI_VTX_RETRY; nCntGameEnd < UI_VTX_RETRY + 4; nCntGameEnd++)
		{
			pVtx[nCntGameEnd].Col = D3DCOLOR_RGBA(255, 255, 0, 255);
		}
		break;

	case 2:
		// タイトル
		for (nCntGameEnd = UI_VTX_TITLE; nCntGameEnd < UI_VTX_TITLE + 4; nCntGameEnd++)
		{
			pVtx[nCntGameEnd].Col = D3DCOLOR_RGBA(255, 255, 0, 255);
		}
		break;
	}

	g_pVtxBuffUI->Unlock();


	//リザルト
	pDevice->SetTexture(0, g_apTextureUI[UI_TEX_RESULT]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_RESULT, 2);

	// リトライ
	pDevice->SetTexture(0, g_apTextureUI[UI_TEX_RETRY]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_RETRY, 2);

	// タイトル
	pDevice->SetTexture(0, g_apTextureUI[UI_TEX_TITLE]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_TITLE, 2);
}
//=============================================================================
//BossHP_UIの初期化処理
//=============================================================================
void InitBossHP(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャ読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\HP_Frame.png",
		&g_apTextureUI[UI_TEX_BOSS_HP_FRAME]
	);
	// テクスチャ読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\Boss_HP_Gauge.png",
		&g_apTextureUI[UI_TEX_BOSS_HP_GAUGE]
	);




	VERTEX_2D* pVtx;
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	// HPフレーム
	pVtx[UI_VTX_BOSS_HP_FRAME + 0].pos = D3DXVECTOR3(BOSSHP_LEFT, BOSSHP_TOP, 0.0f);
	pVtx[UI_VTX_BOSS_HP_FRAME + 1].pos = D3DXVECTOR3(BOSSHP_RIGHT, BOSSHP_TOP, 0.0f);
	pVtx[UI_VTX_BOSS_HP_FRAME + 2].pos = D3DXVECTOR3(BOSSHP_LEFT, BOSSHP_BOTTOM, 0.0f);
	pVtx[UI_VTX_BOSS_HP_FRAME + 3].pos = D3DXVECTOR3(BOSSHP_RIGHT, BOSSHP_BOTTOM, 0.0f);


	// HPゲージ
	pVtx[UI_VTX_BOSS_HP_GAUGE + 0].pos = D3DXVECTOR3(BOSSHP_LEFT + 40, BOSSHP_TOP, 0.0f);
	pVtx[UI_VTX_BOSS_HP_GAUGE + 1].pos = D3DXVECTOR3(BOSSHP_RIGHT, BOSSHP_TOP, 0.0f);
	pVtx[UI_VTX_BOSS_HP_GAUGE + 2].pos = D3DXVECTOR3(BOSSHP_LEFT + 40, BOSSHP_BOTTOM, 0.0f);
	pVtx[UI_VTX_BOSS_HP_GAUGE + 3].pos = D3DXVECTOR3(BOSSHP_RIGHT, BOSSHP_BOTTOM, 0.0f);



	for (int nCntUI = UI_VTX_BOSS_HP_FRAME; nCntUI < UI_VTX_BOSS_HP_GAUGE + 4; nCntUI++)
	{
		pVtx[nCntUI].rhw = 1.0f;
		pVtx[nCntUI].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}


	// フレーム
	pVtx[UI_VTX_BOSS_HP_FRAME + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[UI_VTX_BOSS_HP_FRAME + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[UI_VTX_BOSS_HP_FRAME + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[UI_VTX_BOSS_HP_FRAME + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ゲージ
	pVtx[UI_VTX_BOSS_HP_GAUGE + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[UI_VTX_BOSS_HP_GAUGE + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[UI_VTX_BOSS_HP_GAUGE + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[UI_VTX_BOSS_HP_GAUGE + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffUI->Unlock();
}
//=============================================================================
//BossHP_UIの更新処理
//=============================================================================
void UpdateBossHP(void)
{
	float fRate;
	Boss* pBoss = GetBoss();

	fRate = (float)pBoss->nLife / (float)pBoss->nMaxLife;


	if (fRate < 0.0f)
	{
		fRate = 0.0f;
	}

	if (fRate > 1.0f)
	{
		fRate = 1.0f;
	}


	VERTEX_2D* pVtx;
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);
	// ボスHPゲージ
	pVtx[UI_VTX_BOSS_HP_GAUGE + 1].pos.x = BOSSHP_LEFT + 40.0f + (BOSSHP_WIDTH - 40.0f) * fRate;
	pVtx[UI_VTX_BOSS_HP_GAUGE + 3].pos.x = BOSSHP_LEFT + 40.0f + (BOSSHP_WIDTH - 40.0f) * fRate;

	// テクスチャ座標
	pVtx[UI_VTX_BOSS_HP_GAUGE + 1].tex = D3DXVECTOR2(fRate, 0.0f);
	pVtx[UI_VTX_BOSS_HP_GAUGE + 3].tex = D3DXVECTOR2(fRate, 1.0f);

	g_pVtxBuffUI->Unlock();
}
//=============================================================================
//BossHP_UIの描画処理
//=============================================================================
void DrawBossHP(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャをぼかさず表示
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);



	pDevice->SetTexture(0, g_apTextureUI[UI_TEX_BOSS_HP_GAUGE]);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_BOSS_HP_GAUGE, 2);


	pDevice->SetTexture(0, g_apTextureUI[UI_TEX_BOSS_HP_FRAME]);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_BOSS_HP_FRAME, 2);
}
//=============================================================================
//ポーズ画面UIの初期化処理
//=============================================================================
void InitGamePause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\Pause.png",
		&g_apTextureUI[UI_TEX_PAUSE]
	);



	VERTEX_2D* pVtx;

	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);


	//ポーズ
	pVtx[UI_VTX_PAUSE + 0].pos = D3DXVECTOR3(440.0f, 100.0f, 0.0f);
	pVtx[UI_VTX_PAUSE + 1].pos = D3DXVECTOR3(840.0f, 100.0f, 0.0f);
	pVtx[UI_VTX_PAUSE + 2].pos = D3DXVECTOR3(440.0f, 300.0f, 0.0f);
	pVtx[UI_VTX_PAUSE + 3].pos = D3DXVECTOR3(840.0f, 300.0f, 0.0f);

	//リザルト
	pVtx[UI_VTX_RESULT + 0].pos = D3DXVECTOR3(500.0f, 300.0f, 0.0f);
	pVtx[UI_VTX_RESULT + 1].pos = D3DXVECTOR3(780.0f, 300.0f, 0.0f);
	pVtx[UI_VTX_RESULT + 2].pos = D3DXVECTOR3(500.0f, 360.0f, 0.0f);
	pVtx[UI_VTX_RESULT + 3].pos = D3DXVECTOR3(780.0f, 360.0f, 0.0f);

	//リトライ
	pVtx[UI_VTX_RETRY + 0].pos = D3DXVECTOR3(500.0f, 390.0f, 0.0f);
	pVtx[UI_VTX_RETRY + 1].pos = D3DXVECTOR3(780.0f, 390.0f, 0.0f);
	pVtx[UI_VTX_RETRY + 2].pos = D3DXVECTOR3(500.0f, 450.0f, 0.0f);
	pVtx[UI_VTX_RETRY + 3].pos = D3DXVECTOR3(780.0f, 450.0f, 0.0f);

	//タイトル
	pVtx[UI_VTX_TITLE + 0].pos = D3DXVECTOR3(500.0f, 480.0f, 0.0f);
	pVtx[UI_VTX_TITLE + 1].pos = D3DXVECTOR3(780.0f, 480.0f, 0.0f);
	pVtx[UI_VTX_TITLE + 2].pos = D3DXVECTOR3(500.0f, 540.0f, 0.0f);
	pVtx[UI_VTX_TITLE + 3].pos = D3DXVECTOR3(780.0f, 540.0f, 0.0f);


	// 頂点設定
	for (int nCnt = UI_VTX_PAUSE; nCnt < UI_VTX_PAUSE + 4; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
		pVtx[nCnt].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}


	pVtx[UI_VTX_PAUSE + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[UI_VTX_PAUSE + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[UI_VTX_PAUSE + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[UI_VTX_PAUSE + 3].tex = D3DXVECTOR2(1.0f, 1.0f);


	g_pVtxBuffUI->Unlock();
}
//=============================================================================
//ポーズ画面UIの描画処理
//=============================================================================
void DrawGamePause(void)
{
	int nCntGameEnd;
	int nCntGamePause;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_apTextureUI[UI_TEX_PAUSE]);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_PAUSE, 2);

	VERTEX_2D* pVtx;

	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	//未選択は暗くする
	for (nCntGameEnd = UI_VTX_RESULT; nCntGameEnd < UI_VTX_TITLE + 4; nCntGameEnd++)
	{
		pVtx[nCntGameEnd].Col = D3DCOLOR_RGBA(150, 150, 150, 255);
	}

	//選択中の項目だけ黄色
	switch (g_nGameEndSelect)
	{
	case 0:

		//リザルト
		for (nCntGamePause = UI_VTX_RESULT; nCntGamePause < UI_VTX_RESULT + 4; nCntGamePause++)
		{
			pVtx[nCntGamePause].Col = D3DCOLOR_RGBA(255, 255, 0, 255);
		}

		break;

	case 1:

		//リトライ
		for (nCntGamePause = UI_VTX_RETRY; nCntGamePause < UI_VTX_RETRY + 4; nCntGamePause++)
		{
			pVtx[nCntGamePause].Col = D3DCOLOR_RGBA(255, 255, 0, 255);
		}

		break;

	case 2:

		//タイトル
		for (nCntGamePause = UI_VTX_TITLE; nCntGamePause < UI_VTX_TITLE + 4; nCntGamePause++)
		{
			pVtx[nCntGamePause].Col = D3DCOLOR_RGBA(255, 255, 0, 255);
		}

		break;
	}


	g_pVtxBuffUI->Unlock();


	pDevice->SetTexture(0, g_apTextureUI[UI_TEX_RESULT]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_RESULT, 2);


	pDevice->SetTexture(0, g_apTextureUI[UI_TEX_RETRY]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_RETRY, 2);


	pDevice->SetTexture(0, g_apTextureUI[UI_TEX_TITLE]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_TITLE, 2);

}
//=============================================================================
//チュートリアルUIの初期化処理
//=============================================================================
void InitTutorialUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\Tutorial_Skip.png",
		&g_apTextureUI[UI_TEX_TUTORIAL_SKIP]
	);

	VERTEX_2D* pVtx;
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	//スキップ
	pVtx[UI_VTX_TUTORIAL_SKIP + 0].pos = D3DXVECTOR3(1000.0f, 100.0f, 0.0f);
	pVtx[UI_VTX_TUTORIAL_SKIP + 1].pos = D3DXVECTOR3(1200.0f, 100.0f, 0.0f);
	pVtx[UI_VTX_TUTORIAL_SKIP + 2].pos = D3DXVECTOR3(1000.0f, 200.0f, 0.0f);
	pVtx[UI_VTX_TUTORIAL_SKIP + 3].pos = D3DXVECTOR3(1200.0f, 200.0f, 0.0f);

	// 頂点設定
	for (int nCnt = UI_VTX_TUTORIAL_SKIP; nCnt < UI_VTX_TUTORIAL_SKIP + 4; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
		pVtx[nCnt].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	pVtx[UI_VTX_TUTORIAL_SKIP + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[UI_VTX_TUTORIAL_SKIP + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[UI_VTX_TUTORIAL_SKIP + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[UI_VTX_TUTORIAL_SKIP + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffUI->Unlock();

}
//=============================================================================
//チュートリアルUIの描画処理
//=============================================================================
void DrawTutorialUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャをぼかさず表示
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_apTextureUI[UI_TEX_TUTORIAL_SKIP]);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_TUTORIAL_SKIP, 2);
}
//=============================================================================
//ゲームUIの初期化処理
//=============================================================================
void InitGameUI(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\WASDkey.png",
		&g_apTextureUI[UI_TEX_GAME_WASD]
	);
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\SpaceKey.png",
		&g_apTextureUI[UI_TEX_GAME_SPACE]
	);

	VERTEX_2D* pVtx;
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	//移動説明
	pVtx[UI_VTX_GAME_WASD + 0].pos = D3DXVECTOR3(1000.0f, 100.0f, 0.0f);
	pVtx[UI_VTX_GAME_WASD + 1].pos = D3DXVECTOR3(1200.0f, 100.0f, 0.0f);
	pVtx[UI_VTX_GAME_WASD + 2].pos = D3DXVECTOR3(1000.0f, 200.0f, 0.0f);
	pVtx[UI_VTX_GAME_WASD + 3].pos = D3DXVECTOR3(1200.0f, 200.0f, 0.0f);
	//撃つ説明
	pVtx[UI_VTX_GAME_SPACE + 0].pos = D3DXVECTOR3(1000.0f, 300.0f, 0.0f);
	pVtx[UI_VTX_GAME_SPACE + 1].pos = D3DXVECTOR3(1200.0f, 300.0f, 0.0f);
	pVtx[UI_VTX_GAME_SPACE + 2].pos = D3DXVECTOR3(1000.0f, 400.0f, 0.0f);
	pVtx[UI_VTX_GAME_SPACE + 3].pos = D3DXVECTOR3(1200.0f, 400.0f, 0.0f);

	// 頂点設定
	for (int nCnt = UI_VTX_GAME_WASD; nCnt < UI_VTX_GAME_SPACE + 4; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
		pVtx[nCnt].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	pVtx[UI_VTX_GAME_WASD + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[UI_VTX_GAME_WASD + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[UI_VTX_GAME_WASD + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[UI_VTX_GAME_WASD + 3].tex = D3DXVECTOR2(1.0f, 1.0f);


	pVtx[UI_VTX_GAME_SPACE + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[UI_VTX_GAME_SPACE + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[UI_VTX_GAME_SPACE + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[UI_VTX_GAME_SPACE + 3].tex = D3DXVECTOR2(1.0f, 1.0f);


	g_pVtxBuffUI->Unlock();
}
//=============================================================================
//ゲームUIの描画処理
//=============================================================================
void DrawGameUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャをぼかさず表示
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_apTextureUI[UI_TEX_GAME_WASD]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_GAME_WASD, 2);

	pDevice->SetTexture(0, g_apTextureUI[UI_TEX_GAME_SPACE]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UI_VTX_GAME_SPACE, 2);
}
