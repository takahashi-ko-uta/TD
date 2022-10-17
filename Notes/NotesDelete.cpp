#include "NotesDelete.h"
#include "AxisIndicator.h"
#include "MathUtility.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "nameSpace/affinTransformation.h"
#include <cassert>
#include <random>
#define PI 3.141592

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
	worldTransforms_.translation_ = Vector3(-4.0f, -10.0f, 0.0f);
	//worldTransforms_.translation_ = Vector3(0.0f, 50.0f, 0.0f);
	worldTransforms_.scale_ = Vector3(2.0f, 1.0f, 1.0f);

	affinTransformation::Transfer(worldTransforms_);
	//�s��X�V
	worldTransforms_.TransferMatrix();
}

void NotesDelete::Update() {

	//Rotate();	//�J�����ɍ��킹�ĉ�]
	//Trans();	//�J�����ɍ��킹�Ĉړ�
	
	//�f�o�b�N�p�\��
	debugText_->SetPos(50, 170);
	debugText_->Printf("rotation_.x:%f", worldTransforms_.rotation_.x);

	debugText_->SetPos(50, 190);
	debugText_->Printf("worldTransforms_.translation_=(%f, %f, %f)",
		worldTransforms_.translation_.x, worldTransforms_.translation_.y, worldTransforms_.translation_.z);
}


void NotesDelete::Rotate() {
	
	worldTransforms_.rotation_.x = rotateY_ * PI /180;

	

	affinTransformation::Transfer(worldTransforms_);
	//�s��X�V
	worldTransforms_.TransferMatrix();


}

void NotesDelete ::SetRotateY(float rotateY)
{
	rotateY_ = rotateY;
}

void NotesDelete::Trans()
{
	const float move = 50.0f / 90.0f;
	worldTransforms_.translation_.y = move * rotateY_ - 10.0f;
	worldTransforms_.translation_.z = move * -rotateY_;

	//if(upFlag_ == 1)
	//{
	//	//�ŏI�I��y = 50, z = -50
	//	worldTransforms_.translation_.y += move * rotateY_;
	//	worldTransforms_.translation_.z -= move * rotateY_;
	//}
	//else if(downFlag_ == 1)
	//{
	//	//�����O��y = 50, z = -50
	//	//�ŏI�I��y = 0, z = 0
	//	worldTransforms_.translation_.y -= move * rotateY_;
	//	worldTransforms_.translation_.z += move * rotateY_;
	//}

	affinTransformation::Transfer(worldTransforms_);
	//�s��X�V
	worldTransforms_.TransferMatrix();
}

void NotesDelete::SetUpDownFlag(bool upFlag, bool downFlag)
{
	upFlag_ = upFlag;
	downFlag_ = downFlag;
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