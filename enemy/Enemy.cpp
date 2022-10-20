#include "Enemy.h"
#include "AxisIndicator.h"
#include "MathUtility.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "nameSpace/affinTransformation.h"
#include <cassert>
#include <random>


void Enemy::Initalize(Model* num1){//}, Model* num2, Model* num3) {
	// NULL�|�C���^�`�F�b�N
	assert(num1);
	/*assert(num2);
	assert(num3);*/

	//�����Ƃ��Ď󂯎���ăf�[�^�������o�ϐ��ɋL�^����
	model_ = num1;
	num1_ = num1;
	/*num2_ = num2;
	num3_ = num3;*/

	textureHandle_ = TextureManager::Load("black.jpg");

	textureHandle_Num1_ = TextureManager::Load("num1.png");
	textureHandle_Num2_ = TextureManager::Load("num2.png");
	textureHandle_Num3_ = TextureManager::Load("num3.png");
	textureHandle_Tyu_ = TextureManager::Load("tutorial.png");

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransforms_.Initialize();
	worldTransforms_.translation_ = Vector3(0.0f, 0.0f, 50.0f);
	worldTransforms_.scale_ = Vector3(5.0f, 5.0f, 5.0f);
	affinTransformation::Transfer(worldTransforms_);
	worldTransforms_.TransferMatrix();
}

void Enemy::Update() {
	
	switch (enemyNumber_)
	{
#pragma region �`���[�g���A��
	case 0:
		
		break;
#pragma endregion

#pragma region �X�e�[�W�P����������i���j
	case 1:
		if(input_->PushKey(DIK_2))
		{
			soundLevel = 0.0f;
		}
		else
		{
			soundLevel = 0.5f;
		}

		break;
#pragma endregion

#pragma region �X�e�[�W�Q
	case 2:

		break;
#pragma endregion

#pragma region �X�e�[�W�R
	case 3:

		break;
#pragma endregion
	}

}

void Enemy::SetEnemyNumber(uint16_t enemyNumber)
{
	enemyNumber_ = enemyNumber;
}

float Enemy::GetSoundLevel()
{
	return soundLevel;
}

void Enemy::Draw(ViewProjection& viewProjection) {
	
	switch (enemyNumber_)
	{
	case 0://�`���[�g���A��
		model_->Draw(worldTransforms_, viewProjection, textureHandle_Tyu_);
		//num1_->Draw(worldTransforms_, viewProjection);
		break;
	case 1://�X�e�[�W�P
		model_->Draw(worldTransforms_, viewProjection, textureHandle_Num1_);
		//num1_->Draw(worldTransforms_, viewProjection);
		break;
	case 2://�X�e�[�W�Q
		model_->Draw(worldTransforms_, viewProjection, textureHandle_Num2_);
		//num2_->Draw(worldTransforms_, viewProjection);
		break;
	case 3://�X�e�[�W�R
		model_->Draw(worldTransforms_, viewProjection, textureHandle_Num3_);
		//num3_->Draw(worldTransforms_, viewProjection);
		break;
	}

}