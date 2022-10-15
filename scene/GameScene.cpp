#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"
#include "MathUtility.h"
#include <random>

#include "nameSpace/affinTransformation.h"

GameScene::GameScene() {}

GameScene::~GameScene()
{ 
	delete model_;
	delete debugCamera_;
	delete player_;
	delete notesStart_;
	delete notesHit_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_PL_ = TextureManager::Load("mario.jpg");
	textureHandle_EN_ = TextureManager::Load("enemy.jpg");
	textureHandle_Black_ = TextureManager::Load("black.jpg");
	textureHandle_Red_ = TextureManager::Load("red.png");
	//タイトル用テクスチャ
	titlePic[0] = TextureManager::Load("title1.png");
	title[0] = Sprite::Create(titlePic[0], { 0, 0 });
	//サウンドを読み込む
	soundDataHandle_ = audio_->LoadWave("u002.wav");	//タイトルのBGM
	soundDataHandle2_ = audio_->LoadWave("mokugyo.wav");//メトロノーム

	//モデル生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
		
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバックカメラの生成
	debugCamera_ = new DebugCamera(600, 600);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向は参照するビュープロジェクションを指定する
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
	
	Vector3 ten_move[8];

#pragma region 各クラスの生成と初期化
	//自キャラの生成と初期化
	player_ = new Player();
	player_->Initalize(model_,textureHandle_PL_);

	//敵キャラの生成と初期化
	enemy_ = new Enemy();
	enemy_->Initalize(model_);

	//ノーツの当たり判定取るやつ生成と初期化
	notesHit_ = new NotesHit();
	notesHit_->Initalize(model_, textureHandle_Black_);

	//InNotesを消すやつの生成と初期化
	notesDelete_ = new NotesDelete();
	notesDelete_->Initalize(model_, textureHandle_Red_);

	//ノーツの出発点の生成と初期化
	notesStart_ = new NotesStart();
	notesStart_->Initalize(model_, textureHandle_EN_);
	//ノーツの出発点にアドレスを渡す
	notesStart_->SetPlayer(player_);
	notesStart_->SetNotesHit(notesHit_);

	//ノーツの出発点の生成と初期化
	notesEnd_ = new NotesEnd();
	notesEnd_->Initalize(model_, textureHandle_EN_);

	//スカイドームの生成と初期化
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);
#pragma endregion

	bgmHandle_ = audio_->PlayWave(soundDataHandle_, true, 0.1f);
	//bgmHandle2_ = audio_->PlayWave(soundDataHandle2_, true, 0.1f);
}

void GameScene::Update()
{
	bgmHandle_;
	//デバックカメラの更新
	debugCamera_->Update();

	switch (scene) {
#pragma region タイトル
	case 0://タイトル
		
		audio_->StopWave(soundDataHandle2_);

		if (input_->TriggerKey(DIK_SPACE)) {
			scene = 1;
		}
		break;
#pragma endregion

#pragma region ゲーム
	case 1://ゲーム
		if (input_->TriggerKey(DIK_O)) {

			scene = 2;

		}

#pragma region 各更新処理
		//自キャラの更新
		player_->Update();

		//敵キャラの更新
		enemy_->Update();

		//ノーツの出発点の更新
		notesStart_->Update();

		//ノーツの終着点の更新
		notesEnd_->Update();

		// notesHitの更新
		notesHit_->Update();

		//InNotesを消すやつの更新
		notesDelete_->Update();

		//スカイドームの更新
		skydome_->Update();

		//衝突判定
		CheckAllCollisons();

		SceneChenge();

		//成功判定
		TriggerJudge();
#pragma endregion
		
#pragma region 音の処理
		
		if (soundkeep == 0)
		{
			audio_->StopWave(soundDataHandle_);
			
			soundkeep = 1;
		}
		Metronome(30, soundDataHandle2_);

		debugText_->SetPos(50, 110);
		debugText_->Printf("soundLevel:%f", soundLevel);
		
#pragma endregion

		break;
#pragma endregion 

#pragma region エンディング
	case 2://エンディング
		audio_->StopWave(soundDataHandle2_);


		break;
#pragma endregion

	}
}

void GameScene::CheckAllCollisons() 
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//敵弾リストの取得
	const std::list<std::unique_ptr<Notes>>& notes = notesStart_->GetNotes();
	const std::list<std::unique_ptr<InNotes>>& inNotes = notesStart_->GetInNotes();

#pragma region NotesEndとnotesの当たり判定
	//自キャラの座標
	posA = notesEnd_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<Notes>& bullet : notes) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		const float AR = 1;
		const float BR = 1;

		float A = pow((posB.x - posA.x), 2) + pow((posB.y - posA.y), 2) + pow((posB.z - posA.z), 2);
		float B = pow((AR + BR), 2);

		if (A <= B) {
			//自キャラの衝突時コールバックを呼び出す
			
			//敵弾の衝突時コールバックを呼び出す
			bullet->deleteNotes();
		}

	}

#pragma endregion

#pragma region NotesHitとnotesの当たり判定
	//自キャラの座標
	posA = notesHit_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<Notes>& bullet : notes) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		const float AR = 1;
		const float BR = 1;

		float A = pow((posB.x - posA.x), 2) + pow((posB.y - posA.y), 2) + pow((posB.z - posA.z), 2);
		float B = pow((AR + BR), 2);

		if (A <= B) {
			//自キャラの衝突時コールバックを呼び出す
			notesHit_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
		else
		{
			bullet->NotCollision();
		}
	}

#pragma endregion

