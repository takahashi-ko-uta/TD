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
	worldTransforms_.translation_ = Vector3(-4.0f, -10.0f, 0.0f);
	worldTransforms_.scale_ = Vector3(2.0f, 1.0f, 1.0f);
}

void NotesDelete::Update() {

	Rotate(); //���񏈗�
}


void NotesDelete::Rotate() {
	Vector3 RotY = { 0.0f, 0.0f, 0.0f };
	if (input_->PushKey(DIK_U)) {
		RotY.y += 0.01f;
	}
	else if (input_->PushKey(DIK_I)) {
		RotY.y -= 0.01f;
	}

	worldTransforms_.rotation_ += RotY;
	affinTransformation::Transfer(worldTransforms_);
	//�s��X�V
	worldTransforms_.TransferMatrix();
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