#pragma once
#include <math.h>
#include "WorldTransform.h"

namespace Vector3Math 
{
Vector3 diff(Vector3& vecA,Vector3&vecB);//����(A - B)
bool norm(Vector3& vec3);//���������߂�
Vector3 Normalize(Vector3& vec3);//���K��
}