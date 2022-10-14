#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model) {
	assert(model);

	model_ = model;
	worldTransform_.Initialize();

	worldTransform_.matWorld_ *= MathUtility::Matrix4Scaling(scale.x, scale.y, scale.z);
	worldTransform_.matWorld_ *= MathUtility::Matrix4Translation(trans.x, trans.y, trans.z);

	worldTransform_.TransferMatrix();
}
void Skydome::Update() {}

void Skydome::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection);
}
