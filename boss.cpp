//=============================================================================
//
//ボスの処理実装ファイル[boss.cpp]
//Author:鈴木哲汰
//
// NOTE:清野蒼翔 お前は誰だ。私は何だ。君は一体何故生きる。私はいつから私になれた？知るも知らぬも機は己。君もまたそれを知るだろう。
//=============================================================================
#include "boss.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "explosion.h"


//マクロ定義
#define BOSS_WIDTH  (500.0f)
#define BOSS_HEIGHT (250.0f)
// グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBoss = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoss = NULL;
Boss g_Boss;
//=============================================================================
// ボスの初期化処理
//=============================================================================
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャ読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\Boss.png",
		&g_pTextureBoss
	);

	// 頂点バッファ作成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBoss,
		NULL
	);

	// ボスの初期化
	g_Boss.pos = D3DXVECTOR3(638.0f, -50.0f, 0.0f);
	g_Boss.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Boss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Boss.fWidth = BOSS_WIDTH;
	g_Boss.fHeight = BOSS_HEIGHT;

	g_Boss.nLife = 0;
	g_Boss.nMaxLife = 0;

	g_Boss.state = BOSSSTATE_DEAD;
	g_Boss.nCounterState = 0;
	g_Boss.bUse = false;

	g_Boss.nShotTimer = 0;
	g_Boss.nSpawnTimer = 0;
	g_Boss.nRandomEnemy = 0;

	// 頂点バッファ設定
	VERTEX_2D* pVtx;

	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-BOSS_WIDTH * 0.5f, -BOSS_HEIGHT * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BOSS_WIDTH * 0.5f, -BOSS_HEIGHT * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-BOSS_WIDTH * 0.5f, BOSS_HEIGHT * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BOSS_WIDTH * 0.5f, BOSS_HEIGHT * 0.5f, 0.0f);

	for (int nCntBoss = 0; nCntBoss < 4; nCntBoss++)
	{
		pVtx[nCntBoss].rhw = 1.0f;
		pVtx[nCntBoss].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffBoss->Unlock();
}
//=============================================================================
// ボスの終了処理
//=============================================================================
void UninitBoss(void)
{
	if (g_pTextureBoss != NULL)
	{
		g_pTextureBoss->Release();
		g_pTextureBoss = NULL;
	}

	if (g_pVtxBuffBoss != NULL)
	{
		g_pVtxBuffBoss->Release();
		g_pVtxBuffBoss = NULL;
	}

	g_Boss.bUse = false;
}
//=============================================================================
// ボスの更新処理
//=============================================================================
void UpdateBoss(void)
{
	if (g_Boss.bUse == false)
	{
		return;
	}

	// 頂点バッファ設定
	VERTEX_2D* pVtx;

	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Boss.bUse == true)
	{
		CollisionPlayer(&g_Boss);
		switch (g_Boss.state)
		{
		case BOSSSTATE_NORMAL:
			//画面外から
			if (g_Boss.pos.y < 50.0f)
			{
				g_Boss.pos.y += g_Boss.move.y;
			}
			else
			{
				g_Boss.move.y = 0.0f;//止まる

				//弾を打つ
				g_Boss.nShotTimer--;

				if (g_Boss.nShotTimer <= 0)
				{
					// 5方向に弾を撃つ
					for (int nCntBossBullet = 0; nCntBossBullet < 5; nCntBossBullet++)
					{
						float fAngle = -30.0f + nCntBossBullet * 15.0f;

						fAngle *= D3DX_PI / 180.0f;
						float fSpeed = 3.0f;
						D3DXVECTOR3 move;
						move.x = sinf(fAngle) * fSpeed;
						move.y = cosf(fAngle) * fSpeed;
						move.z = 0.0f;


						SetBullet(g_Boss.pos, move, 300, 2, BULLETTYPE_ENEMY);
					}

					// 次の発射まで
					g_Boss.nShotTimer = 120;
				}

				//敵を召喚する
				g_Boss.nSpawnTimer--;

				if (g_Boss.nSpawnTimer <= 0)
				{
					g_Boss.nRandomEnemy = rand() % 3;
					// 雑魚を召喚
					
					SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
					SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, g_Boss.nRandomEnemy);
					
					

					// 次の召喚まで
					g_Boss.nSpawnTimer = 300;
				}

			}

			break;
		case BOSSSTATE_DAMAGE:
			g_Boss.nCounterState--;
			if (g_Boss.nCounterState <= 0)
			{
				g_Boss.state = BOSSSTATE_NORMAL;
				//頂点カラーの設定
				pVtx[0].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].Col = D3DCOLOR_RGBA(255, 255, 255, 255);

			}
			break;
		}
	}
	//バッファをアンロック
	g_pVtxBuffBoss->Unlock();

}
//=============================================================================
// ボスの描画処理
//=============================================================================
void DrawBoss(void)
{
	if (g_Boss.bUse == false)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VERTEX_2D* pVtx;
	//頂点バッファをロック
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	// ボスの位置を設定
	pVtx[0].pos = D3DXVECTOR3(g_Boss.pos.x - BOSS_WIDTH * 0.5f, g_Boss.pos.y - BOSS_HEIGHT * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Boss.pos.x + BOSS_WIDTH * 0.5f, g_Boss.pos.y - BOSS_HEIGHT * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Boss.pos.x - BOSS_WIDTH * 0.5f, g_Boss.pos.y + BOSS_HEIGHT * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Boss.pos.x + BOSS_WIDTH * 0.5f, g_Boss.pos.y + BOSS_HEIGHT * 0.5f, 0.0f);

	//頂点バッファをアンロック
	g_pVtxBuffBoss->Unlock();

	//頂点バッファを設定
	pDevice->SetStreamSource(0, g_pVtxBuffBoss, 0, sizeof(VERTEX_2D));

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTextureBoss);

	//ボスを描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// ボスの設定
