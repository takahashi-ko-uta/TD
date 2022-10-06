#include "PlayerBullet.h"
#include "AxisIndicator.h"
#include "MathUtility.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "affinTransformation.h"
#include <cassert>
#include <random>

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎���ăf�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = TextureManager::Load("black.jpg");

	worldTransform_.Initialize();
	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = {position.x,position.y,position.z};

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ������
	velocity_ = velocity;
}


void PlayerBullet::Update() {
	worldTransform_.translation_ += velocity_;
	affinTransformation::Transfer(worldTransform_);
	worldTransform_.TransferMatrix();
	//���Ԍo�߂ŏ�����
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void PlayerBullet::OnCollision() { IsDead(); }

Vector3 PlayerBullet::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}