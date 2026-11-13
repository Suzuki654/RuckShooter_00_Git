//=============================================================================
//
//ゲーム画面の処理実装ファイル[game.cpp]
//Author:鈴木哲汰
//
//=============================================================================
#include "game.h"
#include "player.h"
#include "bg.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "effect.h"
#include "sound.h"
#include "fade.h"
#include "gun.h"
#include "ui.h"
#include "laser.h"
#include "boss.h"
#include "result.h"

//マクロ定義
#define WAVE_WAIT_TIME (120) //ウェーブ間待ち時間
#define GAMEEND_WAIT_TIME (60) //ゲームオーバー後待ち時間
#define GAMECLEAR_WAIT_TIME (180) //ゲームクリア後待ち時間

//プロトタイプ宣言
void SetGameWave(int nWave, int nPhase);

//グローバル変数
bool g_bPause = false;//ポーズ状態のON/OFF
bool g_bDispDebug = false;//デバック表示のON/OFF
bool g_bGameEnd = false;//ゲーム終了画面のON/OFF
bool g_bGameClear = false;//クリア画面/ゲームオーバー画面
int g_nGameEndSelect = 0;//ゲーム終了画面の選択
int g_nGameWave = 0;//ウェーブ
int g_nGameWaveDisplay = 0; // WAVE表示済みのWave
int g_nGameStep = 0;//ステップ
int g_nGameTimer = 0;//タイマーwave用
int g_nGameEndTimer = 0;//タイマー終了画面用
int g_nGamePhase = 0;//ウェーブ内の展開
int g_nGamePhaseMax = 0;//そのウェーブの展開数
int nWave = 1;//ウェーブ
bool g_bGameEndSound = false;
bool g_nPushGame = false;
//=============================================================================
//ゲーム画面の初期化処理
//=============================================================================
void InitGame(void)
{
	//BGの初期化処理
	InitBG();
	//プレイヤーの初期化処理
	InitPlayer();
	//弾の初期化処理
	InitBullet();
	//銃の初期化処理
	InitGun();
	//エフェクトの初期化処理
	InitEffect();
	//爆発の初期化処理
	InitExplosion();
	//レーザーの初期化処理
	InitLaser();
	//敵の初期化処理
	InitEnemy();
	//ボスの初期化処理
	InitBoss();
	//スコアの初期化処理
	InitScore();
	//UIの初期化処理
	InitUI();

	g_bPause = false;
	g_bGameEnd = false;
	g_bGameClear = false;
	g_nGameEndSelect = 0;
	g_nGameStep = 0;
	g_nGameTimer = WAVE_WAIT_TIME;
	g_nGameEndTimer = GAMEEND_WAIT_TIME;
	g_nGamePhase = 1;
	g_nGamePhaseMax = 3;
	nWave = 1;
	g_nGameWaveDisplay = 0;
	g_bGameEndSound = false;
	g_nPushGame = false;

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM001);
}
//=============================================================================
//ゲーム画面の終了処理
//=============================================================================
void UninitGame(void)
{
	//サウンドの停止
	StopSound();

	//BGの終了処理
	UninitBG();
	//プレイヤーの終了処理
	UninitPlayer();
	//弾の終了処理
	UninitBullet();
	//銃の終了処理
	UninitGun();
	//エフェクトの終了処理
	UninitEffect();
	//爆発の終了処理
	UninitExplosion();
	//レーザーの終了処理
	UninitLaser();
	//敵の終了処理
	UninitEnemy();
	//ボスの終了処理
	UninitBoss();
	//スコアの終了処理
	UninitScore();
	//UIの終了処理
	UninitUI();
}
//=============================================================================
//ゲーム画面の更新処理
//=============================================================================
void UpdateGame(void)
{

	//ポーズ処理
	if (g_bPause == false)
	{

		//BGの更新処理
		UpdateBG();
		//プレイヤーの更新処理
		UpdatePlayer();
		//弾の更新処理
		UpdateBullet();
		//銃の更新処理
		UpdateGun();
		//エフェクトの更新処理
		UpdateEffect();
		//爆発の更新処理
		UpdateExplosion();
		//レーザーの更新処理
		UpdateLaser();
		//敵の更新処理
		UpdateEnemy();
		//ボスの更新処理
		UpdateBoss();
		//スコアの更新処理
		UpdateScore();
		//UIの更新処理
		UpdateUI();

	}
	if (GetKeyboardTrigger(DIK_P) || GetKeyboardTrigger(DIK_ESCAPE) || GetJoypadTrigger(JOYKEY_START))
	{   //Pキーを押した
		g_bPause = !g_bPause;
		PlaySound(SOUND_LABEL_SE_DECISION000);
	}

	// ゲーム展開
	if (GetPlayer()->bUse == false)
	{//プレイヤー死亡

		g_bGameClear = false;//ゲームクリア画面出さない

		// ゲーム終了タイマー開始
		if (g_nGameEndTimer == 0)
		{
			g_nGameEndTimer = GAMEEND_WAIT_TIME;

			if (g_bGameEndSound == false)
			{
				PlaySound(SOUND_LABEL_SE_GAMEOVER);
				g_bGameEndSound = true;
			}

		}
		// ゲーム終了までの待ち時間
		if (g_nGameEndTimer > 0)
		{
			g_nGameEndTimer--;

			if (g_nGameEndTimer <= 0)
			{
				g_bGameEnd = true;
				
			}
		}
	}
	else
	{
		switch (g_nGameStep)
		{
		case 0:
			// ウェーブ開始前の待ち時間
			g_nGameTimer--;

			if (g_nGameTimer <= 0)
			{
				// WaveごとのPhase数
				switch (nWave)
				{
				case 1:
					g_nGamePhaseMax = 3;
					break;

				case 2:
					g_nGamePhaseMax = 3;
					break;

				case 3:
					g_nGamePhaseMax = 3;
					break;

				case 4:
					g_nGamePhaseMax = 1;
					break;
				}

				//Phaseを変えた時表示を出さない
				if (g_nGameWaveDisplay != nWave)
				{
					g_nGameWaveDisplay = nWave;
					PlaySound(SOUND_LABEL_SE_Wave000);
					//WAVE表示へ
					g_nGameStep = 1;
					//表示時間
					g_nGameTimer = WAVE_WAIT_TIME;
				}
				else
				{
					//Phaseを変えた時表示を出さない
					g_nGameStep = 2;
					// 少し待つ
					g_nGameTimer = 60;
				}
			}

			break;
		case 1:
			// WAVE表示中
			g_nGameTimer--;

			if (g_nGameTimer <= 0)
			{
				// 敵出現待ちへ
				g_nGameStep = 2;
			}
			break;


		case 2:
			//敵出現までの待ち時間
			g_nGameTimer--;

			if (g_nGameTimer <= 0)
			{
				//敵を出現させる
				SetGameWave(nWave, g_nGamePhase);
				//戦闘開始
				g_nGameStep = 3;
			}
			break;


		case 3:
			//戦闘中
			if (GetNumEnemy() <= 0 && GetBossUse() == false)
			{
				//wave4
				if (nWave == 4)
				{
					// ゲームクリア
					g_bGameClear = true;
					

					if (g_nGameEndTimer == 0)
					{
						g_nGameEndTimer = GAMECLEAR_WAIT_TIME;
						if (g_bGameEndSound == false)
						{
							PlaySound(SOUND_LABEL_SE_GAMECLEAR);
							g_bGameEndSound = true;
						}
					}

					if (g_nGameEndTimer > 0)
					{
						g_nGameEndTimer--;

						if (g_nGameEndTimer <= 0)
						{
							g_bGameEnd = true;
							
						}
					}
				}
				else
				{
					//次のPhaseがあるか
					if (g_nGamePhase < g_nGamePhaseMax)
					{
						//次のPhaseへ
						g_nGamePhase++;

						//Phase開始待ち
						g_nGameStep = 0;
						g_nGameTimer = WAVE_WAIT_TIME;
					}
					else
					{
						//Phaseを全部クリアした
						if (nWave < 4)
						{
							//次のWaveへ
							nWave++;

							//Phaseを最初に戻す
							g_nGamePhase = 1;

							//次のWaveまで待つ
							g_nGameStep = 0;
							g_nGameTimer = WAVE_WAIT_TIME;
						}
					}
				}
			}
			break;
		}
	}

	//ゲーム終了画面
	if (g_bGameEnd)
	{

		// Wキーで上
		if (GetKeyboardRepeat(DIK_W) || GetKeyboardRepeat(DIK_UP) || GetJoypadRepeat(JOYKEY_UP) || GetJoypadLeftStickUpRepeat())
		{
			g_nGameEndSelect--;

			if (g_nGameEndSelect < 0)
			{
				g_nGameEndSelect = 2;
			}
			PlaySound(SOUND_LABEL_SE_CURSOR000);
		}

		// Sキーで下
		if (GetKeyboardRepeat(DIK_S) || GetKeyboardRepeat(DIK_DOWN) || GetJoypadRepeat(JOYKEY_DOWN) || GetJoypadLeftStickDownRepeat())
		{
			g_nGameEndSelect++;

			if (g_nGameEndSelect > 2)
			{
				g_nGameEndSelect = 0;
			}
			PlaySound(SOUND_LABEL_SE_CURSOR000);
		}

		if (g_nPushGame == false)
		{
			//決定
			if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A))
			{
				switch (g_nGameEndSelect)
				{
				case 0:
					SetResultScore(GetScore());
					//リザルト
					SetFade(MODE_RESULT);
					g_nPushGame = true;
					break;

				case 1:
					//リトライ
					SetFade(MODE_GAME);
					g_nPushGame = true;
					break;

				case 2:
					//タイトルに戻る
					SetFade(MODE_TITLE);
					g_nPushGame = true;
					break;
				}
				PlaySound(SOUND_LABEL_SE_DECISION000);
			}
		}
		
	}

	//ポーズ画面
	if (g_bPause == true)
	{
		// Wキーで上
		if (GetKeyboardRepeat(DIK_W) || GetKeyboardRepeat(DIK_UP) || GetJoypadRepeat(JOYKEY_UP) || GetJoypadLeftStickUpRepeat())
		{
			g_nGameEndSelect--;

			if (g_nGameEndSelect < 0)
			{
				g_nGameEndSelect = 2;
			}
			PlaySound(SOUND_LABEL_SE_CURSOR000);
		}

		// Sキーで下
		if (GetKeyboardRepeat(DIK_S) || GetKeyboardRepeat(DIK_DOWN) || GetJoypadRepeat(JOYKEY_DOWN) || GetJoypadLeftStickDownRepeat())
		{
			g_nGameEndSelect++;

			if (g_nGameEndSelect > 2)
			{
				g_nGameEndSelect = 0;
			}
			PlaySound(SOUND_LABEL_SE_CURSOR000);
		}

		if (g_nPushGame == false)
		{
			//決定
			if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A))
			{
				switch (g_nGameEndSelect)
				{
				case 0:
					SetResultScore(GetScore());
					//リザルト
					SetFade(MODE_RESULT);
					g_nPushGame = true;
					break;

				case 1:
					//リトライ
					SetFade(MODE_GAME);
					g_nPushGame = true;
					break;

				case 2:
					//タイトルに戻る
					SetFade(MODE_TITLE);
					g_nPushGame = true;
					break;
				}
				PlaySound(SOUND_LABEL_SE_DECISION000);
			}
		}
		
	}
}

