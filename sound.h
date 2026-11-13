//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_TUTORIAL000,    // TutorialBGM0
	SOUND_LABEL_BOSS000,        // BossBGM0
	SOUND_LABEL_SE_CURSOR000,
	SOUND_LABEL_SE_DECISION000,
	SOUND_LABEL_SE_DECISION001,
	SOUND_LABEL_SE_SHOT,		// 弾発射音
	SOUND_LABEL_SE_HIT,			// ヒット音
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_SUCCSESS,
	SOUND_LABEL_SE_CHARGE000,
	SOUND_LABEL_SE_WALL000,
	SOUND_LABEL_SE_GAMECLEAR,
	SOUND_LABEL_SE_GAMEOVER,
	SOUND_LABEL_SE_Enemy1,
	SOUND_LABEL_SE_Enemy2,
	SOUND_LABEL_SE_BossDamage,
	SOUND_LABEL_SE_BossExplosion,
	SOUND_LABEL_SE_PlayerDeath,
	SOUND_LABEL_SE_Wave000,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
