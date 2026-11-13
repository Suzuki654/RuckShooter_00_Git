//=============================================================================
//
//弾の処理実装ファイル[bullet.cpp]
//Author:鈴木哲汰
//
//=============================================================================
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "effect.h"
#include "player.h"
#include "boss.h"

//マクロ定義
#define MAX_BULLET (128)//弾の最大数
#define BULLET_WIDTH  (50.0f)//幅
#define BULLET_HEIGHT (50.0f)//縦

#define PLAYER_BULLET_WIDTH  (10.0f)//幅
#define PLAYER_BULLET_HEIGHT (50.0f)//縦

#define ENEMY_BULLET_WIDTH   (30.0f)//幅
#define ENEMY_BULLET_HEIGHT  (30.0f)//縦

#define NUM_BULLET (2)//弾の種類


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBullet[NUM_BULLET] = {};//テクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;//頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];//弾の情報

//=============================================================================
//弾の初期化処理
//=============================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャーの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BulletHeaven\\Snipe1.png",
		&g_pTextureBullet[0]);

	D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\New_All_Fire_Bullet_Pixel_16x16\\All_Fire_Bullet_Pixel_16x16_001.png",
			&g_pTextureBullet[1]);

	

	//弾の初期化処理
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nPower = 1;
		g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER;
		g_aBullet[nCntBullet].nLife = 100;
		g_aBullet[nCntBullet].bUse = false;
		g_aBullet[nCntBullet].fLength = sqrtf(BULLET_WIDTH * BULLET_WIDTH + BULLET_HEIGHT * BULLET_HEIGHT) * 0.5f;
		g_aBullet[nCntBullet].fAngle =atan2f(BULLET_WIDTH, BULLET_HEIGHT);
	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4*MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet <MAX_BULLET; nCntBullet++)
	{
		// 頂点座標の設定
		pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x +sinf(g_aBullet[nCntBullet].rot.z +(D3DX_PI + g_aBullet[nCntBullet].fAngle))* g_aBullet[nCntBullet].fLength;
		pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y +cosf(g_aBullet[nCntBullet].rot.z +(D3DX_PI + g_aBullet[nCntBullet].fAngle))* g_aBullet[nCntBullet].fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x +sinf(g_aBullet[nCntBullet].rot.z +(D3DX_PI - g_aBullet[nCntBullet].fAngle))* g_aBullet[nCntBullet].fLength;
		pVtx[1].pos.y =g_aBullet[nCntBullet].pos.y +cosf(g_aBullet[nCntBullet].rot.z +(D3DX_PI - g_aBullet[nCntBullet].fAngle))* g_aBullet[nCntBullet].fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x =g_aBullet[nCntBullet].pos.x +sinf(g_aBullet[nCntBullet].rot.z -g_aBullet[nCntBullet].fAngle)* g_aBullet[nCntBullet].fLength;
		pVtx[2].pos.y =g_aBullet[nCntBullet].pos.y +cosf(g_aBullet[nCntBullet].rot.z -g_aBullet[nCntBullet].fAngle)* g_aBullet[nCntBullet].fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x =g_aBullet[nCntBullet].pos.x +sinf(g_aBullet[nCntBullet].rot.z +g_aBullet[nCntBullet].fAngle)* g_aBullet[nCntBullet].fLength;
		pVtx[3].pos.y =g_aBullet[nCntBullet].pos.y +cosf(g_aBullet[nCntBullet].rot.z +g_aBullet[nCntBullet].fAngle)* g_aBullet[nCntBullet].fLength;
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
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
//弾の終了処理
//=============================================================================
void UninitBullet(void)
{
	int nCntBullet;
	//テクスチャーの破棄
	for (nCntBullet = 0; nCntBullet < NUM_BULLET; nCntBullet++)
	{
		if (g_pTextureBullet[nCntBullet] != NULL)
		{
			g_pTextureBullet[nCntBullet]->Release();
			g_pTextureBullet[nCntBullet] = NULL;
		}
	}
	
	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
// 弾の更新処理
//=============================================================================
void UpdateBullet(void)
{
	int nCntBullet;
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{//プレイヤーの弾
				CollisionEnemy(&g_aBullet[nCntBullet]);
				CollisionBoss(&g_aBullet[nCntBullet]);
				
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{//敵の弾
				CollisionPlayer(&g_aBullet[nCntBullet]);
				
			}
		}
	}

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている
		   //位置を更新
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;

			// 頂点座標の設定
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (D3DX_PI + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (D3DX_PI + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.z = 0.0f;

			if (g_aBullet[nCntBullet].pos.x < 0 || g_aBullet[nCntBullet].pos.x > SCREEN_WIDTH ||
				g_aBullet[nCntBullet].pos.y < 0 || g_aBullet[nCntBullet].pos.y > SCREEN_HEIGHT)
			{
				g_aBullet[nCntBullet].bUse = false;//使用してない状態にする

			}
			//寿命のカウントダウン
			g_aBullet[nCntBullet].nLife--;

			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				g_aBullet[nCntBullet].bUse = false;//使用してない状態にする
			}
			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				SetExplosion(g_aBullet[nCntBullet].pos, D3DCOLOR_RGBA(255, 255, 255, 255),0);
				g_aBullet[nCntBullet].bUse = false;//使用してない状態にする
			}
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
//弾の描画処理
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));
	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている
			//テクスチャーの設定
			pDevice->SetTexture(0, g_pTextureBullet[g_aBullet[nCntBullet].type]);
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntBullet*4 , 2);
		}
	}


}

