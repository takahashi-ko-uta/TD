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
	Trans();
}

void NotesEnd::Trans()
{
	float x = 50.0f;

	if (stopFlag_ == true)
	{
		worldTransforms_.translation_ = Vector3(-20.0f, 50.0f, 10.0f);
	}
	else if (stopFlag_ == false)
	{
		worldTransforms_.translation_ = Vector3(-20.0f, -10.0f, 50.0f);
	}

	affinTransformation::Transfer(worldTransforms_);
	//行列更新
	worldTransforms_.TransferMatrix();

	debugText_->SetPos(50, 220);
	debugText_->Printf("stopFlag:%d", stopFlag_);

	/*debugText_->SetPos(50, 250);
	debugText_->Printf("%f, %f ,%f",
		worldTransforms_.translation_.x , worldTransforms_.translation_.y, worldTransforms_.translation_.z);*/
}

void NotesEnd::SetStopFlag(bool stopFlag)
{
	stopFlag_ = stopFlag;
}

void NotesEnd::Rotate()
{
	float PI = 3.141592;
	
	//worldTransforms_.rotation_.x = -90 * PI / 180;

	if (stopFlag_ == true)
	{
		worldTransforms_.rotation_.x = -90 * PI / 180;
	}
	else if (stopFlag_ == false)
	{
		worldTransforms_.rotation_.x = 0;
	}

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