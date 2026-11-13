//=============================================================================
//
//敵を管理するクラスの宣言[boss.h]
//Author:鈴木哲汰
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include"main.h"

// ボスの状態
typedef enum
{
	BOSSSTATE_NORMAL = 0,	// 通常
	BOSSSTATE_DAMAGE,		// ダメージ
	BOSSSTATE_DEAD,			// 撃破
	BOSSSTATE_MAX

}BOSSSTATE;


// ボス構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 rot;		// 回転

	float fWidth;			// 幅
	float fHeight;			// 高さ

	int nLife;				// HP
	int nMaxLife;			// 最大HP

	BOSSSTATE state;		// 状態
	int nCounterState;      // 状態カウント

	bool bUse;				// 使用中か

	int nShotTimer;			// 弾を撃つタイマー
	int nSpawnTimer;		// 雑魚召喚タイマー
	int nRandomEnemy;		// 雑魚ランダム召喚

}Boss;

//マクロ定義
#define BOSS_WIDTH  (500.0f)
#define BOSS_HEIGHT (250.0f)

// プロトタイプ宣言
void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
void SetBoss(D3DXVECTOR3 pos, int nLife);
void CollisionPlayer(Boss* pBoss);
void HitBoss(int nDamage);
Boss* GetBoss(void);
bool GetBossUse(void);



#endif // !_BOSS_H_
