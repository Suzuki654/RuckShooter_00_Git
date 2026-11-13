//=============================================================================
//
//レーザーを管理するクラスの宣言[laser.h]
//Author:鈴木哲汰
//
//=============================================================================
#ifndef _LASER_H_
#define _LASER_H_
#include"main.h"
// レーザーの設定
#define MAX_LASER (32)

// レーザーの種類
typedef enum
{
	LASERTYPE_HORIZONTAL = 0,//横レーザー
	LASERTYPE_VERTICAL,
	LASERTYPE_MAX

}LASERTYPE;

// レーザー情報
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量

	float fWidth;		//幅
	float fHeight;		//高さ

	int nCounter;		//表示時間
	int nType;			//種類
	float nPower;         //攻撃力
	
	int nCounterAnim;   // アニメーションカウンター
	int nPatternAnim;   // アニメーション番号

	bool bUse;			//使用中か

}Laser;

//プロトタイプ宣言
void InitLaser(void);
void UninitLaser(void);
void UpdateLaser(void);
void DrawLaser(void);
void SetLaser(D3DXVECTOR3 pos, float fWidth, float fHeight, float nPower, int nLife);
void CollisionPlayer(Laser* pLaser);

#endif // !_LASER_H_


