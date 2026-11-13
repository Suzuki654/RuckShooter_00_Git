//=============================================================================
//
//弾を管理するクラスの宣言[bullet.h]
//Author:鈴木哲汰
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"
//弾の種類
typedef enum
{
	BULLETTYPE_PLAYER = 0,//プレイヤーの弾
	BULLETTYPE_ENEMY,//敵の弾
	BULLETTYPE_MAX

}BULLETTYPE;

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move; //移動量
	D3DXVECTOR3 rot;//向き

	float fLength;
	float fAngle;

	int nLife;//寿命
	int nPower;//威力

	BULLETTYPE type;//種類
	
	bool bUse;//使用しているかどうか
}Bullet;
//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, int nPower, BULLETTYPE type);
void CollisionEnemy(Bullet* pBullet);
void CollisionPlayer(Bullet* pBullet);
void CollisionBoss(Bullet* pBullet);
#endif // !_BULLET_H_

