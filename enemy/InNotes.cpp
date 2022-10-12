#include "InNotes.h"
#include "AxisIndicator.h"
#include "MathUtility.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "affinTransformation.h"
#include <cassert>
#include <random>

void InNotes::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	//引数として受け取ってデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = TextureManager::Load("black.jpg");
	textureHandle_red = TextureManager::Load("red.png");
	textureHandle_blue = TextureManager::Load("blue.jpg");
	textureHandle_yellow_ = TextureManager::Load("Yellow.jpg");
	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = { position.x, position.y, position.z };
	worldTransform_.scale_ = Vector3(0.9f, 1.9f, 0.9f);
	//引数で受け取った速度をメンバ変数に代入する
	velocity_ = velocity;
}

void InNotes::Update() {
	worldTransform_.translation_ += velocity_;
	affinTransformation::Transfer(worldTransform_);
	worldTransform_.TransferMatrix();
	//時間経過で消える
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void InNotes::OnCollision() {
	//isDead_ = true;
	isTrigger_ = false;	//当たっているときはfalse
}
void InNotes::NotCollision() {
	// isDead_ = true;
	isTrigger_ = true;	//当たっていないときはtrue
}

void InNotes::deleteNotes()
{
	isDead_ = true;
}

Vector3 InNotes::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void InNotes::Draw(const ViewProjection& viewProjection) {
	if (isTrigger_ == false) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_yellow_);
	}
	else {
		model_->Draw(worldTransform_, viewProjection, textureHandle_yellow_);
	}
}