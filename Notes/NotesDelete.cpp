#include "NotesDelete.h"
#include "AxisIndicator.h"
#include "MathUtility.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "nameSpace/affinTransformation.h"
#include <cassert>
#include <random>
#define PI 3.141592

void NotesDelete::Initalize(Model* model, uint32_t textureHandle) {
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
	worldTransforms_.translation_ = Vector3(-4.0f, -10.0f, 0.0f);
	//worldTransforms_.translation_ = Vector3(0.0f, 50.0f, 0.0f);
	worldTransforms_.scale_ = Vector3(2.0f, 1.0f, 1.0f);

	affinTransformation::Transfer(worldTransforms_);
	//行列更新
	worldTransforms_.TransferMatrix();
}

void NotesDelete::Update() {

	//Rotate();	//カメラに合わせて回転
	//Trans();	//カメラに合わせて移動
	
	//デバック用表示
	debugText_->SetPos(50, 170);
	debugText_->Printf("rotation_.x:%f", worldTransforms_.rotation_.x);

	debugText_->SetPos(50, 190);
	debugText_->Printf("worldTransforms_.translation_=(%f, %f, %f)",
		worldTransforms_.translation_.x, worldTransforms_.translation_.y, worldTransforms_.translation_.z);
}


void NotesDelete::Rotate() {
	
	worldTransforms_.rotation_.x = rotateY_ * PI /180;

	

	affinTransformation::Transfer(worldTransforms_);
	//行列更新
	worldTransforms_.TransferMatrix();


}

void NotesDelete ::SetRotateY(float rotateY)
{
	rotateY_ = rotateY;
}

void NotesDelete::Trans()
{
	const float move = 50.0f / 90.0f;
	worldTransforms_.translation_.y = move * rotateY_ - 10.0f;
	worldTransforms_.translation_.z = move * -rotateY_;

	//if(upFlag_ == 1)
	//{
	//	//最終的にy = 50, z = -50
	//	worldTransforms_.translation_.y += move * rotateY_;
	//	worldTransforms_.translation_.z -= move * rotateY_;
	//}
	//else if(downFlag_ == 1)
	//{
	//	//処理前はy = 50, z = -50
	//	//最終的にy = 0, z = 0
	//	worldTransforms_.translation_.y -= move * rotateY_;
	//	worldTransforms_.translation_.z += move * rotateY_;
	//}

	affinTransformation::Transfer(worldTransforms_);
	//行列更新
	worldTransforms_.TransferMatrix();
}

void NotesDelete::SetUpDownFlag(bool upFlag, bool downFlag)
{
	upFlag_ = upFlag;
	downFlag_ = downFlag;
}

Vector3 NotesDelete::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransforms_.translation_.x;
	worldPos.y = worldTransforms_.translation_.y;
	worldPos.z = worldTransforms_.translation_.z;

	return worldPos;
}

void NotesDelete::OnCollision() {
	//何もしない
}

void NotesDelete::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransforms_, viewProjection, textureHandle_);

}