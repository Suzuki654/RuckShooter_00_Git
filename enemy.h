//=============================================================================
//
//敵を管理するクラスの宣言[enemy.h]
//Author:鈴木哲汰
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
//マクロ定義
#define MAX_ENEMY (128)//敵の最大数

//敵の状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,//通常状態
	ENEMYSTATE_DAMAGE,//ダメージ状態
	ENEMYSTATE_MAX

}ENEMYSTATE;

//敵構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	float fStopY_00; // 敵が止まるY座標
	float fStopY_01; // 敵が止まるY座標

	int nType; //種類
	int nLife;//体力
	int nHitDamage;//あたった時のダメージ

	ENEMYSTATE state;//状態
	int nCounterState;//状態カウンター
	int nWaitTime;//待機時間
	int nExistenceTime;//存在時間
	
	int nCounterAnim;   // アニメーションカウンター
	int nPatternAnim;   // アニメーション番号

	bool bCharge;//突進しているかどうか
	bool bUse;//使用しているかどうか

}Enemy;
//プロとタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos,int nLife, int nType);
void HitEnemy(int nCntEnemy, int nDamage);
void CollisionPlayer(Enemy* pEnemy);
int GetNumEnemy(void);
float GetRandomEnemyX(void);
Enemy* GetEnemy(void);
#endif // !_ENEMY_H_

