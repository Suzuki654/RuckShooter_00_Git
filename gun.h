//=============================================================================
//
//銃を管理するクラスの宣言[gun.h]
//Author:鈴木哲汰
//
//=============================================================================
#ifndef _GUN_H_
#define _GUN_H_
#include "main.h"

//銃構造体
typedef struct
{
	D3DXVECTOR3 pos; //位置
	D3DXVECTOR3 move;//移動量
	int nCounterAnim;//アニメーションカウンター
	int nPatternAnim;//アニメーションパターンNo.
	D3DXVECTOR3 rot;//向き
	float fLength;//対角線の長さ
	float fAngle;//対角線の角度
}Gun;

//プロトタイプ宣言
void InitGun(void);
void UninitGun(void);
void UpdateGun(void);
void DrawGun(void);


#endif // !_GUN_H_
