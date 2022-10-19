#include "NotesDelete.h"
#include "AxisIndicator.h"
#include "MathUtility.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "nameSpace/affinTransformation.h"
#include <cassert>
#include <random>

void NotesDelete::Initalize(Model* model, uint32_t textureHandle) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎���ăf�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;
	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransforms_.Initialize();
	worldTransforms_.translation_ = Vector3(-4.0f, -10.0f, 50.0f);
	//worldTransforms_.translation_ = Vector3(0.0f, 50.0f, 0.0f);
	//worldTransforms_.translation_ = Vector3(0.0f, 0.0f, 50.0f);
	worldTransforms_.scale_ = Vector3(2.0f, 1.0f, 1.0f);

	affinTransformation::Transfer(worldTransforms_);
	//�s��X�V
	worldTransforms_.TransferMatrix();
}

void NotesDelete::Update() {

	Rotate();	//�J�����ɍ��킹�ĉ�]
	Trans();	//�J�����ɍ��킹�Ĉړ�
	
	//�f�o�b�N�p�\��
	debugText_->SetPos(50, 170);
	debugText_->Printf("rotation_.x:%f", worldTransforms_.rotation_.x);

	debugText_->SetPos(50, 190);
	debugText_->Printf("worldTransforms_.translation_=(%f, %f, %f)",
		worldTransforms_.translation_.x, worldTransforms_.translation_.y, worldTransforms_.translation_.z);
}

void NotesDelete::Rotate()
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
	//�s��X�V
	worldTransforms_.TransferMatrix();
}

void NotesDelete::Trans()
{
	float x = 50.0f;

	if(stopFlag_ == true)
	{
		worldTransforms_.translation_ = Vector3(-4.0f, 50.0f, 10.0f);
	}
	else if (stopFlag_ == false)
	{
		worldTransforms_.translation_ = Vector3(-4.0f, -10.0f, 50.0f);
	}

	affinTransformation::Transfer(worldTransforms_);
	//�s��X�V
	worldTransforms_.TransferMatrix();

	debugText_->SetPos(50, 220);
	debugText_->Printf("stopFlag:%d", stopFlag_);

}

void NotesDelete::SetStopFlag(bool stopFlag)
{
	stopFlag_ = stopFlag;
}

Vector3 NotesDelete::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransforms_.translation_.x;
	worldPos.y = worldTransforms_.translation_.y;
	worldPos.z = worldTransforms_.translation_.z;

	return worldPos;
}

void NotesDelete::OnCollision() {
	//�������Ȃ�
}

void NotesDelete::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransforms_, viewProjection, textureHandle_);

}