
#include "affinTransformation.h"
#include "MathUtility.h"
#include <math.h>

#define PI 3.141592

namespace affinTransformation {
void Scale(WorldTransform &worldTransform_ ) {//�X�P�[�����O

	
	//�X�P�[�����O�s���錾
	Matrix4 matScale;
	//�X�P�[�����O�{�����s���錾
	matScale = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;

	worldTransform_.matWorld_ = MathUtility ::Matrix4Identity();
	worldTransform_.matWorld_ *= matScale;

	
}
void Rotate(WorldTransform &worldTransform_) {//��]

	//��]�s���錾
	Matrix4 matRot;
	Matrix4 matRotX, matRotY, matRotZ;
	//��]�{�����s���錾
	matRot = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	          0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	matRotZ = MathUtility ::Matrix4Identity();
	matRotX = MathUtility ::Matrix4Identity();
	matRotY = MathUtility ::Matrix4Identity();
	// Z����]�̊e�v�f
	matRotZ.m[0][0] = (float)cos(worldTransform_.rotation_.z);
	matRotZ.m[0][1] = (float)sin(worldTransform_.rotation_.z);
	matRotZ.m[1][0] = (float)-sin(worldTransform_.rotation_.z);
	matRotZ.m[1][1] = (float)cos(worldTransform_.rotation_.z);
	// X����]�̊e�v�f
	matRotX.m[1][1] = (float)cos(worldTransform_.rotation_.x);
	matRotX.m[1][2] = (float)sin(worldTransform_.rotation_.x);
	matRotX.m[2][1] = (float)-sin(worldTransform_.rotation_.x);
	matRotX.m[2][2] = (float)cos(worldTransform_.rotation_.x);
	// Y����]�̊e�v�f
	matRotY.m[0][0] = (float)cos(worldTransform_.rotation_.y);
	matRotY.m[0][2] = (float)-sin(worldTransform_.rotation_.y);
	matRotY.m[2][0] = (float)sin(worldTransform_.rotation_.y);
	matRotY.m[2][2] = (float)cos(worldTransform_.rotation_.y);

	//�e���p��]�s���錾
	matRot *= matRotZ *= matRotX *= matRotY;

	worldTransform_.matWorld_ = MathUtility ::Matrix4Identity();
	worldTransform_.matWorld_ *= matRot;

	//�s��̓]��
	worldTransform_.TransferMatrix();
}
void Translate(WorldTransform &worldTransform_) {//�ړ�

	//���s�ړ��s���錾
	Matrix4 matTrans;

	//���s�ړ����s���錾
	matTrans = MathUtility ::Matrix4Identity();

	matTrans.m[3][0] += worldTransform_.translation_.x;
	matTrans.m[3][1] += worldTransform_.translation_.y;
	matTrans.m[3][2] += worldTransform_.translation_.z;

	worldTransform_.matWorld_ = MathUtility ::Matrix4Identity();
	worldTransform_.matWorld_ *= matTrans;
}

void Transfer(WorldTransform& worldTransform_) { //��̑S��
  

	//�X�P�[�����O�s���錾
	Matrix4 matScale;
	//��]�s���錾
	Matrix4 matRot;
	Matrix4 matRotX, matRotY, matRotZ;
	//���s�ړ��s���錾
	Matrix4 matTrans;

	matScale = MathUtility ::Matrix4Identity();
	matRot = MathUtility ::Matrix4Identity();
	matRotX = MathUtility ::Matrix4Identity();
	matRotY = MathUtility ::Matrix4Identity();
	matRotZ = MathUtility ::Matrix4Identity();
	matTrans = MathUtility ::Matrix4Identity();

	//�{��
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;
	// Z����]�̊e�v�f
	matRotZ.m[0][0] = (float)cos(worldTransform_.rotation_.z);
	matRotZ.m[0][1] = (float)sin(worldTransform_.rotation_.z);
	matRotZ.m[1][0] = (float)-sin(worldTransform_.rotation_.z);
	matRotZ.m[1][1] = (float)cos(worldTransform_.rotation_.z);
	// X����]�̊e�v�f
	matRotX.m[1][1] = (float)cos(worldTransform_.rotation_.x);
	matRotX.m[1][2] = (float)sin(worldTransform_.rotation_.x);
	matRotX.m[2][1] = (float)-sin(worldTransform_.rotation_.x);
	matRotX.m[2][2] = (float)cos(worldTransform_.rotation_.x);
	// Y����]�̊e�v�f
	matRotY.m[0][0] = (float)cos(worldTransform_.rotation_.y);
	matRotY.m[0][2] = (float)-sin(worldTransform_.rotation_.y);
	matRotY.m[2][0] = (float)sin(worldTransform_.rotation_.y);
	matRotY.m[2][2] = (float)cos(worldTransform_.rotation_.y);
	//���s�ړ�
	matTrans.m[3][0] += worldTransform_.translation_.x;
	matTrans.m[3][1] += worldTransform_.translation_.y;
	matTrans.m[3][2] += worldTransform_.translation_.z;

	//�e���p��]�s���錾
	matRot *= matRotZ *= matRotX *= matRotY;

	worldTransform_.matWorld_ = MathUtility ::Matrix4Identity();
	worldTransform_.matWorld_ = matScale *= matRot *= matTrans;

}

void VecMat(Vector3 vec3, WorldTransform& worldTransform_) 
{
	/*vec3.x = (vec3.x * worldTransform_.matWorld_.m[0][0]) +
	         (vec3.y * worldTransform_.matWorld_.m[1][0]) +
	         (vec3.z * worldTransform_.matWorld_.m[2][0]) + 
			 (0 * worldTransform_.matWorld_.m[3][0]);

	vec3.y = (vec3.x * worldTransform_.matWorld_.m[0][1]) +
	         (vec3.y * worldTransform_.matWorld_.m[1][1]) +
	         (vec3.z * worldTransform_.matWorld_.m[2][1]) + 
			 (0 * worldTransform_.matWorld_.m[3][1]);

	vec3.z = (vec3.x * worldTransform_.matWorld_.m[0][2]) +
	         (vec3.y * worldTransform_.matWorld_.m[1][2]) +
	         (vec3.z * worldTransform_.matWorld_.m[2][2]) + 
			 (0 * worldTransform_.matWorld_.m[3][2]);*/

	//vec3.x = 10;
	
	//return vec3;
}

}