//=============================================================================
//弾の設定処理
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move,int nLife, int nPower, BULLETTYPE type)
{
	int nCntBullet;
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse==false)
		{//弾が使用されていない
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].nLife = nLife;
			g_aBullet[nCntBullet].nPower = nPower;
			g_aBullet[nCntBullet].type = type;
			if (type == BULLETTYPE_PLAYER)
			{
				// プレイヤー弾
				g_aBullet[nCntBullet].fLength = sqrtf(PLAYER_BULLET_WIDTH * PLAYER_BULLET_WIDTH +PLAYER_BULLET_HEIGHT * PLAYER_BULLET_HEIGHT) * 0.5f;
				g_aBullet[nCntBullet].fAngle = atan2f(PLAYER_BULLET_WIDTH, PLAYER_BULLET_HEIGHT);
		
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			else if (type == BULLETTYPE_ENEMY)
			{
				// 敵弾
				g_aBullet[nCntBullet].fLength = sqrtf(ENEMY_BULLET_WIDTH * ENEMY_BULLET_WIDTH + ENEMY_BULLET_HEIGHT * ENEMY_BULLET_HEIGHT) * 0.5f;
				g_aBullet[nCntBullet].fAngle = atan2f(ENEMY_BULLET_WIDTH, ENEMY_BULLET_HEIGHT);
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].rot.z = atan2f(move.x, move.y) + D3DX_PI;

			//頂点座標の設定
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (D3DX_PI + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (D3DX_PI + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.z = 0.0f;

			
			
			g_aBullet[nCntBullet].bUse = true;//使用している状態にする
			break;
		}
		pVtx += 4;

	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}
//=============================================================================
//敵の当たり判定
//=============================================================================
void CollisionEnemy(Bullet* pBullet)
{
	int nCntBullet;
	int nCntEnemy;
	Enemy* pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			if (pBullet->pos.x >= pEnemy->pos.x - 50.0f &&
				pBullet->pos.x <= pEnemy->pos.x + 50.0f &&
				pBullet->pos.y >= pEnemy->pos.y - 50.0f &&
				pBullet->pos.y <= pEnemy->pos.y + 50.0f)
			{
				HitEnemy(nCntEnemy, pBullet->nPower);
				pBullet->bUse = false;
				break;
			}
		}
	}
}
//=============================================================================
//プレイヤーの当たり判定
//=============================================================================
void CollisionPlayer(Bullet* pBullet)
{
	Player* pPlayer = GetPlayer();
	if (pPlayer->bUse == true)
	{
		if (pBullet->pos.x >= pPlayer->pos.x - 20.0f &&
			pBullet->pos.x <= pPlayer->pos.x + 20.0f &&
			pBullet->pos.y >= pPlayer->pos.y - 20.0f &&
			pBullet->pos.y <= pPlayer->pos.y + 20.0f)
		{
			HitPlayer( pBullet->nPower);
			pBullet->bUse = false;
			
		}
	}
}
//=============================================================================
//ボスの当たり判定
//=============================================================================
void CollisionBoss(Bullet* pBullet)
{
	Boss* pBoss = GetBoss();

	if (pBoss->bUse == true)
	{
		if (pBullet->pos.x >= pBoss->pos.x - BOSS_WIDTH * 0.5f &&
			pBullet->pos.x <= pBoss->pos.x + BOSS_WIDTH * 0.5f &&
			pBullet->pos.y >= pBoss->pos.y - BOSS_HEIGHT * 0.5f &&
			pBullet->pos.y <= pBoss->pos.y + BOSS_HEIGHT * 0.5f)
		{
			HitBoss(pBullet->nPower);

			pBullet->bUse = false;
		}
	}
}