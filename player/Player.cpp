#include "Player.h"
#include "AxisIndicator.h"
#include "MathUtility.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "nameSpace/affinTransformation.h"
#include <cassert>
#include <random>

void Player::Initalize(Model* model, uint32_t textureHandle) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎���ăf�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;
	textureHandle_red = TextureManager::Load("red.png");
	textureHandle_blue = TextureManager::Load("blue.jpg");
	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransforms_.Initialize();
	worldTransforms_.translation_ = Vector3(0.0f, 0.0f, 0.0f);
	worldTransforms_.scale_ = Vector3(2.0f, 1.0f, 1.0f);
}

void Player::Update() {
	Move();   //�ړ�����
	Rotate(); //���񏈗�
	Attack(); //�U������
}

void Player::Move() {
#pragma region �L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = {0, 0, 0};
	{
		////�ړ��x�N�g���̕ύX���鏈��
		// if (input_->PushKey(DIK_UP)) {
		//	move.y += 0.5f;
		// } else if (input_->PushKey(DIK_DOWN)) {
		//	move.y -= 0.5f;
		// } else if (input_->PushKey(DIK_LEFT)) {
		//	move.x -= 0.5f;
		// } else if (input_->PushKey(DIK_RIGHT)) {
		//	move.x += 0.5f;
		// }
	}
	//���W�ړ�(�x�N�g�����Z)
	worldTransforms_.translation_ += move;
	affinTransformation::Transfer(worldTransforms_);

#pragma region �ړ�����
	//�ړ����E���W
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 20.0f;

	//�͈͂𒴂��Ȃ�����
	worldTransforms_.translation_.x = max(worldTransforms_.translation_.x, -kMoveLimitX);
	worldTransforms_.translation_.x = min(worldTransforms_.translation_.x, +kMoveLimitX);
	worldTransforms_.translation_.y = max(worldTransforms_.translation_.y, -kMoveLimitY);
	worldTransforms_.translation_.y = min(worldTransforms_.translation_.y, +kMoveLimitY);

#pragma endregion

	//�s��X�V
	worldTransforms_.TransferMatrix();
#pragma endregion
}

void Player::Rotate() {
	Vector3 RotY = {0.0f, 0.0f, 0.0f};
	if (input_->PushKey(DIK_U)) {
		RotY.y += 0.01f;
	} else if (input_->PushKey(DIK_I)) {
		RotY.y -= 0.01f;
	}

	worldTransforms_.rotation_ += RotY;
	affinTransformation::Transfer(worldTransforms_);
	//�s��X�V
	worldTransforms_.TransferMatrix();
}

void Player::Attack() {

	 if (input_->TriggerKey(DIK_SPACE)) 
	 {
		isTrigger_ = true;
	 }
	 else
	 {
		 isTrigger_ = false;
	 }
}

Vector3 Player::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransforms_.translation_.x;
	worldPos.y = worldTransforms_.translation_.y;
	worldPos.z = worldTransforms_.translation_.z;

	return worldPos;
}

void Player::OnCollision() {
	//�������Ȃ�
}

void Player::Draw(ViewProjection& viewProjection) {

	//model_->Draw(worldTransforms_, viewProjection, textureHandle_);
	if (isTrigger_ == true) //�������Ƃ�
	{
		model_->Draw(worldTransforms_, viewProjection, textureHandle_red);
	}
	else //�����Ă��Ȃ���
	{
		model_->Draw(worldTransforms_, viewProjection, textureHandle_blue);
	}
}