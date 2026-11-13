//=============================================================================
//
//敵の処理実装ファイル[enemy.cpp]
//Author:鈴木哲汰
//
//=============================================================================
#include "enemy.h"
#include "explosion.h"
#include "bullet.h"
#include "score.h"
#include "player.h"
#include "bg.h"
#include "laser.h"
#include "sound.h"

//マクロ定義
#define NUM_ENEMY (4)//敵の種類
#define ENEMY_WIDTH  (50.0f)//幅
#define ENEMY_HEIGHT (50.0f)//縦
#define ENEMY_BULLET_SPEED (2.0f) //弾の速さ
#define ENEMY_BULLET_LIFE (500) //弾の寿命


//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
Enemy g_aEnemy[MAX_ENEMY];//敵の情報
float g_fLengthEnemy;//対角線の長さ
float g_fAngleEnemy;//対角線の角度
int g_nNumEnemy;//敵の総数
//=============================================================================
//敵の初期化処理
//=============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy000.png",
		&g_apTextureEnemy[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy001_a.png",
		&g_apTextureEnemy[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy002.png",
		&g_apTextureEnemy[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy003.png",
		&g_apTextureEnemy[3]);

	//敵情報の初期化
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].fStopY_00 = 0.0f;
		g_aEnemy[nCntEnemy].fStopY_01 = 0.0f;
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].nLife = 100;
		g_aEnemy[nCntEnemy].nHitDamage = 0;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nWaitTime = 0;
		g_aEnemy[nCntEnemy].nExistenceTime = 0;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].nCounterAnim = 0;
		g_aEnemy[nCntEnemy].nPatternAnim = 0;
		g_aEnemy[nCntEnemy].bCharge = false;
		g_aEnemy[nCntEnemy].bUse = false;
	}

	g_nNumEnemy = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	//対角線を算出
	g_fLengthEnemy = sqrtf(ENEMY_WIDTH * ENEMY_WIDTH + ENEMY_HEIGHT * ENEMY_HEIGHT) * 0.5f;
	//対角線の角度を算出
	g_fAngleEnemy = atan2f(ENEMY_WIDTH, ENEMY_HEIGHT);


	VERTEX_2D* pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);



	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI + g_fAngleEnemy)) * g_fLengthEnemy;
		pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI + g_fAngleEnemy)) * g_fLengthEnemy;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI - g_fAngleEnemy)) * g_fLengthEnemy;
		pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI - g_fAngleEnemy)) * g_fLengthEnemy;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - g_fAngleEnemy) * g_fLengthEnemy;
		pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - g_fAngleEnemy) * g_fLengthEnemy;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + g_fAngleEnemy) * g_fLengthEnemy;
		pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + g_fAngleEnemy) * g_fLengthEnemy;
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

		pVtx += 4;
	}
}
//=============================================================================
//敵の終了処理
//=============================================================================
void UninitEnemy(void)
{
	int nCntEnemy;
	//テクスチャー4枚分の破棄
	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}