#pragma region NotesHitとinNotesの当たり判定
	//自キャラの座標
	posA = notesHit_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<InNotes>& bullet : inNotes) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		const float AR = 1;
		const float BR = 1;

		float A = pow((posB.x - posA.x), 2) + pow((posB.y - posA.y), 2) + pow((posB.z - posA.z), 2);
		float B = pow((AR + BR), 2);

		if (A <= B) {
			//自キャラの衝突時コールバックを呼び出す
			notesHit_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}

	}

#pragma endregion

#pragma region NotesDeleteとinNotesの当たり判定
	//自キャラの座標
	posA = notesDelete_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<InNotes>& bullet : inNotes) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		const float AR = 1;
		const float BR = 1;

		float A = pow((posB.x - posA.x), 2) + pow((posB.y - posA.y), 2) + pow((posB.z - posA.z), 2);
		float B = pow((AR + BR), 2);

		if (A <= B) {
			bullet->deleteNotes();//inNotesを消す
			notesCount++;
			judge_failure++;//押し逃してしまっているのでミスカウント
		}
	}

#pragma endregion

}

void GameScene::TriggerJudge() 
{
	
	bool playerTrigger = false;
	bool notesTrigger = false;
	bool inNotesTrigger = false;
	bool failureFlag = false;

	//敵弾リストの取得
	const std::list<std::unique_ptr<Notes>>& notes = notesStart_->GetNotes();
	const std::list<std::unique_ptr<InNotes>>& inNotes = notesStart_->GetInNotes();

	playerTrigger = player_->GetTrigger();			//playerクラスからtriggerを取得する

#pragma region 成功失敗判定

#pragma region 成功したとき
	for (const std::unique_ptr<InNotes>& inNote : inNotes)
	{
		inNotesTrigger = inNote->GetTrigger();	//inNotesクラスからtriggerを取得する　
		if (inNotesTrigger == true && playerTrigger == true)
		{
			judge_success++;
			inNote->deleteNotes();
			notesCount++;
			if(judge_failure >= 0)
			{
				judge_failure--;
			}
		}
	}
#pragma endregion

#pragma region 失敗したとき
	for (const std::unique_ptr<Notes>& note : notes) 
	{
		notesTrigger = note->GetTrigger();			//notesクラスからtriggerを取得する
		for (const std::unique_ptr<InNotes>& inNote : inNotes)
		{
			inNotesTrigger = inNote->GetTrigger();	//inNotesクラスからtriggerを取得する　
			if (notesTrigger == false && inNotesTrigger == false && playerTrigger == true)//手前で押したとき
			{
				failureFlag = true;
			}
			else 
			{
				failureFlag = false;
			}
		}
	}

	if(failureFlag == true)
	{
		judge_failure++;
	}
#pragma endregion 

#pragma endregion

	debugText_->SetPos(50, 70);
	debugText_->Printf("success:%d ,failure:%d ,notesCount:%d", judge_success,judge_failure,notesCount);
}

void GameScene::SceneChenge()
{
	enemy_->SetEnemyNumber(enemyNumber);
	if(input_->PushKey(DIK_1))//仮に[1]を押したとき
	{
		upFlag = 1;
	}
	if (viewProjection_.target.y <= 90 && upFlag == 1)//視点を上げる
	{
		move = { 0, kTargetSpeed, 0 };
		//viewProjection_.target.y++;
		viewProjection_.target.y += move.y;
		if (viewProjection_.target.y >= 90)//真上を向いたら、下げるフェーズに移行
		{
			upFlag = 0;
			downFlag = 1;
#pragma region 敵の選択

			//敵の数が(仮に)3つのため1~3で回す
			enemyNumber++;
			if(enemyNumber == 4)
			{
				enemyNumber = 1;
			}
			enemy_->SetEnemyNumber(enemyNumber);

#pragma endregion
		}
	}
	if (viewProjection_.target.y >= 0 && downFlag == 1)//視点を下げる
	{
		move = { 0, -kTargetSpeed, 0 };
		viewProjection_.target.y += move.y;
		if (viewProjection_.target.y <= 0)//視点が元の位置(viewProjection_.target.y = 0のとき)に戻ったら下げるのを止める
		{
			downFlag = 0;
		}
	}
	//注視点移動
	/*viewProjection_.target.x += move.x;
	viewProjection_.target.y += move.y;
	viewProjection_.target.z += move.z;*/

	//行列の再計算
	viewProjection_.UpdateMatrix();
	//デバック用表示
	debugText_->SetPos(50, 90);
	debugText_->Printf("enemyNumber:%d", enemyNumber);

}

void GameScene::Metronome(float tempo ,uint32_t bgmHandle)
{
	soundLevel = enemy_->GetSoundLevel();
	interval = tempo;

	if (timer >= 0)
	{
		timer--;
		if (timer <= 0)
		{
			audio_->PlayWave(soundDataHandle2_, false, soundLevel);
			timer = interval;
		}
	}

	debugText_->SetPos(50, 130);
	debugText_->Printf("timer:%f", timer);
	//audio_->PlayWave(bgmHandle, true, 0.5f);
}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	
	

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/// 
	//3Dモデル描画
	
	
	//自キャラの描画
	player_->Draw(viewProjection_);

	enemy_->Draw(viewProjection_);

	notesStart_->Draw(viewProjection_);

	notesEnd_->Draw(viewProjection_);

	notesHit_->Draw(viewProjection_);

	notesDelete_->Draw(viewProjection_);

	skydome_->Draw(viewProjection_);
	
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)

	

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	switch (scene) {
	case 0:
		title[titlenum]->Draw();
		break;
	case 1:

		break;
	}
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
	
}
