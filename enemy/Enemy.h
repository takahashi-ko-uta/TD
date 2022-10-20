#pragma once
#include "Audio.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <list>
#include <math.h>
#include <memory>

class Enemy {
public:
	void Initalize(Model* num1);// , Model* num2, Model* num3);

	void Update();

	void SetEnemyNumber(uint16_t enemyNumber);//GameSceneÇ©ÇÁenemyNumberÇéùÇ¡ÇƒÇ≠ÇÈ

	float GetSoundLevel();

	void Draw(ViewProjection& viewProjection);
	
private:
	Model* model_ = nullptr;
	Model* num1_ = nullptr;
	Model* num2_ = nullptr;
	Model* num3_ = nullptr;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	WorldTransform worldTransforms_;
	uint32_t textureHandle_ = 0;

	uint32_t textureHandle_Num1_ = 0;
	uint32_t textureHandle_Num2_ = 0;
	uint32_t textureHandle_Num3_ = 0;
	uint32_t textureHandle_Tyu_ = 0;

	uint16_t enemyNumber_ = 1;//âºÇ…3Ç‹Ç≈
	float soundLevel = 0.5f;
};