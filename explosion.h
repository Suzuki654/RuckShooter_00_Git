//=============================================================================
//
//爆発エフェクトを管理するクラスの宣言[explosion.h]
//Author:鈴木哲汰
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include"main.h"

typedef enum
{
	EXPLOSION_NORMAL = 0,
	EXPLOSION_ENEMY,
	EXPLOSION_BOSS,
	EXPLOSION_MAX

}EXPLOSIONEFFECT;

//爆発構造体の定義
typedef struct
{
	D3DXVECTOR3 pos; //位置
	D3DXCOLOR col;//色
	int nType; //種類
	int nCounterAnim;//アニメーションカウンター
	int nPatternAnim;//アニメーションポインター
	bool bUse;//使用しているかどうか
}Explosion;

//プロトタイプ宣言
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, int nType);

#endif // _EXPLOSION_H_



