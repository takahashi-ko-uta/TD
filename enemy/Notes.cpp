#include "Notes.h"
#include "AxisIndicator.h"
#include "MathUtility.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "affinTransformation.h"
#include <cassert>
#include <random>

void Notes::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎���ăf�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = TextureManager::Load("black.jpg");
	textureHandle_red = TextureManager::Load("red.png");
	textureHandle_blue = TextureManager::Load("blue.jpg");
	worldTransform_.Initialize();
	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = {position.x, position.y, position.z};
	worldTransform_.scale_ = Vector3(1.0f, 2.0f, 1.0f);
	//�����Ŏ󂯎�������x�������o�ϐ��ɑ������
	velocity_ = velocity;
}

void Notes::Update() {
	worldTransform_.translation_ += velocity_;
	affinTransformation::Transfer(worldTransform_);
	worldTransform_.TransferMatrix();
	//���Ԍo�߂ŏ�����
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void Notes::OnCollision() {
	//isDead_ = true;
	isTrigger_ = true;
}
void Notes::NotCollision() {
	//isDead_ = true;
	isTrigger_ = false;
}

void Notes::deleteNotes()
{
	isDead_ = true;
}

Vector3 Notes::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Notes::Draw(const ViewProjection& viewProjection) {
	if (isTrigger_ == false) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_red);
	} else {
		model_->Draw(worldTransform_, viewProjection, textureHandle_blue);
	}
}