#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include <math.h>

#include "nameSpace/affinTransformation.h"
#include "player/Player.h"
#include "Notes/NotesStart.h"
#include "Notes/NotesHit.h"
#include "Notes/NotesDelete.h"

#include "Notes/NotesEnd.h"
#include "Skydome.h"
#include <DirectXTex.h>

#include "enemy/Enemy.h"

#define PI 3.141592

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	GameScene();

	~GameScene();

	void Initialize();
	
	void Update();
	void CheckAllCollisons();
	void TriggerJudge();//成功判定
	void SceneChenge();
	void Metronome(float tempo, uint32_t bgmHandle);

	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	Audio* audio_ = nullptr;

	
	//テクスチャハンドル
	uint32_t textureHandle_PL_ = 0;    // 自機のテクスチャ
	uint32_t textureHandle_EN_ = 0;    // 敵のテクスチャ
	uint32_t textureHandle_Black_ = 0; //黒一色のテクスチャ
	uint32_t textureHandle_Red_ = 0;
	//音声再生ハンドル
	uint32_t bgmHandle_ = 0;
	uint32_t bgmHandle2_ = 0;

	//サウンドデータハンドル
	uint32_t soundDataHandle_ = 0;
	uint32_t soundDataHandle2_ = 0;

	// 3Dモデル
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelStage_ = nullptr;
	//ワールドトランスフォームビュープロジェクション
	WorldTransform worldTransforms_[100];
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;
	//デバックカメラ有効
	//カメラ上方向の角度
	float viewAngle = 0.0f;

	//自キャラ
	Player* player_ = nullptr;

	//ノーツの出発点、終着点
	NotesStart* notesStart_ = nullptr;
	NotesEnd* notesEnd_ = nullptr;

	//ノーツの成功失敗判定
	NotesHit* notesHit_ = nullptr;

	//ノーツを消すやつ
	NotesDelete* notesDelete_ = nullptr;

	//敵キャラ
	Enemy* enemy_ = nullptr;

	//スカイドーム
	Skydome* skydome_ = nullptr;

	bool BLtrigger = false;
	uint16_t judge_success = 0;
	uint16_t judge_failure = 0;
	uint16_t notesCount = 0;//流れてきたノーツを数える
	/// <summary>
	/// ゲームシーン用
	/// </summary>
	uint32_t titlePic[1] = {};
	Sprite* title[1] = {};
	int titlenum = 0;
	uint16_t scene = 0;
	uint16_t soundkeep = 0;

	//注視点の移動ベクトル
	DirectX::XMFLOAT3 move = { 0, 0, 0 };
	//注視点の移動速度
	float kTargetSpeed = 0.5f;

	uint16_t upFlag = 0;	//視点を上げる
	uint16_t downFlag = 0;	//視点を下げる
	uint16_t stopFlag = 0;	//視点を止める

	float stopTimer = 0;
	

	uint16_t enemyNumber = 0;//仮に3まで 敵の選択 0...チュートリアル 

	float soundLevel = 0.5f;//0.0f~


	float interval = 0;
	float timer = 0;
};
