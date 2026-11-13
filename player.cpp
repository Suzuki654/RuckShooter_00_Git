//=============================================================================
//
//プレイヤーの処理実装ファイル[Player.cpp]
//Author:鈴木哲汰
//
//=============================================================================
#include "player.h"
#include "bg.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"

//マクロ定義
#define PLAYER_WIDTH  60.0f//幅
#define PLAYER_HEIGHT 80.0f//縦

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;//テクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;//頂点バッファ
Player g_Player;
bool g_bChargeSound = false;
float g_fShotPower = 0.0f;

//=============================================================================
//プレイヤーの初期化処理
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャーの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ruckman_02.png",
		&g_pTexturePlayer);

	g_Player.pos = D3DXVECTOR3(500.0f, 500.0f, 0.0f);//位置を初期化する
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量を初期化する
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きを初期化する
	g_Player.nCounterAnim = 0;//カウンターを初期化する
	g_Player.nPatternAnim = 0;//パターンNo.を初期化する
	g_Player.fPower = 0;//パワーを初期化
	g_Player.nLife = g_Player.nMaxLife;//体力を初期化
	g_Player.nMaxLife = 200;			// 最大HP
	g_Player.nCounterState = 0;//状態カウンターを初期化
	g_Player.state = PLAYERSTATE_NORMAL;//状態を初期化
	g_Player.bUse = true;//使用している状態に初期化

	//対角線を算出
	g_Player.fLength = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT) * 0.5f;//幅＋縦
	//対角線の角度を算出
	g_Player.fAngle = atan2f(PLAYER_WIDTH, PLAYER_HEIGHT);//幅、縦

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D* pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength;
	pVtx[3].pos.z = 0.0f;

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

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}
//=============================================================================
//プレイヤーの終了処理
//=============================================================================
void UninitPlayer(void)
{
	//テクスチャーの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}
//=============================================================================
//プレイヤーの更新処理
//=============================================================================
void UpdatePlayer(void)
{

	// スティックの入力値を取得
	SHORT stickX = GetJoypadLeftStickX();
	SHORT stickY = GetJoypadLeftStickY();

	VERTEX_2D* pVtx;//頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Player.bUse == true)
	{
		switch (g_Player.state)
		{
		case PLAYERSTATE_NORMAL:
			// チャージ
			if (GetKeyboardPress(DIK_SPACE) || GetJoypadPress(JOYKEY_A) || GetJoypadPress(JOYKEY_RIGHT_SHOULDER))
			{
				if (g_Player.fPower < 100.0f)
				{
					g_Player.fPower += 1.5f;
					
				}
				else
				{
					StopSound(SOUND_LABEL_SE_CHARGE000);
				}

				if (!g_bChargeSound)
				{
					PlaySound(SOUND_LABEL_SE_CHARGE000);
					g_bChargeSound = true;
				}

	
			}
			else
			{
				// ボタンを離したらリセット
				g_bChargeSound = false;
			}

			if (GetKeyboardRelease(DIK_SPACE) == true || GetJoypadRelease(JOYKEY_A) == true || GetJoypadRelease(JOYKEY_RIGHT_SHOULDER) == true)
			{//スペースキーが放されたら
				g_fShotPower = g_Player.fPower;
				StopSound(SOUND_LABEL_SE_CHARGE000);
				PlaySound(SOUND_LABEL_SE_SHOT);

				if (g_Player.fPower < 30.0f)
				{
					// 向いている方向と反対へ移動
					g_Player.move.x -= cosf(g_Player.rot.z) * 5;
					g_Player.move.y += sinf(g_Player.rot.z) * 5;

				}
				else if (g_Player.fPower < 70.0f)
				{
					// 向いている方向と反対へ移動
					g_Player.move.x -= cosf(g_Player.rot.z) * 15;
					g_Player.move.y += sinf(g_Player.rot.z) * 15;
					

				}
				else if (g_Player.fPower < 99.0f)
				{
					// 向いている方向と反対へ移動
					g_Player.move.x -= cosf(g_Player.rot.z) * 30;
					g_Player.move.y += sinf(g_Player.rot.z) * 30;
					
				}
				else
				{
					// 向いている方向と反対へ移動
					g_Player.move.x -= cosf(g_Player.rot.z) * 40;
					g_Player.move.y += sinf(g_Player.rot.z) * 40;
					
				}

			}

			//左スティックが倒されている場合
			if (stickX != 0 || stickY != 0)
			{
				//スティックの方向から角度を計算
				g_Player.rot.z = atan2f((float)-stickX, (float)stickY) + D3DX_PI * 0.5f;
			}
			else if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
			{//Aキーが押された

				if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
				{//左上を向く
					g_Player.rot.z = D3DX_PI * 0.75f;
				}
				else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
				{//左下を向く

					g_Player.rot.z = -D3DX_PI * 0.75f;
				}
				else
				{//左を向く

					//向きを更新する
					g_Player.rot.z = D3DX_PI * 1.0f;
				}
			}
			else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
			{//Dキーが押された


				if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
				{//右上
					g_Player.rot.z = D3DX_PI * 0.25f;

				}
				else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
				{//右下
					g_Player.rot.z = -D3DX_PI * 0.25f;
				}
				else
				{//右移動
					g_Player.rot.z = D3DX_PI * 0.0f;

				}
			}
			else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
			{//Sキーが押された
				g_Player.rot.z = -D3DX_PI * 0.5f;
			}
			else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
			{//Wキーが押された
				g_Player.rot.z = D3DX_PI * 0.5f;
			}
			break;
		case PLAYERSTATE_DAMAGE:

			g_Player.nCounterState--;
			if (g_Player.nCounterState <= 0)
			{
				g_Player.state = PLAYERSTATE_NORMAL;
				//頂点カラーの設定
				pVtx[0].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].Col = D3DCOLOR_RGBA(255, 255, 255, 255);

			}
			break;
		}
	}


	//壁
	if (g_Player.pos.x >= (BORDEA_BG_RIGHT - 120.0f)) // 右端
	{
		g_Player.pos.x = BORDEA_BG_RIGHT - 120.0f;
		g_Player.move.x = -g_Player.move.x; // 反転
		PlaySound(SOUND_LABEL_SE_WALL000);
	}
	else if (g_Player.pos.x <= BORDEA_BG_LEFT + 120.0f) // 左端
	{
		g_Player.pos.x = BORDEA_BG_LEFT + 120.0f;
		g_Player.move.x = -g_Player.move.x; // 反転
		PlaySound(SOUND_LABEL_SE_WALL000);
	}
	if (g_Player.pos.y >= (BORDEA_BG_UNDER - 200.0f)) // 下端
	{
		g_Player.pos.y = BORDEA_BG_UNDER - 200.0f;
		g_Player.move.y = -g_Player.move.y; // 反転
		PlaySound(SOUND_LABEL_SE_WALL000);

	}
	else if (g_Player.pos.y <= BORDEA_BG_TOP + 150.0f) // 上端
	{
		g_Player.pos.y = BORDEA_BG_TOP + 150.0f;
		g_Player.move.y = -g_Player.move.y; // 反転
		PlaySound(SOUND_LABEL_SE_WALL000);

	}


	//アニメーション
	float fUCoord, fVCoord;

	//g_nCounterAnimPlayer++;//カウンタを加算
	//if ((g_nCounterAnimPlayer % 4)== 0)
	//{//一定時間経過
	//	
	//   //パターンNo.を更新する
	//	g_nPatternAnimPlayer = (g_nPatternAnimPlayer + 1) % 10;
	//	
	//	//頂点バッファをロックし、頂点情報へのポインタを取得
	//	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//	fUCoord = (g_nPatternAnimPlayer % 4) * 0.25f;//sfsfs
	//	fVCoord = (g_nPatternAnimPlayer / 4) * 0.25f;//
	//	
	//	//テクスチャー座標の再設定
	//	pVtx[0].tex = D3DXVECTOR2(fUCoord, fVCoord);
	//	pVtx[1].tex = D3DXVECTOR2(fUCoord + 0.25f , fVCoord);
	//	pVtx[2].tex = D3DXVECTOR2(fUCoord, fVCoord + 0.25f);
	//	pVtx[3].tex = D3DXVECTOR2(fUCoord + 0.25f , fVCoord + 0.25f);
	//	
	//	//頂点バッファをあんろっくする
	//	g_pVtxBuffPlayer->Unlock();
	//}

	//位置を更新
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.y += g_Player.move.y;

	//移動量を更新
	g_Player.move.x += (0.0f - g_Player.move.x) * 0.1f;
	g_Player.move.y += (0.0f - g_Player.move.y) * 0.1f;





	//頂点座標の設定
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをあんろっくする
	g_pVtxBuffPlayer->Unlock();

}
//=============================================================================
//プレイヤーの描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);
	if (g_Player.bUse == true)
	{
		//テクスチャーの設定
		pDevice->SetTexture(0, g_pTexturePlayer);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}


}
//=============================================================================
//プレイヤーのヒット処理
//=============================================================================
void HitPlayer(int nDamage)
{
	VERTEX_2D* pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	g_Player.nLife -= nDamage;

	if (g_Player.nLife <= 0)
	{//体力がゼロになったら
		//爆発の設定
		SetExplosion(g_Player.pos, D3DCOLOR_RGBA(255, 255, 255, 255),1);
		g_Player.bUse = false;//使用していない
		PlaySound(SOUND_LABEL_SE_PlayerDeath);

	}
	else
	{//ゼロじゃないとき
		g_Player.state = PLAYERSTATE_DAMAGE;
		g_Player.nCounterState = 5;//ダメージ状態をたもつ時間

		PlaySound(SOUND_LABEL_SE_HIT);

		//頂点カラーの設定
		pVtx[0].Col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].Col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].Col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].Col = D3DCOLOR_RGBA(255, 0, 0, 255);
	}

	//頂点バッファをあんろっくする

	g_pVtxBuffPlayer->Unlock();

}
//=============================================================================
//プレイヤーを取得
//=============================================================================
Player* GetPlayer(void)
{
	return &g_Player;
}
//=============================================================================
//プレイヤーのHpを取得
//=============================================================================
int GetPlayerLife(void)
{
	return g_Player.nLife;
}
//=============================================================================
//プレイヤーのパワーを取得
//============================================================================
float GetShotPower(void)
{
	return g_fShotPower;
}
