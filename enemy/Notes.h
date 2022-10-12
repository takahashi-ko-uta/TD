#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Notes {
  public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();
	// �Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();
	void NotCollision();

	//���������Ƃ��Ƀm�[�c������
	void deleteNotes();

	Vector3 GetWorldPosition();

	int GetTrigger() const { return isTrigger_; }
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }
	float radius = 3.0f;

  private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	uint32_t textureHandle_red = 0;
	uint32_t textureHandle_blue = 0;
	//���x
	Vector3 velocity_;
	//����
	static const int32_t kLifeTime = 60 * 5;
	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;
	bool isTrigger_ = false;
};