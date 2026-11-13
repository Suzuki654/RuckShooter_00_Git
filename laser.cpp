//=============================================================================
//
//レーザーの処理実装ファイル[laser.cpp]
//Author:鈴木哲汰
//
//=============================================================================
#include "laser.h"
#include "player.h"
#include "sound.h"
//マクロ定義
#define LASER_WIDTH  (500.0f)
#define LASER_HEIGHT (20.0f)

#define NUM_LASER (2)
// グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureLaser = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLaser = NULL;
Laser g_aLaser[MAX_LASER];
//=============================================================================
//レーザーの初期化処理
//=============================================================================
void InitLaser(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntLaser;

	pDevice = GetDevice();

	// テクスチャ読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\Laser.png",
		&g_pTextureLaser);

	// レーザー情報の初期化
	for (nCntLaser = 0; nCntLaser < MAX_LASER; nCntLaser++)
	{
		g_aLaser[nCntLaser].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aLaser[nCntLaser].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aLaser[nCntLaser].fWidth = 0.0f;
		g_aLaser[nCntLaser].fHeight = 0.0f;
		g_aLaser[nCntLaser].nCounter = 0;
		g_aLaser[nCntLaser].nType = LASERTYPE_HORIZONTAL;
		g_aLaser[nCntLaser].nPower = 0;
		g_aLaser[nCntLaser].nCounterAnim = 0;
		g_aLaser[nCntLaser].nPatternAnim = 0;
		g_aLaser[nCntLaser].bUse = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_LASER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLaser,
		NULL);

	VERTEX_2D* pVtx;

	g_pVtxBuffLaser->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntLaser = 0; nCntLaser < MAX_LASER; nCntLaser++)
	{
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].Col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.25f);
		pVtx[3].tex = D3DXVECTOR2(0.25f, 0.25f);

		pVtx += 4;
	}

	g_pVtxBuffLaser->Unlock();

}
//=============================================================================
//レーザーの終了処理
//=============================================================================
void UninitLaser(void)
{
	// テクスチャの破棄
	if (g_pTextureLaser != NULL)
	{
		g_pTextureLaser->Release();
		g_pTextureLaser = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffLaser != NULL)
	{
		g_pVtxBuffLaser->Release();
		g_pVtxBuffLaser = NULL;
	}
}
//=============================================================================
//レーザーの更新処理
//=============================================================================
void UpdateLaser(void)
{
	int nCntLaser;
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データポインタを取得
	g_pVtxBuffLaser->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntLaser = 0; nCntLaser < MAX_LASER; nCntLaser++)
	{
		if (g_aLaser[nCntLaser].bUse == true)
		{

			// 寿命を減らす
			g_aLaser[nCntLaser].nCounter--;

			//プレイヤーとの当たり判定
			CollisionPlayer(&g_aLaser[nCntLaser]);

			// アニメーション
			g_aLaser[nCntLaser].nCounterAnim++;
			if (g_aLaser[nCntLaser].nCounterAnim >= 1)
			{
				g_aLaser[nCntLaser].nCounterAnim = 0;
				g_aLaser[nCntLaser].nPatternAnim++;

				if (g_aLaser[nCntLaser].nPatternAnim >= 2)
				{
					g_aLaser[nCntLaser].nPatternAnim = 0;
				}

				float fUCoord = g_aLaser[nCntLaser].nPatternAnim * 0.5f;

				pVtx[0].tex = D3DXVECTOR2(fUCoord, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(fUCoord + 0.25f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(fUCoord, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(fUCoord + 0.25f, 1.0f);
				
			}

			// 寿命がなくなったら消す
			if (g_aLaser[nCntLaser].nCounter <= 0)
			{
				g_aLaser[nCntLaser].bUse = false;
				StopSound(SOUND_LABEL_SE_Enemy2);
			}
		}
		pVtx += 4;
	}
	//頂点バッファをあんろっくする
	g_pVtxBuffLaser->Unlock();
}
//=============================================================================
//レーザーの描画処理
//=============================================================================
void DrawLaser(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntLaser;

	pDevice = GetDevice();

	// 頂点バッファを設定
	pDevice->SetStreamSource(0, g_pVtxBuffLaser, 0, sizeof(VERTEX_2D));
	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャ設定
	pDevice->SetTexture(0, g_pTextureLaser);

	for (nCntLaser = 0; nCntLaser < MAX_LASER; nCntLaser++)
	{
		if (g_aLaser[nCntLaser].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLaser * 4, 2);
		}
	}
}
//=============================================================================
//レーザーの設定処理
//=============================================================================
void SetLaser(D3DXVECTOR3 pos, float fWidth, float fHeight, float nPower, int nLife)
{
	int nCntLaser;
	VERTEX_2D* pVtx;

	// 頂点バッファをロック
	g_pVtxBuffLaser->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntLaser = 0; nCntLaser < MAX_LASER; nCntLaser++)
	{
		if (g_aLaser[nCntLaser].bUse == false)
		{
			// レーザー情報を設定
			g_aLaser[nCntLaser].pos = pos;
			g_aLaser[nCntLaser].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aLaser[nCntLaser].fWidth = fWidth;
			g_aLaser[nCntLaser].fHeight = fHeight;
			g_aLaser[nCntLaser].nCounter = nLife;
			g_aLaser[nCntLaser].nType = LASERTYPE_HORIZONTAL;
			g_aLaser[nCntLaser].nPower= nPower;
			g_aLaser[nCntLaser].bUse = true;

			pVtx[0].pos.x = pos.x - fWidth * 0.5f;
			pVtx[0].pos.y = pos.y - fHeight * 0.5f;
			pVtx[0].pos.z = 0.0f;
			
			pVtx[1].pos.x = pos.x + fWidth * 0.5f;
			pVtx[1].pos.y = pos.y - fHeight * 0.5f;
			pVtx[1].pos.z = 0.0f;
		
			pVtx[2].pos.x = pos.x - fWidth * 0.5f;
			pVtx[2].pos.y = pos.y + fHeight * 0.5f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pos.x + fWidth * 0.5f;
			pVtx[3].pos.y = pos.y + fHeight * 0.5f;
			pVtx[3].pos.z = 0.0f;
			break;
		}
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffLaser->Unlock();

}
//=============================================================================
//プレイヤーの当たり判定
//=============================================================================
void CollisionPlayer(Laser* pLaser)
{
	Player* pPlayer = GetPlayer();
	if (pPlayer->bUse == true)
	{
		// レーザーの範囲
		float fLaserLeft = pLaser->pos.x - pLaser->fWidth * 0.5f;
		float fLaserRight = pLaser->pos.x + pLaser->fWidth * 0.5f;
		float fLaserTop = pLaser->pos.y - pLaser->fHeight * 0.5f;
		float fLaserBottom = pLaser->pos.y + pLaser->fHeight * 0.5f;

		// プレイヤーの範囲
		float fPlayerLeft = pPlayer->pos.x - 20.0f;
		float fPlayerRight = pPlayer->pos.x + 20.0f;
		float fPlayerTop = pPlayer->pos.y - 20.0f;
		float fPlayerBottom = pPlayer->pos.y + 20.0f;

		// 矩形の当たり判定
		if (fLaserLeft < fPlayerRight &&
			fLaserRight > fPlayerLeft &&
			fLaserTop < fPlayerBottom &&
			fLaserBottom > fPlayerTop)
		{
			HitPlayer(pLaser->nPower);
			if (pPlayer->pos.x < fPlayerLeft)
			{
				pPlayer->pos.x = fPlayerLeft;
				pPlayer->move.x -= 10.0f;
			}
			else
			{
				pPlayer->pos.x = fPlayerRight;
				pPlayer->move.x += 10.0f;
			}

			if (pPlayer->pos.y < fPlayerTop)
			{
				pPlayer->pos.y = fLaserTop;
				pPlayer->move.y -= 10.0f;
			}
			else
			{
				pPlayer->pos.y = fLaserBottom;
				pPlayer->move.y += 10.0f;
			}
		}
	}
}