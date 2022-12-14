#pragma once
#include "Audio.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Notes.h"
#include "InNotes.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "nameSpace/Vector3Math.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <list>
#include <math.h>
#include <memory>

//行動フェーズ
enum class Phase {
	Approach, //接近する
	Leave,    //離脱する
};

//自機のクラスの前方宣言
class Player;

class NotesHit;

class NotesStart {
  public:
	void Initalize(Model* model, uint32_t textureHandle);
	void ApproachInitalize();

	void Update();
	void Move();

	void SetPlayer(Player* player) { player_ = player; }
	void SetNotesHit(NotesHit* notesHit) { notesHit_ = notesHit; }
	void Attack();
	//ワールド座標を取得
	Vector3 GetWorldPosition();
	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();
	//弾リストを取得
	const std::list<std::unique_ptr<Notes>>& GetNotes() { return notes_; }
	const std::list<std::unique_ptr<InNotes>>& GetInNotes() { return inNotes_; }
	void Draw(ViewProjection& viewProjection);
	bool IsDead() const { return isDead_; }
	//発射間隔
	static const int kFireInterval = 30;

	

	float radius = 3.0f;

  private:
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	WorldTransform worldTransforms_;
	uint32_t textureHandle_ = 0;
	//フェーズ
	Phase phase_ = Phase::Approach;
	//弾
	std::list<std::unique_ptr<Notes>> notes_;
	std::list<std::unique_ptr<InNotes>> inNotes_;

	//発射タイマー
	int32_t FireTimer = 0;
	//自キャラ
	Player* player_ = nullptr;
	NotesHit* notesHit_ = nullptr;

	//デスフラグ
	bool isDead_ = false;
};