//=============================================================================
void SetBoss(D3DXVECTOR3 pos, int nLife)
{
	g_Boss.pos = pos;

	g_Boss.move = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	g_Boss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Boss.fWidth = BOSS_WIDTH;
	g_Boss.fHeight = BOSS_HEIGHT;

	g_Boss.nLife = nLife;
	g_Boss.nMaxLife = nLife;

	g_Boss.state = BOSSSTATE_NORMAL;
	g_Boss.nCounterState = 0;

	g_Boss.bUse = true;

	g_Boss.nShotTimer = 0;
	g_Boss.nSpawnTimer = 0;

}
//=============================================================================
// プレイヤーとの当たり判定
//=============================================================================
void CollisionPlayer(Boss* pBoss)
{
	Player* pPlayer = GetPlayer();

	if (pPlayer->bUse == true)
	{
		if (pPlayer->pos.x >= pBoss->pos.x - BOSS_WIDTH * 0.5f &&
			pPlayer->pos.x <= pBoss->pos.x + BOSS_WIDTH * 0.5f &&
			pPlayer->pos.y >= pBoss->pos.y - BOSS_HEIGHT * 0.5f &&
			pPlayer->pos.y <= pBoss->pos.y + BOSS_HEIGHT * 0.5f)
		{
			HitPlayer(4);

			// プレイヤーを少し下へ押し返す
			pPlayer->move.y += 5.0f;
		}
	}
}
//=============================================================================
// ボスのヒット処理
//=============================================================================
void HitBoss(int nDamage)
{
	Player* pPlayer = GetPlayer();

	if (g_Boss.bUse == false)
	{
		return;
	}
	VERTEX_2D* pVtx;
	//頂点バッファをロック
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	g_Boss.nLife -= nDamage;

	if (g_Boss.nLife <= 0)
	{
		g_Boss.nLife = 0;
		g_Boss.state = BOSSSTATE_DEAD;
		g_Boss.bUse = false;

		SetExplosion(g_Boss.pos, D3DCOLOR_RGBA(255, 255, 255, 255), 2);
		int nClearHP = GetPlayerLife();
		
		int nScore = nClearHP * 499999;
		AddScore(nScore);
		PlaySound(SOUND_LABEL_SE_BossExplosion);
	}
	else
	{
		// ダメージ状態
		g_Boss.state = BOSSSTATE_DAMAGE;
		g_Boss.nCounterState = 5;//ダメージ状態をたもつ時間
		pVtx[0].Col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].Col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].Col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].Col = D3DCOLOR_RGBA(255, 0, 0, 255);

		PlaySound(SOUND_LABEL_SE_BossDamage);
	}
	//頂点バッファをあんろっくする
	g_pVtxBuffBoss->Unlock();
}
//=============================================================================
// ボス情報取得
//=============================================================================
Boss* GetBoss(void)
{
	return &g_Boss;
}

//=============================================================================
// ボス使用中か取得
//=============================================================================
bool GetBossUse(void)
{
	return g_Boss.bUse;
}