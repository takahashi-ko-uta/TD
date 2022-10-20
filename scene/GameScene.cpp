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
#pragma region テクスチャとサウンドのロード
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_PL_ = TextureManager::Load("mario.jpg");
	textureHandle_EN_ = TextureManager::Load("enemy.jpg");
	textureHandle_Black_ = TextureManager::Load("black.jpg");
	textureHandle_Red_ = TextureManager::Load("red.png");
	//タイトル用テクスチャ
	titlePic[0] = TextureManager::Load("title1.png");
	title[0] = Sprite::Create(titlePic[0], { 0, 0 });
	//チュートリアル用テクスチャ
	textureHandle_Tyu_ = TextureManager::Load("tutorial.png");
	textureHandle_Clear = TextureManager::Load("Clear.png");
	textClear = Sprite::Create(textureHandle_Clear, { 850.0f,50.0f });
	textureHandle_text1 = TextureManager::Load("text1.png");
	text1 = Sprite::Create(textureHandle_text1, { 850.0f,50.0f });
	textureHandle_text2 = TextureManager::Load("text2.png");
	text2 = Sprite::Create(textureHandle_text2, { 850.0f,50.0f });
	textureHandle_text3 = TextureManager::Load("text3.png");
	text3 = Sprite::Create(textureHandle_text3, { 850.0f,50.0f });
	textureHandle_text4 = TextureManager::Load("text4.png");
	text4 = Sprite::Create(textureHandle_text4, { 850.0f,50.0f });
	textureHandle_text5 = TextureManager::Load("text5.png");
	text5 = Sprite::Create(textureHandle_text5, { 850.0f,50.0f });

	//敵の説明
	textureHandle_Exp1 = TextureManager::Load("tutorial.png");
	Exp1 = Sprite::Create(textureHandle_Exp1, { 50.0f,50.0f });
	//サウンドを読み込む
	soundData_title = audio_->LoadWave("u002.wav");	//タイトルのBGM
	soundData_tempo = audio_->LoadWave("mokugyo.wav");//メトロノーム
	soundData_gameBGM120 = audio_->LoadWave("gameBGM_120.wav");
	//soundData_gameBGM150 = audio_->LoadWave("gameBGM_150.wav");


#pragma endregion
	//モデル生成
	model_ = Model::Create();

	//modelNum1_ = Model::CreateFromOBJ("num1", true);
	//modelNum2_ = Model::CreateFromOBJ("num2", true);
	modelNum3_ = Model::CreateFromOBJ("num3", true);

	//model_->SetMaterialParameta(0.1f);
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
		
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	viewProjection_.eye = Vector3(0.0f, 0.0f, 0.0f);
	viewProjection_.target = Vector3(0.0f, 0.0f, 50.01f); //target.zは0にすると見えなくなるため0.01追加
	viewProjection_.UpdateMatrix();
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
	enemy_->Initalize(model_);//, modelNum2_, modelNum3_);

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

	bgmHandle_ = audio_->PlayWave(soundData_title, true, 0.1f);
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
		
		audio_->StopWave(soundData_tempo);

		if (input_->TriggerKey(DIK_SPACE)) {//シーン切り替え
			scene = 1;
		}
		break;
#pragma endregion

#pragma region ゲーム
	case 1://ゲーム
		if (input_->TriggerKey(DIK_O)) { //シーン切り替え

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
			audio_->PlayWave(soundData_gameBGM120, true, 0.5f);
			audio_->StopWave(soundData_title);
			
			soundkeep = 1;
		}
		Metronome(30, soundData_tempo);

#pragma endregion

		break;
#pragma endregion 

#pragma region エンディング
	case 2://エンディング
		audio_->StopWave(soundData_tempo);


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
	
	kTargetSpeed = 0.5f;
	enemy_->SetEnemyNumber(sceneNumber);

	//if(input_->PushKey(DIK_1))//仮に[1]を押したとき
	//{
	//	upFlag = 1;
	//}

	switch (sceneNumber)
	{
	case 0:
		Tutorial();
		break;
	case 1:

		break;
	case 2:

		break;
	case 3:
		if (soundStart == 0)
		{
			audio_->PlayWave(soundData_gameBGM150, true, 0.5f);
			audio_->StopWave(soundData_gameBGM120);

			soundStart = 1;
		}
		break;
	}


#pragma region 視点を上下してシーンを切り替える

	//------「視点を上げる」→「視点を止める」→「視点を下げる」-----//
	//「視点を上げる」...視点を真上まで上げてEnemyNumberを変える
	//「視点を止める」...視点を真上の状態で止める(timerで管理)ここで一度リズム合わせをする
	//「視点を下げる」...視点ををもとに戻す

