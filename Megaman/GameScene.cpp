#include "GameScene.h"

void GameScene::Initialize()
{

	tree = new Quadtree(1, new Rect(0, 0, 8000, 8000));

	tree->LoadTree();
	//tree->PrintObjectWhere();
	//tree->TestObjectCreate();

	map.Initialize();
	map2.Initialize();
	// MapCollision::LoadMapCollision();
	/*cout << "Call From GameScene.cpp Line 8" << endl;
	MapCollision::PrintList();*/
	

	megaman = Megaman::getInstance();
	megaman->Initialize();
	megaman->SetPosition(300, 3320);
	//MapCollision::CreateMapCollision(0, 0, 3200, 3000, 50);
	//megaman->SetPosition(2100, 2904);
	//megaman->SetPosition(0, 0);

	//Layer2::CreateLayer2(2200, 2904);

	//Spawner<NotorBanger>(D3DXVECTOR2(400,3220));				// Spawn con NotorBanger
	//Spawner<HeadHunter>(D3DXVECTOR2(891, 3220), false);
	//Spawner<Helit>(D3DXVECTOR2(400, 3220), true);
	Spawner<BlastHornet>(D3DXVECTOR2(2491, 3000), true);
	

	//mapcollision1.Initialize(500,0,3200,3000,20);
	//mapcollision1.SetName("rightScroller");

	ele.Initialize(892,3130,64,16);
	gate1.Initialize(2307, 2912, 16, 48);
	gate2.Initialize(2549, 2912, 16, 48);

	//Setting Camera
	Camera::getInstance()->SetPosition(0, 0);

	HUD::getInstance()->Ghost_Initialize();

	//////////////////////////////////////////

}

void GameScene::UpdateInput(float deltatime)
{
	ele.UpdateInput(deltatime);
	
	Camera::getInstance()->UpdateInput(deltatime);
	megaman->UpdateInput(deltatime);
	
}

void GameScene::Update(float deltatime)
{
	D3DXVECTOR3 temp1 = megaman->GetPosition();
	D3DXVECTOR3 temp2 = ele.GetPosition();
	megaman->Update(deltatime);
	ele.Update(deltatime);
	gate1.Update(deltatime);
	gate2.Update(deltatime);

	temp1.y = megaman->GetPosition().y - temp1.y;
	temp2.y = ele.GetPosition().y - temp2.y;

	NotorBanger::UpdateAll(deltatime);
	HeadHunter::UpdateAll(deltatime);
	Helit::UpdateAll(deltatime);
	NotorBullet::UpdateAll(deltatime);
	Rocket::UpdateAll(deltatime);
	HelitRocket::UpdateAll(deltatime);
	BlastHornet::UpdateAll(deltatime);
	Bee::UpdateAll(deltatime);
	Item::UpdateAll(deltatime);

	Camera::getInstance()->Update(deltatime,megaman->GetPosition());	
	tree->DynamicLoad(deltatime);
	tree->DynamicunLoad(deltatime);

	HUD::getInstance()->Update(deltatime);
	//cout << megaman->GetPosition().x << "\t" << megaman->GetPosition().y << "\t\tCall From GameScene.cpp Line: 50" << endl; // 3091 3118
}

void GameScene::Draw()
{
	Camera::getInstance();
	map.Draw();
	

	megaman->Draw();
	NotorBanger::DrawAll();
	HeadHunter::DrawAll();
	Helit::DrawAll();
	NotorBullet::DrawAll();
	Rocket::DrawAll();
	HelitRocket::DrawAll();
	BlastHornet::DrawAll();
	Bee::DrawAll();
	Item::DrawAll();
	Layer2::DrawAll();
	HUD::getInstance()->Draw();

	ele.Draw();
	map2.Draw();
	gate1.Draw();
	gate2.Draw();
}
