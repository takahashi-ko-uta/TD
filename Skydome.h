#pragma once

#include "Model.h"
#include "WorldTransform.h"

class Skydome
{
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//テクスチャハンドル
	//uint32_t textureHandle_ = 0u;

	//モデル
	Model* model_ = nullptr;
	const float size = 5.0f;
	Vector3 scale = { size,size,size };
	Vector3 trans = { 0.0f, 0.0f, 0.0f };
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection viewProjection);
};

