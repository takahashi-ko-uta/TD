﻿#pragma once

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
#include "enemy/NotesStart.h"
#include "Notes/NotesHit.h"
#include "Notes/NotesDelete.h"


#include "Skydome.h"

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

	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	Audio* audio_ = nullptr;

	int scene;
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

	//敵キャラ
	NotesStart* enemy_ = nullptr;

	//EnemyBullet* bullet_ = nullptr;
	NotesHit* notesHit_ = nullptr;

	NotesDelete* notesDelete_ = nullptr;

	//3Dモデル
	Model* modelSkydome_ = nullptr;

	Skydome* skydome_ = nullptr;


	bool BLtrigger = false;
	uint16_t judge_success = 0;
	uint16_t judge_failure = 0;
	uint16_t notesCount = 0;//流れてきたノーツを数える
	/// <summary>
	/// ゲームシーン用
	/// </summary>
	uint32_t titlePic[1] = {};
	Sprite* title[1];
	int titlenum;

	uint16_t soundkeep = 0;
};
