#include "Notes/NotesStart.h"
#include "AxisIndicator.h"
#include "MathUtility.h"
#include "Notes/NotesHit.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "nameSpace/affinTransformation.h"
#include "player/Player.h"
#include <cassert>
#include <random>


void NotesStart::Initalize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);

	//引数として受け取ってデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//接近フェーズの初期化
	ApproachInitalize();

	//ワールド変換の初期化
	worldTransforms_.Initialize();
	worldTransforms_.translation_ = Vector3(20.0f, -10.0f, 50.0f);
}

void NotesStart::ApproachInitalize() {
	//発射タイマー初期化
	FireTimer = kFireInterval;
}

void NotesStart::Update() {
	Trans();
	Rotate();

	Move();

	for (std::unique_ptr<Notes>& notes : notes_) {
		notes->Update();
	}

	for (std::unique_ptr<InNotes>& inNotes : inNotes_) {
		inNotes->Update();
	}

	//デスフラグが立った弾を排除
	notes_.remove_if([](std::unique_ptr<Notes>& notes) { return notes->IsDead(); });
	inNotes_.remove_if([](std::unique_ptr<InNotes>& inNotes) { return inNotes->IsDead(); });
}

void NotesStart::Move() {
	Vector3 move = {0, 0, 0};
	switch (phase_) {
	case Phase::Approach:
	default:
		//発射タイマーカウントダウン
		FireTimer -= 1;
		//指定時間に達した
		if (FireTimer == 0) {
			//弾を発射
			Attack();
			//タイマーを戻す
			FireTimer = kFireInterval;
		}

		//移動
		move = {0.0f, 0.0f, -0.0f};
		//既定の位置に到達したら離脱
		if (worldTransforms_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		move = {-0.2f, 0.0f, -0.2f};
		break;
	}
	//座標移動(ベクトル加算)
	worldTransforms_.translation_ += move;
	affinTransformation::Transfer(worldTransforms_);
	//行列更新
	worldTransforms_.TransferMatrix();
}

void NotesStart::Attack() {
	assert(notesHit_);
	//assert(player_);

	//弾の速度
	const float kBulletSpeed =
	  0.3 * 0.1f; //バグで速いため0.1をかけている(要修正)--------------------------------

	// Vector3 velocity(0, 0, kBulletSpeed);
	//Vector3 PLvec = player_->GetWorldPosition(); //自キャラのワールド座標を取得
	Vector3 PLvec = notesHit_->GetWorldPosition();     //自キャラのワールド座標を取得
	Vector3 ENvec = GetWorldPosition();            //敵キャラのワールド座標を取得
	Vector3 SPvec = Vector3Math::diff(PLvec, ENvec); //敵キャラ→自キャラの差分ベクトルを求める
	SPvec = Vector3Math::Normalize(SPvec); //ベクトルの正規化
	Vector3 velocity(
	  SPvec.x * kBulletSpeed, SPvec.y * kBulletSpeed,
	  SPvec.z * kBulletSpeed); //ベクトルの長さを、速さに合わせる

	//速度ベクトルを自機の向きに合わせて回転させる
	velocity.x = (velocity.x * worldTransforms_.matWorld_.m[0][0]) +
	             (velocity.y * worldTransforms_.matWorld_.m[1][0]) +
	             (velocity.z * worldTransforms_.matWorld_.m[2][0]) +
	             (0 * worldTransforms_.matWorld_.m[3][0]);

	velocity.y = (velocity.x * worldTransforms_.matWorld_.m[0][1]) +
	             (velocity.y * worldTransforms_.matWorld_.m[1][1]) +
	             (velocity.z * worldTransforms_.matWorld_.m[2][1]) +
	             (0 * worldTransforms_.matWorld_.m[3][1]);

	velocity.z = (velocity.x * worldTransforms_.matWorld_.m[0][2]) +
	             (velocity.y * worldTransforms_.matWorld_.m[1][2]) +
	             (velocity.z * worldTransforms_.matWorld_.m[2][2]) +
	             (0 * worldTransforms_.matWorld_.m[3][2]);



	//弾を生成し、初期化
	std::unique_ptr<Notes> notes = std::make_unique<Notes>();
	notes->Initialize(model_, worldTransforms_.translation_, velocity);

	std::unique_ptr<InNotes> inNotes = std::make_unique<InNotes>();
	inNotes->Initialize(model_, worldTransforms_.translation_, velocity);

	//弾を登録する
	notes_.push_back(std::move(notes));
	inNotes_.push_back(std::move(inNotes));
}

void NotesStart::Rotate()
{
	float PI = 3.141592;

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

void NotesStart::Trans()
{
	float x = 50.0f;

	if (stopFlag_ == true)
	{
		worldTransforms_.translation_ = Vector3(20.0f, 50.0f, 10.0f);
	}
	else if (stopFlag_ == false)
	{
		worldTransforms_.translation_ = Vector3(20.0f, -10.0f, 50.0f);
	}

	affinTransformation::Transfer(worldTransforms_);
	//行列更新
	worldTransforms_.TransferMatrix();

	/*debugText_->SetPos(50, 220);
	debugText_->Printf("stopFlag:%d", stopFlag_);

	debugText_->SetPos(50, 250);
	debugText_->Printf("%f, %f ,%f",
		worldTransforms_.translation_.x, worldTransforms_.translation_.y, worldTransforms_.translation_.z);*/
}

void NotesStart::SetStopFlag(bool stopFlag)
{
	stopFlag_ = stopFlag;
}

Vector3 NotesStart::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransforms_.translation_.x;
	worldPos.y = worldTransforms_.translation_.y;
	worldPos.z = worldTransforms_.translation_.z;

	return worldPos;
}

void NotesStart::OnCollision() {
	//何もしない
}

void NotesStart::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransforms_, viewProjection, textureHandle_);
	//弾描画
	for (std::unique_ptr<Notes>& notes : notes_) {
		notes->Draw(viewProjection);
	}
	for (std::unique_ptr<InNotes>& inNotes : inNotes_) {
		inNotes->Draw(viewProjection);
	}

}
