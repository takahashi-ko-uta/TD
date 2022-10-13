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
#include "Vector3Math.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <list>
#include <math.h>
#include <memory>

//�s���t�F�[�Y
enum class Phase {
	Approach, //�ڋ߂���
	Leave,    //���E����
};

//���@�̃N���X�̑O���錾
class Player;

class NotesHit;

class Enemy {
  public:
	void Initalize(Model* model, uint32_t textureHandle);
	void ApproachInitalize();

	void Update();
	void Move();

	void SetPlayer(Player* player) { player_ = player; }
	void SetNotesHit(NotesHit* notesHit) { notesHit_ = notesHit; }
	void Attack();
	//���[���h���W���擾
	Vector3 GetWorldPosition();
	// �Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();
	//�e���X�g���擾
	const std::list<std::unique_ptr<Notes>>& GetNotes() { return notes_; }
	const std::list<std::unique_ptr<InNotes>>& GetInNotes() { return inNotes_; }
	void Draw(ViewProjection& viewProjection);
	bool IsDead() const { return isDead_; }
	//���ˊԊu
	static const int kFireInterval = 30;

	

	float radius = 3.0f;

  private:
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	WorldTransform worldTransforms_;
	uint32_t textureHandle_ = 0;
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
	//�e
	std::list<std::unique_ptr<Notes>> notes_;
	std::list<std::unique_ptr<InNotes>> inNotes_;

	//���˃^�C�}�[
	int32_t FireTimer = 0;
	//���L����
	Player* player_ = nullptr;
	NotesHit* notesHit_ = nullptr;

	//�f�X�t���O
	bool isDead_ = false;
};
