//=============================================================================
//
//銃の処理実装ファイル[gun.cpp]
//Author:鈴木哲汰
//
//=============================================================================
#include "gun.h"
#include"bullet.h"
#include "input.h"
#include "bg.h"
#include "player.h"
#include "explosion.h"

//マクロ
#define NUM_GUN (2)//タイトルの画像の数

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureGun[NUM_GUN] = {};///テクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGun = NULL;//頂点バッファ
Gun g_Gun;
//=============================================================================
//銃の初期化処理
//=============================================================================
void InitGun(void)
{
	int nCntGun;

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャーの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GunPack\\Guns\\41.png",
		&g_pTextureGun[0]);
	//テクスチャーの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GunArrow.png",
		&g_pTextureGun[1]);

	g_Gun.pos = D3DXVECTOR3(500.0f, 500.0f, 0.0f);//位置を初期化する
	g_Gun.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量を初期化する
	g_Gun.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きを初期化する
	g_Gun.nCounterAnim = 0;//カウンターを初期化する
	g_Gun.nPatternAnim = 0;//パターンNo.を初期化する

	//対角線を算出
	g_Gun.fLength = sqrtf(50.0f * 50.0f + 50.0f * 50.0f) * 0.5f;
	//対角線の角度を算出
	g_Gun.fAngle = atan2f(100.0f, 100.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_GUN,//必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGun,
		NULL);

	VERTEX_2D* pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGun->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntGun = 0; nCntGun < NUM_GUN; nCntGun++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_Gun.pos.x + sinf(g_Gun.rot.z + (D3DX_PI + g_Gun.fAngle)) * g_Gun.fLength;
		pVtx[0].pos.y = g_Gun.pos.y + cosf(g_Gun.rot.z + (D3DX_PI + g_Gun.fAngle)) * g_Gun.fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_Gun.pos.x + sinf(g_Gun.rot.z + (D3DX_PI - g_Gun.fAngle)) * g_Gun.fLength;
		pVtx[1].pos.y = g_Gun.pos.y + cosf(g_Gun.rot.z + (D3DX_PI - g_Gun.fAngle)) * g_Gun.fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_Gun.pos.x + sinf(g_Gun.rot.z - g_Gun.fAngle) * g_Gun.fLength;
		pVtx[2].pos.y = g_Gun.pos.y + cosf(g_Gun.rot.z - g_Gun.fAngle) * g_Gun.fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_Gun.pos.x + sinf(g_Gun.rot.z + g_Gun.fAngle) * g_Gun.fLength;
		pVtx[3].pos.y = g_Gun.pos.y + cosf(g_Gun.rot.z + g_Gun.fAngle) * g_Gun.fLength;
		pVtx[3].pos.z = 0.0f;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntGun == 0)
		{
			//頂点カラーの設定
			pVtx[0].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].Col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		if (nCntGun == 1)
		{
			
			pVtx[0].Col = D3DCOLOR_RGBA(255, 255, 255, 185);
			pVtx[1].Col = D3DCOLOR_RGBA(255, 255, 255, 185);
			pVtx[2].Col = D3DCOLOR_RGBA(255, 255, 255, 185);
			pVtx[3].Col = D3DCOLOR_RGBA(255, 255, 255, 185);
		}
		

		//テクスチャー座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;

	}

	//頂点バッファをアンロックする
	g_pVtxBuffGun->Unlock();
}
//=============================================================================
//銃の終了処理
//=============================================================================
void UninitGun(void)
{
	int nCntGun;
	for (nCntGun = 0; nCntGun < NUM_GUN; nCntGun++)
	{
		//テクスチャーの破棄
		if (g_pTextureGun[nCntGun] != NULL)
		{
			g_pTextureGun[nCntGun]->Release();
			g_pTextureGun[nCntGun] = NULL;
		}

	}
	//頂点バッファの破棄
	if (g_pVtxBuffGun != NULL)
	{
		g_pVtxBuffGun->Release();
		g_pVtxBuffGun = NULL;
	}

}
//=============================================================================
//銃の更新処理
//=============================================================================
void UpdateGun(void)
{

	Player* pPlayer = GetPlayer();

	int nCntBullet;

	// プレイヤーの手元に銃を配置
	// プレイヤーの中心に固定
	g_Gun.pos = pPlayer->pos;

	// プレイヤーと同じ向き
	g_Gun.rot = pPlayer->rot;


	D3DXVECTOR3 pos;

	// 銃口の位置
	float fMuzzle = g_Gun.fLength;
	pos.x = g_Gun.pos.x + sinf(g_Gun.rot.z + D3DX_PI * 0.5f) * fMuzzle;
	pos.y = g_Gun.pos.y + cosf(g_Gun.rot.z + D3DX_PI * 0.5f) * fMuzzle;
	pos.z = 0.0f;

	if (GetKeyboardRelease(DIK_SPACE) == true || GetJoypadRelease(JOYKEY_A) == true || GetJoypadRelease(JOYKEY_RIGHT_SHOULDER) == true)
	{//スペースキーが放されたら


		g_Gun.pos = pPlayer->pos;


		if (pPlayer->fPower < 30.0f)
		{
			// 1発
			//弾の設定
			SetBullet(pos, D3DXVECTOR3(sinf(g_Gun.rot.z + (D3DX_PI * 0.5f)) * 10.0f, cosf(g_Gun.rot.z + (D3DX_PI * 0.5f)) * 10.0f,
				0.0f),
				5,
				5,
				BULLETTYPE_PLAYER);
			SetExplosion(pos, D3DCOLOR_RGBA(255, 255, 255, 255), 0);
		}
		else if (pPlayer->fPower < 70.0f)
		{
			// 3発
			for (nCntBullet = -1; nCntBullet <= 1; nCntBullet++)
			{
				float fAngle = g_Gun.rot.z + D3DX_PI / 12.0f * nCntBullet;

				SetBullet(pos, D3DXVECTOR3(sinf(fAngle + D3DX_PI * 0.5f) * 10.0f, cosf(fAngle + D3DX_PI * 0.5f) * 10.0f,
					0.0f),
					10,
					10,
					BULLETTYPE_PLAYER);
			}
		}
		else if (pPlayer->fPower < 99.0f)
		{
			// 3発
			for (nCntBullet = -1; nCntBullet <= 1; nCntBullet++)
			{
				float fAngle = g_Gun.rot.z + D3DX_PI / 12.0f * nCntBullet;

				SetBullet(pos, D3DXVECTOR3(sinf(fAngle + D3DX_PI * 0.5f) * 10.0f, cosf(fAngle + D3DX_PI * 0.5f) * 10.0f,
					0.0f),
					30,
					10,
					BULLETTYPE_PLAYER);
			}
		}
		else
		{//5発
			for (nCntBullet = -2; nCntBullet <= 2; nCntBullet++)
			{
				float fAngle = g_Gun.rot.z + D3DX_PI / 12.0f * nCntBullet;

				SetBullet(pos, D3DXVECTOR3(sinf(fAngle + D3DX_PI * 0.5f) * 10.0f, cosf(fAngle + D3DX_PI * 0.5f) * 10.0f,
					0.0f),
					100,
					10,
					BULLETTYPE_PLAYER);
			}
		}

		pPlayer->fPower = 0.0f;

	}




	//壁
	if (g_Gun.pos.x >= (BORDEA_BG_RIGHT - 120.0f)) // 右端
	{
		g_Gun.pos.x = BORDEA_BG_RIGHT - 120.0f;
		g_Gun.move.x = -g_Gun.move.x; // 反転
	}
	else if (g_Gun.pos.x <= BORDEA_BG_LEFT + 120.0f) // 左端
	{
		g_Gun.pos.x = BORDEA_BG_LEFT + 120.0f;
		g_Gun.move.x = -g_Gun.move.x; // 反転
	}
	if (g_Gun.pos.y >= (BORDEA_BG_UNDER - 200.0f)) // 下端
	{
		g_Gun.pos.y = BORDEA_BG_UNDER - 200.0f;
		g_Gun.move.y = -g_Gun.move.y; // 反転
	}
	else if (g_Gun.pos.y <= BORDEA_BG_TOP + 150.0f) // 上端
	{
		g_Gun.pos.y = BORDEA_BG_TOP + 150.0f;
		g_Gun.move.y = -g_Gun.move.y; // 反転

	}

	VERTEX_2D* pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGun->Lock(0, 0, (void**)&pVtx, 0);


	//頂点座標の設定
	pVtx[0].pos.x = g_Gun.pos.x + sinf(g_Gun.rot.z + (D3DX_PI + g_Gun.fAngle)) * g_Gun.fLength;
	pVtx[0].pos.y = g_Gun.pos.y + cosf(g_Gun.rot.z + (D3DX_PI + g_Gun.fAngle)) * g_Gun.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Gun.pos.x + sinf(g_Gun.rot.z + (D3DX_PI - g_Gun.fAngle)) * g_Gun.fLength;
	pVtx[1].pos.y = g_Gun.pos.y + cosf(g_Gun.rot.z + (D3DX_PI - g_Gun.fAngle)) * g_Gun.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Gun.pos.x + sinf(g_Gun.rot.z - g_Gun.fAngle) * g_Gun.fLength;
	pVtx[2].pos.y = g_Gun.pos.y + cosf(g_Gun.rot.z - g_Gun.fAngle) * g_Gun.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Gun.pos.x + sinf(g_Gun.rot.z + g_Gun.fAngle) * g_Gun.fLength;
	pVtx[3].pos.y = g_Gun.pos.y + cosf(g_Gun.rot.z + g_Gun.fAngle) * g_Gun.fLength;
	pVtx[3].pos.z = 0.0f;


	//矢印の横幅・縦幅
	float fWidth = 20.0f;
	float fHeight = 250.0f;


	//対角線を算出
	float fLength = sqrtf(fWidth * fWidth + fHeight * fHeight) * 0.5f;
	//対角線の角度を算出
	float fAngle = atan2f(fHeight, fWidth);
	//矢印
	pVtx[4].pos.x = g_Gun.pos.x + sinf(g_Gun.rot.z + (D3DX_PI + fAngle)) * fLength;
	pVtx[4].pos.y = g_Gun.pos.y + cosf(g_Gun.rot.z + (D3DX_PI + fAngle)) * fLength;
	pVtx[4].pos.z = 0.0f;

	pVtx[5].pos.x = g_Gun.pos.x + sinf(g_Gun.rot.z + (D3DX_PI - fAngle)) * fLength;
	pVtx[5].pos.y = g_Gun.pos.y + cosf(g_Gun.rot.z + (D3DX_PI - fAngle)) * fLength;
	pVtx[5].pos.z = 0.0f;

	pVtx[6].pos.x = g_Gun.pos.x + sinf(g_Gun.rot.z - fAngle) * fLength;
	pVtx[6].pos.y = g_Gun.pos.y + cosf(g_Gun.rot.z - fAngle) * fLength;
	pVtx[6].pos.z = 0.0f;

	pVtx[7].pos.x = g_Gun.pos.x + sinf(g_Gun.rot.z + fAngle) * fLength;
	pVtx[7].pos.y = g_Gun.pos.y + cosf(g_Gun.rot.z + fAngle) * fLength;
	pVtx[7].pos.z = 0.0f;
	g_pVtxBuffGun->Unlock();

}
//=============================================================================
//銃の描画処理
//=============================================================================
void DrawGun(void)
{
	int nCntGun;
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGun, 0, sizeof(VERTEX_2D));

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);


	//テクスチャーの設定
	pDevice->SetTexture(0, g_pTextureGun[1]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
	
	//テクスチャーの設定
	pDevice->SetTexture(0, g_pTextureGun[0]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);




}