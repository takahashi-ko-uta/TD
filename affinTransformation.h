#pragma once
#include "WorldTransform.h"

namespace affinTransformation
{
void Scale(WorldTransform& worldTransform_);     //�X�P�[�����O
void Rotate(WorldTransform& worldTransform_);    //��]
void Translate(WorldTransform& worldTransform_); //�ړ�
void Transfer(WorldTransform& worldTransform_); //��̑S��		
void VecMat(Vector3 vec3, WorldTransform& worldTransform_);//�x�N�g���ƍs��̊|���Z
}