//=============================================================================
//ゲーム画面の描画処理
//=============================================================================
void DrawGame(void)
{
	//BGの描画処理
	DrawBG();
	//プレイヤーの描画処理
	DrawPlayer();
	//弾の描画処理
	DrawBullet();
	//銃の描画処理
	DrawGun();
	//エフェクトの描画処理
	DrawEffect();
	//爆発の描画処理
	DrawExplosion();
	//レーザーの描画処理
	DrawLaser();
	//敵の描画処理
	DrawEnemy();
	//ボスの描画処理
	DrawBoss();
	//スコアの描画処理
	SetScorePosition(960.0f, 50.0f);//場所
	DrawScore();
	//UIの描画処理
	DrawUI();
	//ゲームUIの描画処理
	DrawGameUI();

	//WAVE表示
	if (g_nGameStep == 1)
	{
		DrawGameWave(nWave);
	}

	if (g_bGameEnd)
	{
		DrawGameEnd(g_bGameClear);
		StopSound(SOUND_LABEL_BGM001);
		StopSound(SOUND_LABEL_BOSS000);
	}

	if (g_bPause == true)
	{
		DrawGamePause();
	}

}
//=============================================================================
//ゲームのウェーブ処理
//=============================================================================
void SetGameWave(int nWave, int nPhase)
{
	switch (nWave)
	{
	case 1:
		switch (nPhase)
		{
		case 1:
			// Wave1 Phase1
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			break;

		case 2:
			// Wave1 Phase2
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			break;

		case 3:
			// Wave1 Phase3
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 2);
			break;
		}

		break;


	case 2:
		switch (nPhase)
		{
		case 1:
			// Wave2 Phase1
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 2);
			break;

		case 2:
			// Wave2 Phase2
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 2);
			
			break;

		case 3:
			// Wave2 Phase3
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 2);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 2);
			break;
		}
		break;


	case 3:
		switch (nPhase)
		{
		case 1:
			// Wave3 Phase1
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 20, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 20, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 20, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 20, 1);
			break;

		case 2:
			// Wave3 Phase2
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 2);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 2);
			
			
			break;

		case 3:
			// Wave3 Phase3
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 0);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 1);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 2);
			SetEnemy(D3DXVECTOR3(GetRandomEnemyX(), -50.0f, 0.0f), 10, 2);
			break;
		}
		break;


	case 4:
		//Wave 4
		SetBoss(D3DXVECTOR3(638.0f, -150.0f, 0.0f), 700);
		
		PlaySound(SOUND_LABEL_BOSS000);
		break;
	}
}

