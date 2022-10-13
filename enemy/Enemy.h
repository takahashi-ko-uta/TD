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
	void Initalize(Model* model);

	void Update();

	void SetEnemyNumber(uint16_t enemyNumber);//GameSceneÇ©ÇÁenemyNumberÇéùÇ¡ÇƒÇ≠ÇÈ

	void Draw(ViewProjection& viewProjection);
	
private:
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	WorldTransform worldTransforms_;
	uint32_t textureHandle_ = 0;

	uint32_t textureHandle_Num1_ = 0;
	uint32_t textureHandle_Num2_ = 0;
	uint32_t textureHandle_Num3_ = 0;

	uint16_t enemyNumber_ = 1;//âºÇ…3Ç‹Ç≈
};