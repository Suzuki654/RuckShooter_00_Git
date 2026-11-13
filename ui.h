//=============================================================================
//
//UIを管理するクラスの宣言[ui.h]
//Author:鈴木哲汰
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

#include "main.h"

//UI列挙型
typedef enum
{
	UI_TEX_HP_FRAME,
	UI_TEX_HP_GAUGE,
	UI_TEX_HP_NAME,

	UI_TEX_CHARGE_FRAME,
	UI_TEX_CHARGE_GAUGE,

	UI_TEX_WAVE_01,
	UI_TEX_WAVE_02,
	UI_TEX_WAVE_03,
	UI_TEX_FINAL_WAVE,

	UI_TEX_GAMEOVER,
	UI_TEX_GAMECLEAR,
	UI_TEX_RESULT,
	UI_TEX_RETRY,
	UI_TEX_TITLE,


	UI_TEX_BOSS_HP_FRAME,
	UI_TEX_BOSS_HP_GAUGE,

	UI_TEX_PAUSE,

	UI_TEX_TUTORIAL_SKIP,

	UI_TEX_GAME_WASD,
	UI_TEX_GAME_SPACE,

	UI_TEX_MAX
}UI_TEXTURE;

//UI頂点番号
typedef enum
{
	UI_VTX_HP_FRAME = 0,
	UI_VTX_HP_GAUGE = 4,
	UI_VTX_HP_NAME = 8,

	UI_VTX_CHARGE_FRAME = 12,
	UI_VTX_CHARGE_GAUGE = 16,

	UI_VTX_WAVE = 20,

	UI_VTX_GAMEEND = 24,
	UI_VTX_RESULT = 28,
	UI_VTX_RETRY = 32,
	UI_VTX_TITLE = 36,

	UI_VTX_BOSS_HP_FRAME = 40,
	UI_VTX_BOSS_HP_GAUGE = 44,

	UI_VTX_PAUSE = 48,

	UI_VTX_TUTORIAL_SKIP = 52,

	UI_VTX_GAME_WASD = 56,
	UI_VTX_GAME_SPACE = 60,

	UI_VTX_MAX = 68

}UI_VERTEX;



//プロトタイプ宣言
void InitUI(void);
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);
void DrawGameWave(int nWave);
void DrawGameEnd(bool bGameClear);
void InitGamePause(void);
void DrawGamePause(void);
void DrawTutorialUI(void);
void DrawGameUI(void);



#endif // !_UI_H_