#pragma region 視点を上げる
	if (viewProjection_.target.y <= 90 && upFlag == 1)//視点を上げる
	{
		move = { 0, kTargetSpeed, -kTargetSpeed };
		viewProjection_.target.y += move.y;
		viewProjection_.target.z += move.z;
		if (viewProjection_.target.y >= 50)//真上を向いたら、下げるフェーズに移行
		{
			upFlag = 0;
			stopFlag = 1;
#pragma region 敵の選択

			//敵の数が(仮に)3つのため1~3で回す
			sceneNumber++;
			if (sceneNumber == 4)
			{
				sceneNumber = 1;
			}
			enemy_->SetEnemyNumber(sceneNumber);

#pragma endregion
		}
	}
#pragma endregion

#pragma region 視点を止める
	if (stopFlag == 1)//視点を止める
	{
		float stopTime = 60; //真上で視点を止める時間
		stopTimer++;
		if (stopTimer > stopTime)
		{
			stopTimer = 0;
			stopFlag = 0;
			downFlag = 1;
		}
	}
#pragma endregion

#pragma region 視点を下げる
	if (viewProjection_.target.y >= 0 && downFlag == 1)//視点を下げる
	{
		move = { 0, -kTargetSpeed, +kTargetSpeed };
		viewProjection_.target.y += move.y;
		viewProjection_.target.z += move.z;
		if (viewProjection_.target.y <= 0)//視点が元の位置(viewProjection_.target.y = 0のとき)に戻ったら下げるのを止める
		{
			downFlag = 0;
		}
	}
#pragma endregion

#pragma endregion

	//各クラスにstopFlagを渡す
	notesDelete_->SetStopFlag(stopFlag);
	notesEnd_->SetStopFlag(stopFlag);
	notesStart_->SetStopFlag(stopFlag);
	notesHit_->SetStopFlag(stopFlag);

	//行列の再計算
	viewProjection_.UpdateMatrix();
	//デバック用表示
	debugText_->SetPos(50, 90);
	debugText_->Printf("sceneNumber:%d", sceneNumber);
}


void GameScene::Tutorial()
{
	switch (scene_tutorial)
	{
	case 0://数秒待つ
		time_scene0 = 60;
		timer_tutorial++;
		if(timer_tutorial > time_scene0)
		{
			timer_tutorial = 0;
			scene_tutorial = 1;
			successResetFlag = 1;
		}
		break;
	case 1:
		time_scene1 = 120;
		timer_tutorial++;
		if (timer_tutorial > time_scene1)
		{
			timer_tutorial = 0;
			scene_tutorial = 2;
		}
		break;
	case 2:
		time_scene2 = 120;
		timer_tutorial++;
		if (timer_tutorial > time_scene2)
		{
			timer_tutorial = 0;
			scene_tutorial = 3;
		}
		break;
	case 3:
		time_scene3 = 120;
		timer_tutorial++;
		if (timer_tutorial > time_scene3)
		{
			timer_tutorial = 0;
			scene_tutorial = 4;
		}
		break;
	case 4:
		time_scene4 = 120;
		timer_tutorial++;
		if (timer_tutorial > time_scene4)
		{
			timer_tutorial = 0;
			scene_tutorial = 5;
		}
		break;
	case 5://テキスト5を入れる＋10回成功したらscene2へ
		if(successResetFlag == 1)//一度、成功回数を0へ
		{
			judge_success = 0;
			successResetFlag = 0;
		}

		if(judge_success >= 10)
		{
			scene_tutorial = 6;
		}

		break;
	case 6://[クリア]を表示して数秒待つ
		time_scene6 = 120;
		timer_tutorial++;
		if (timer_tutorial > time_scene6)
		{
			timer_tutorial = 0;
			scene_tutorial = 7;
		}
		break;
	case 7://テキスト2を入れて、数秒まつ
		time_scene7 = 120;
		timer_tutorial++;
		if (timer_tutorial > time_scene7)
		{
			timer_tutorial = 0;
			scene_tutorial = 8;
		}
		break;

	case 8://ステージ１へ
		upFlag = 1;
		break;
	}

	debugText_->SetPos(50, 150);
	debugText_->Printf("scene_tutorial:%d", scene_tutorial);
	debugText_->SetPos(50, 170);
	debugText_->Printf("timer_tutorial:%f", timer_tutorial);

}

void GameScene::Stage1()
{

}

void GameScene::Stage2()
{

}

void GameScene::Stage3()
{

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
			audio_->PlayWave(soundData_tempo, false, soundLevel);
			timer = interval;
		}
	}
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
	if(scene_tutorial == 1)
	{
		
	}
	else if(scene_tutorial == 2)
	{
		
	}
	switch (scene_tutorial)
	{
	case 1:
		text1->Draw();
		break;
	case 2:
		text2->Draw();
		break;
	case 3:
		text3->Draw();
		break;
	case 4:
		text4->Draw();
		break;
	case 5:
		text5->Draw();
		break;
	case 6:
		textClear->Draw();
		break;
	case 7:
		Exp1->Draw();
		break;
	}
	
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();
	

#pragma endregion
	
}

