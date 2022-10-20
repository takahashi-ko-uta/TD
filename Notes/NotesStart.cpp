#include "Notes/NotesStart.h"
#include "AxisIndicator.h"
#include "MathUtility.h"
#include "Notes/NotesHit.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "nameSpace/affinTransformation.h"
#include "player/Player.h"
#include <cassert>
#include <random>


void NotesStart::Initalize(Model* model, uint32_t textureHandle) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎���ăf�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;
	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//�ڋ߃t�F�[�Y�̏�����
	ApproachInitalize();

	//���[���h�ϊ��̏�����
	worldTransforms_.Initialize();
	worldTransforms_.translation_ = Vector3(20.0f, -10.0f, 50.0f);
}

void NotesStart::ApproachInitalize() {
	//���˃^�C�}�[������
	FireTimer = kFireInterval;
}

void NotesStart::Update() {
	Trans();
	Rotate();

	Move();

	for (std::unique_ptr<Notes>& notes : notes_) {
		notes->Update();
	}

	for (std::unique_ptr<InNotes>& inNotes : inNotes_) {
		inNotes->Update();
	}

	//�f�X�t���O���������e��r��
	notes_.remove_if([](std::unique_ptr<Notes>& notes) { return notes->IsDead(); });
	inNotes_.remove_if([](std::unique_ptr<InNotes>& inNotes) { return inNotes->IsDead(); });
}

void NotesStart::Move() {
	Vector3 move = {0, 0, 0};
	switch (phase_) {
	case Phase::Approach:
	default:
		//���˃^�C�}�[�J�E���g�_�E��
		FireTimer -= 1;
		//�w�莞�ԂɒB����
		if (FireTimer == 0) {
			//�e�𔭎�
			Attack();
			//�^�C�}�[��߂�
			FireTimer = kFireInterval;
		}

		//�ړ�
		move = {0.0f, 0.0f, -0.0f};
		//����̈ʒu�ɓ��B�����痣�E
		if (worldTransforms_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		move = {-0.2f, 0.0f, -0.2f};
		break;
	}
	//���W�ړ�(�x�N�g�����Z)
	worldTransforms_.translation_ += move;
	affinTransformation::Transfer(worldTransforms_);
	//�s��X�V
	worldTransforms_.TransferMatrix();
}

void NotesStart::Attack() {
	assert(notesHit_);
	//assert(player_);

	//�e�̑��x
	const float kBulletSpeed =
	  0.3 * 0.1f; //�o�O�ő�������0.1�������Ă���(�v�C��)--------------------------------

	// Vector3 velocity(0, 0, kBulletSpeed);
	//Vector3 PLvec = player_->GetWorldPosition(); //���L�����̃��[���h���W���擾
	Vector3 PLvec = notesHit_->GetWorldPosition();     //���L�����̃��[���h���W���擾
	Vector3 ENvec = GetWorldPosition();            //�G�L�����̃��[���h���W���擾
	Vector3 SPvec = Vector3Math::diff(PLvec, ENvec); //�G�L���������L�����̍����x�N�g�������߂�
	SPvec = Vector3Math::Normalize(SPvec); //�x�N�g���̐��K��
	Vector3 velocity(
	  SPvec.x * kBulletSpeed, SPvec.y * kBulletSpeed,
	  SPvec.z * kBulletSpeed); //�x�N�g���̒������A�����ɍ��킹��

	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	velocity.x = (velocity.x * worldTransforms_.matWorld_.m[0][0]) +
	             (velocity.y * worldTransforms_.matWorld_.m[1][0]) +
	             (velocity.z * worldTransforms_.matWorld_.m[2][0]) +
	             (0 * worldTransforms_.matWorld_.m[3][0]);

	velocity.y = (velocity.x * worldTransforms_.matWorld_.m[0][1]) +
	             (velocity.y * worldTransforms_.matWorld_.m[1][1]) +
	             (velocity.z * worldTransforms_.matWorld_.m[2][1]) +
	             (0 * worldTransforms_.matWorld_.m[3][1]);

	velocity.z = (velocity.x * worldTransforms_.matWorld_.m[0][2]) +
	             (velocity.y * worldTransforms_.matWorld_.m[1][2]) +
	             (velocity.z * worldTransforms_.matWorld_.m[2][2]) +
	             (0 * worldTransforms_.matWorld_.m[3][2]);



	//�e�𐶐����A������
	std::unique_ptr<Notes> notes = std::make_unique<Notes>();
	notes->Initialize(model_, worldTransforms_.translation_, velocity);

	std::unique_ptr<InNotes> inNotes = std::make_unique<InNotes>();
	inNotes->Initialize(model_, worldTransforms_.translation_, velocity);

	//�e��o�^����
	notes_.push_back(std::move(notes));
	inNotes_.push_back(std::move(inNotes));
}

void NotesStart::Rotate()
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

void NotesStart::Trans()
{
	float x = 50.0f;

	if (stopFlag_ == true)
	{
		worldTransforms_.translation_ = Vector3(20.0f, 50.0f, 10.0f);
	}
	else if (stopFlag_ == false)
	{
		worldTransforms_.translation_ = Vector3(20.0f, -10.0f, 50.0f);
	}

	affinTransformation::Transfer(worldTransforms_);
	//�s��X�V
	worldTransforms_.TransferMatrix();

	/*debugText_->SetPos(50, 220);
	debugText_->Printf("stopFlag:%d", stopFlag_);

	debugText_->SetPos(50, 250);
	debugText_->Printf("%f, %f ,%f",
		worldTransforms_.translation_.x, worldTransforms_.translation_.y, worldTransforms_.translation_.z);*/
}

void NotesStart::SetStopFlag(bool stopFlag)
{
	stopFlag_ = stopFlag;
}

Vector3 NotesStart::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransforms_.translation_.x;
	worldPos.y = worldTransforms_.translation_.y;
	worldPos.z = worldTransforms_.translation_.z;

	return worldPos;
}

void NotesStart::OnCollision() {
	//�������Ȃ�
}

void NotesStart::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransforms_, viewProjection, textureHandle_);
	//�e�`��
	for (std::unique_ptr<Notes>& notes : notes_) {
		notes->Draw(viewProjection);
	}
	for (std::unique_ptr<InNotes>& inNotes : inNotes_) {
		inNotes->Draw(viewProjection);
	}

}
