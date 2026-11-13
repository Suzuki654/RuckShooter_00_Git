//=============================================================================
//
//爆発の処理実装ファイル[explosion.cpp]
//Author:鈴木哲汰
//
//=============================================================================
#include"explosion.h"

//マクロ定義
#define MAX_EXPLOSION (128) //爆発の最大数
#define NUM_EXPLOSION (3)   //爆発の種類


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureExplosion[NUM_EXPLOSION] = {};//テクスチャーへのポインター
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;//頂点バッファへのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];//爆発の情報

//=============================================================================
//爆発の初期化処理
//=============================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャーの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\explosion000.png",
		&g_pTextureExplosion[EXPLOSION_NORMAL]);
	
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\explosion_Enemy.png",
		&g_pTextureExplosion[EXPLOSION_ENEMY]);
	
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\explosion_Boss.png",
		&g_pTextureExplosion[EXPLOSION_BOSS]);
	
	

	//爆発の初期化処理
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{

		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DCOLOR_RGBA(255,255,255,255);
		g_aExplosion[nCntExplosion].nType = 0;
		g_aExplosion[nCntExplosion].nCounterAnim = 0;//カウンターを初期化する
		g_aExplosion[nCntExplosion].nPatternAnim = 0; // パターンNo.を初期化する
		g_aExplosion[nCntExplosion].bUse = false;
	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 50.0f, g_aExplosion[nCntExplosion].pos.y - 50.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 50.0f, g_aExplosion[nCntExplosion].pos.y - 50.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 50.0f, g_aExplosion[nCntExplosion].pos.y + 50.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 50.0f, g_aExplosion[nCntExplosion].pos.y + 50.0f, 0.0f);
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
		pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}
//=============================================================================
//爆発の終了処理
//=============================================================================
void UninitExplosion(void)
{

	int nCntExplosion ;
	//テクスチャーの破棄
	for (nCntExplosion  = 0; nCntExplosion  < NUM_EXPLOSION; nCntExplosion ++)
	{
		if (g_pTextureExplosion [nCntExplosion ] != NULL)
		{
			g_pTextureExplosion [nCntExplosion ]->Release();
			g_pTextureExplosion [nCntExplosion ] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}
//=============================================================================
//爆発の更新処理
//=============================================================================
void UpdateExplosion(void)
{
	int nCntExplosion;
	//アニメーション
	float fUCoord, fVCoord;

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{//爆発が使用されている
		

		g_aExplosion[nCntExplosion].nCounterAnim++;//カウンタを加算
		if ((g_aExplosion[nCntExplosion].nCounterAnim % 4) == 0)
		{//一定時間経過

		   //パターンNo.を更新する
			g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim+1 ) % 60;

			

			
			if (g_aExplosion[nCntExplosion].nPatternAnim >= 8)
			{
				g_aExplosion[nCntExplosion].bUse = false;
		
				pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				
			}
			else
			{
				// 横に8分割されたテクスチャを想定
				fUCoord = (g_aExplosion[nCntExplosion].nPatternAnim % 8) * 0.125f;
				fVCoord = (g_aExplosion[nCntExplosion].nPatternAnim / 8) * 1.0f;

				// テクスチャー座標の再設定
				pVtx[0].tex = D3DXVECTOR2(fUCoord, fVCoord);
				pVtx[1].tex = D3DXVECTOR2(fUCoord + 0.125f, fVCoord);
				pVtx[2].tex = D3DXVECTOR2(fUCoord, fVCoord + 1.0f);
				pVtx[3].tex = D3DXVECTOR2(fUCoord + 0.125f, fVCoord + 1.0f);
			}
			
			
		}
		pVtx += 4;
	}
	//頂点バッファをあんろっくする
	g_pVtxBuffExplosion->Unlock();
}
//=============================================================================
//爆発の描画処理
//============================================================================= 
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));
	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);
	

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//弾が使用されている

			//テクスチャーの設定
			pDevice->SetTexture(0, g_pTextureExplosion[g_aExplosion [nCntExplosion].nType]);
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}

}
//=============================================================================
//爆発の設定処理
//============================================================================= 
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, int nType)
{
	int nCntExplosion;
VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse==false)
		{//弾が使用されていない
			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;
			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion [nCntExplosion ].nType = nType;
			
			if (nType == EXPLOSION_NORMAL)
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 50.0f, g_aExplosion[nCntExplosion].pos.y - 50.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 50.0f, g_aExplosion[nCntExplosion].pos.y - 50.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 50.0f, g_aExplosion[nCntExplosion].pos.y + 50.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 50.0f, g_aExplosion[nCntExplosion].pos.y + 50.0f, 0.0f);
			}
			else if (nType == EXPLOSION_ENEMY)
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 40.0f, g_aExplosion[nCntExplosion].pos.y - 40.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 40.0f, g_aExplosion[nCntExplosion].pos.y - 40.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 40.0f, g_aExplosion[nCntExplosion].pos.y + 40.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 40.0f, g_aExplosion[nCntExplosion].pos.y + 40.0f, 0.0f);
			}
			else if (nType == EXPLOSION_BOSS)
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 500.0f, g_aExplosion[nCntExplosion].pos.y - 200.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 500.0f, g_aExplosion[nCntExplosion].pos.y - 200.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 500.0f, g_aExplosion[nCntExplosion].pos.y + 200.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 500.0f, g_aExplosion[nCntExplosion].pos.y + 200.0f, 0.0f);
			}
		
			

			g_aExplosion[nCntExplosion].col = col;
			//テクスチャー座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);
			
			g_aExplosion[nCntExplosion].bUse = true;//使用している状態にする
			break;
		}
		pVtx += 4;
	}
	//頂点バッファをあんろっくする
	g_pVtxBuffExplosion->Unlock();

}