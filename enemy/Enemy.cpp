#include "Enemy.h"
#include "AxisIndicator.h"
#include "MathUtility.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "nameSpace/affinTransformation.h"
#include <cassert>
#include <random>


void Enemy::Initalize(Model* model) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎���ăf�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = TextureManager::Load("black.jpg");

	textureHandle_Num1_ = TextureManager::Load("num1.png");
	textureHandle_Num2_ = TextureManager::Load("num2.png");
	textureHandle_Num3_ = TextureManager::Load("num3.png");
	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransforms_.Initialize();
	worldTransforms_.translation_ = Vector3(0.0f, 0.0f, -5.0f);
	worldTransforms_.scale_ = Vector3(5.0f, 5.0f, 5.0f);
	affinTransformation::Transfer(worldTransforms_);
	worldTransforms_.TransferMatrix();
}

void Enemy::Update() {
	
}

void Enemy::SetEnemyNumber(uint16_t enemyNumber)
{
	enemyNumber_ = enemyNumber;
}

void Enemy::Draw(ViewProjection& viewProjection) {
	
	switch (enemyNumber_)
	{
	case 1:
		model_->Draw(worldTransforms_, viewProjection, textureHandle_Num1_);
		break;
	case 2:
		model_->Draw(worldTransforms_, viewProjection, textureHandle_Num2_);
		break;
	case 3:
		model_->Draw(worldTransforms_, viewProjection, textureHandle_Num3_);
		break;
	}

}