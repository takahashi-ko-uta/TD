#include "NotesEnd.h"
#include "AxisIndicator.h"
#include "MathUtility.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "nameSpace/affinTransformation.h"
#include <cassert>
#include <random>


void NotesEnd::Initalize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);

	//引数として受け取ってデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransforms_.Initialize();
	worldTransforms_.translation_ = Vector3(-20.0f, -10.0f, 50.0f);
	worldTransforms_.scale_ = Vector3(1.0f, 1.0f, 1.0f);
}

void NotesEnd::Update() {

	Rotate(); //旋回処理
}


void NotesEnd::Rotate() {
	
	affinTransformation::Transfer(worldTransforms_);
	//行列更新
	worldTransforms_.TransferMatrix();
}

Vector3 NotesEnd::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransforms_.translation_.x;
	worldPos.y = worldTransforms_.translation_.y;
	worldPos.z = worldTransforms_.translation_.z;

	return worldPos;
}

void NotesEnd::OnCollision() {
	//何もしない
}

void NotesEnd::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransforms_, viewProjection, textureHandle_);

}