//=============================================================================
//敵の更新処理
//=============================================================================
void UpdateEnemy(void)
{

	int nCntEnemy;
	float fDX;
	float fDY;
	float fLength;
	Player* pPlayer = GetPlayer();
	VERTEX_2D* pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			CollisionPlayer(&g_aEnemy[nCntEnemy]);

			if (g_aEnemy[nCntEnemy].nType == 0)
			{


				switch (g_aEnemy[nCntEnemy].state)
				{

				case ENEMYSTATE_NORMAL:
				{
					//敵を動かす
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;


					//画面内に入ったら止める
					if (g_aEnemy[nCntEnemy].pos.y >= g_aEnemy[nCntEnemy].fStopY_00)
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_aEnemy[nCntEnemy].nExistenceTime --;
						//時間経過で去る
						if (g_aEnemy[nCntEnemy].nExistenceTime <= 0)
						{
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
							
						}
					}
					//画面外に出たら敵を消す
					if (g_aEnemy[nCntEnemy].pos.y > GAME_BG_UNDER + 100.0f)
					{
						g_aEnemy[nCntEnemy].bUse = false;
						g_nNumEnemy--;
					}
					

					// プレイヤーの方向を向く
					fDX = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;
					fDY = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;

					g_aEnemy[nCntEnemy].rot.z = -atan2f(fDY, fDX) + D3DX_PI / 2.0f;

					g_aEnemy[nCntEnemy].nCounterState--;
					g_aEnemy[nCntEnemy].nWaitTime--;

					if (g_aEnemy[nCntEnemy].nWaitTime <= 0)
					{
						// プレイヤー方向へ弾を飛ばす
						fLength = sqrtf(fDX * fDX + fDY * fDY);

						if (fLength != 0.0f)
						{
							fDX /= fLength;
							fDY /= fLength;
						}

						SetBullet(g_aEnemy[nCntEnemy].pos,D3DXVECTOR3(fDX * ENEMY_BULLET_SPEED, fDY * ENEMY_BULLET_SPEED, 0.0f),ENEMY_BULLET_LIFE,2,BULLETTYPE_ENEMY);

						g_aEnemy[nCntEnemy].nWaitTime = 240;//行動間隔

					}
					break;
				}
				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
						//頂点カラーの設定
						pVtx[0].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].Col = D3DCOLOR_RGBA(255, 255, 255, 255);

					}

					break;
				}

			}
			else if (g_aEnemy[nCntEnemy].nType == 1)
			{

				switch (g_aEnemy[nCntEnemy].state)
				{

				case ENEMYSTATE_NORMAL:
				{
					if (g_aEnemy[nCntEnemy].bCharge == false)
					{
						// 上から下へ移動
						if (g_aEnemy[nCntEnemy].move.y != 0.0f)
						{
							g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

							// 停止位置に到達
							if (g_aEnemy[nCntEnemy].pos.y >= g_aEnemy[nCntEnemy].fStopY_00)
							{
								g_aEnemy[nCntEnemy].pos.y = g_aEnemy[nCntEnemy].fStopY_00;
								g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								// 突進まで60フレーム
								g_aEnemy[nCntEnemy].nCounterState;
							}
						}
						else
						{
							//プレイヤー方向を計算
							fDX = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;

							fDY = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;

							fLength = sqrtf(fDX * fDX + fDY * fDY);

							if (fLength != 0.0f)
							{
								fDX /= fLength;
								fDY /= fLength;
							}

							// プレイヤーの方向を向く
							g_aEnemy[nCntEnemy].rot.z = -atan2f(fDY, fDX) + D3DX_PI / 2.0f;

							// 待機時間
							g_aEnemy[nCntEnemy].nCounterState--;
							g_aEnemy[nCntEnemy].nWaitTime--;

							if (g_aEnemy[nCntEnemy].nWaitTime <= 0)
							{
								// 突進開始
								g_aEnemy[nCntEnemy].move =D3DXVECTOR3(fDX * 8.0f, fDY * 8.0f, 0.0f);

								g_aEnemy[nCntEnemy].bCharge = true;
								PlaySound(SOUND_LABEL_SE_Enemy1);
							}
						}


					}
					else
					{
						g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

						// 画面外に出たら敵を消す
						if (g_aEnemy[nCntEnemy].pos.x < GAME_BG_LEFT - 100.0f ||
							g_aEnemy[nCntEnemy].pos.x > GAME_BG_RIGHT + 100.0f ||
							g_aEnemy[nCntEnemy].pos.y < GAME_BG_TOP - 100.0f ||
							g_aEnemy[nCntEnemy].pos.y > GAME_BG_UNDER + 100.0f)
						{
							g_aEnemy[nCntEnemy].bUse = false;
							g_nNumEnemy--;
						}
						// アニメーション
						g_aEnemy[nCntEnemy].nCounterAnim++;
						if (g_aEnemy[nCntEnemy].nCounterAnim >= 1)
						{
							g_aEnemy[nCntEnemy].nCounterAnim = 0;
							g_aEnemy[nCntEnemy].nPatternAnim++;

							if (g_aEnemy[nCntEnemy].nPatternAnim >= 2)
							{
								g_aEnemy[nCntEnemy].nPatternAnim = 0;
							}

							float fUCoord = g_aEnemy[nCntEnemy].nPatternAnim * 0.5f;

							pVtx[0].tex = D3DXVECTOR2(fUCoord, 0.0f);
							pVtx[1].tex = D3DXVECTOR2(fUCoord + 0.5f, 0.0f);
							pVtx[2].tex = D3DXVECTOR2(fUCoord, 1.0f);
							pVtx[3].tex = D3DXVECTOR2(fUCoord + 0.5f, 1.0f);
						}
					}

					// プレイヤーとの当たり判定
					CollisionPlayer(&g_aEnemy[nCntEnemy]);

					break;
				}
				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
						//頂点カラーの設定
						pVtx[0].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].Col = D3DCOLOR_RGBA(255, 255, 255, 255);

					}

					break;
				}


			}
			else if (g_aEnemy[nCntEnemy].nType == 2)
			{
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					// 敵を上から下へ移動
					if (g_aEnemy[nCntEnemy].move.y != 0.0f)
					{
						g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

						// 停止位置に到達
						if (g_aEnemy[nCntEnemy].move.y > 0.0f && g_aEnemy[nCntEnemy].pos.y >= g_aEnemy[nCntEnemy].fStopY_01)
						{
							g_aEnemy[nCntEnemy].pos.y = g_aEnemy[nCntEnemy].fStopY_01;
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							// レーザー発射まで待つ
							g_aEnemy[nCntEnemy].nCounterState = 60;
						

						}
					}
					else
					{
						// レーザー発射までカウント
						g_aEnemy[nCntEnemy].nWaitTime--;
						if (g_aEnemy[nCntEnemy].nWaitTime <= 0)
						{
							// 横方向の壁レーザーを生成
							SetLaser(D3DXVECTOR3(GAME_BG_LEFT + 270.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 500.0f, 20.0f,1, 60);
							PlaySound(SOUND_LABEL_SE_Enemy2);
							// 次のレーザーまで待つ
							g_aEnemy[nCntEnemy].nWaitTime = 180;

						}

						g_aEnemy[nCntEnemy].nExistenceTime--;
						if (g_aEnemy[nCntEnemy].nExistenceTime <= 0)
						{
							// 下へ移動
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
						}
						
					}
					//画面外に出たら敵を消す
					if (g_aEnemy[nCntEnemy].pos.y > GAME_BG_UNDER + 100.0f)
					{
						g_aEnemy[nCntEnemy].bUse = false;
						g_nNumEnemy--;
					}
					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
						//頂点カラーの設定
						pVtx[0].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].Col = D3DCOLOR_RGBA(255, 255, 255, 255);

					}

					break;
				}

			}




			//頂点座標の設定
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI + g_fAngleEnemy)) * g_fLengthEnemy;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI + g_fAngleEnemy)) * g_fLengthEnemy;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI - g_fAngleEnemy)) * g_fLengthEnemy;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI - g_fAngleEnemy)) * g_fLengthEnemy;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[3].pos.z = 0.0f;
		}
		pVtx += 4;
	}
	//頂点バッファをあんろっくする
	g_pVtxBuffEnemy->Unlock();

}
//=============================================================================
//敵の描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));
	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵が使用されている

			//テクスチャーの設定
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);

		}
	}
}
//=============================================================================
//敵の設定処理
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, int nLife, int nType)
{
	int nCntEnemy;

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{

		if (g_aEnemy[nCntEnemy].bUse == false)
		{//敵が使用されない
			g_aEnemy[nCntEnemy].pos = pos;
			if (nType==0)
			{
				g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
			}
			else if (nType == 1)
			{
				g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
			}
			else if (nType == 2)
			{
				g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
			}
		
			g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nCntEnemy].fStopY_00 = (float)(rand() % 200 + 50);
			g_aEnemy[nCntEnemy].fStopY_01 = (float)(rand() % 400 + 200);
			g_aEnemy[nCntEnemy].nType = nType;
			g_aEnemy[nCntEnemy].nLife = nLife;
			g_aEnemy[nCntEnemy].nHitDamage = 3;
			g_aEnemy[nCntEnemy].nCounterState = rand() % 100 + 1;
			g_aEnemy[nCntEnemy].nWaitTime = rand() % 100 + 1;
			g_aEnemy[nCntEnemy].nExistenceTime = 240;
			g_aEnemy[nCntEnemy].nCounterAnim = 0;
			g_aEnemy[nCntEnemy].nPatternAnim = 0;
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
			g_aEnemy[nCntEnemy].bCharge = false;
			
			//デフォルトカラー
			pVtx[0].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].Col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//頂点座標の設定
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI + g_fAngleEnemy)) * g_fLengthEnemy;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI + g_fAngleEnemy)) * g_fLengthEnemy;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI - g_fAngleEnemy)) * g_fLengthEnemy;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI - g_fAngleEnemy)) * g_fLengthEnemy;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[3].pos.z = 0.0f;

			if (nType == 1)
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
			}
			else
			{
	
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			g_aEnemy[nCntEnemy].bUse = true;//使用している状態にする
			g_nNumEnemy++;//敵の総数を加算
			break;
		}
		pVtx += 4;
	}
	//頂点バッファをあんろっくする
	g_pVtxBuffEnemy->Unlock();
}
//=============================================================================
//敵のヒット処理
//=============================================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	int nScore;
	VERTEX_2D* pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{//体力がゼロになったら
		 //敵の種類ごとにスコアを変える
		switch (g_aEnemy[nCntEnemy].nType)
		{
		case 0:
			
			if (GetShotPower() < 99.0f)
			{
				nScore = GetShotPower() * 111;
				
			}
			else
			{
				nScore = GetShotPower() * 222;
				
			}
			AddScore(nScore);
			break;

		case 1:
			if (GetShotPower() < 99.0f)
			{
				nScore = GetShotPower() * 222;
	
			}
			else
			{
				nScore = GetShotPower() * 444;
				
			}
			AddScore(nScore);
			
			StopSound(SOUND_LABEL_SE_Enemy1);
			break;

		case 2:
			if (GetShotPower() < 99.0f)
			{
				nScore = GetShotPower() * 111;
				
			}
			else
			{
				nScore = GetShotPower() * 222;
			}
			AddScore(nScore);
			break;

		}

		//爆発の設定
		SetExplosion(g_aEnemy[nCntEnemy].pos, D3DCOLOR_RGBA(255, 255, 255, 255),1);
		g_aEnemy[nCntEnemy].bUse = false;//使用している
		g_nNumEnemy--;//敵の総数を減らす
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
	}
	else
	{//ゼロじゃないとき
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCounterState = 5;//ダメージ状態をたもつ時間
		pVtx += nCntEnemy * 4;
		//頂点カラーの設定
		pVtx[0].Col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].Col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].Col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].Col = D3DCOLOR_RGBA(255, 0, 0, 255);
	}

	//頂点バッファをあんろっくする
	g_pVtxBuffEnemy->Unlock();

}
//=============================================================================
//プレイヤーの当たり判定
//=============================================================================
void CollisionPlayer(Enemy* pEnemy)
{
	Player* pPlayer = GetPlayer();
	if (pPlayer->state != pPlayer->bUse == true)
	{
		if (pEnemy->pos.x >= pPlayer->pos.x - 50.0f &&
			pEnemy->pos.x <= pPlayer->pos.x + 50.0f &&
			pEnemy->pos.y >= pPlayer->pos.y - 50.0f &&
			pEnemy->pos.y <= pPlayer->pos.y + 50.0f)
		{
			HitPlayer(pEnemy->nHitDamage);

			// 敵から離れる方向に弾く
			if (pPlayer->pos.x < pEnemy->pos.x)
			{
				pPlayer->pos.x = pEnemy->pos.x;
				pPlayer->move.x -= 10.0f;
			}
			else
			{
				pPlayer->pos.x = pEnemy->pos.x;
				pPlayer->move.x += 10.0f;
			}

			if (pPlayer->pos.y < pEnemy->pos.y)
			{
				pPlayer->pos.y = pEnemy->pos.y;
				pPlayer->move.y -= 10.0f;
			}
			else
			{
				pPlayer->pos.y = pEnemy->pos.y;
				pPlayer->move.y += 10.0f;
			}
		}
	}
}
//=============================================================================
//敵の総数を取得
//=============================================================================
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}
//=============================================================================
//敵のランダム位置（X）を取得
//=============================================================================
float GetRandomEnemyX(void)
{
	return (float)(rand() % (int)(GAME_BG_RIGHT - GAME_BG_LEFT - 50) + (int)(GAME_BG_LEFT + 25));
}
//=============================================================================
//敵の取得
//=============================================================================
Enemy* GetEnemy(void)
{
	return &g_aEnemy[0]; //敵の情報の先頭アドレスを返す
}