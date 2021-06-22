#include "game.h"
#include "bullet.h"
#include "input.h"
#include "result.h"
#include "model.h"
#include "wall.h"
#include "camera.h"
#include "Billboard.h"
#include "bullet.h"
#include "meshfield.h"
#include "light.h"
#include "object.h"
#include "fade.h"
#include "gamepad.h"
#include "item.h"
#include "Coin.h"


//グローバル変数
bool g_bPause = false;//ポーズ中かどうか
int g_pData;
int nLife;
float fAngle;
D3DXVECTOR3 move;

HRESULT InitGame(void)
{
	g_pData = 1;

	if (g_pData == 1)
	{

	}
	g_bPause = false;

	InitBillboard();
	InitLight();
	InitCamera();
	InitModel();
	InitCoin();
	InitMeshfield();
	//InitEnemy();
	//Initsph();


	Player *pModel;
	pModel = GetModel();

	InitObject();
	SetObject(D3DXVECTOR3(-390.0f, 0.0f, 350.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
	SetObject(D3DXVECTOR3(-410.0f, 0.0f, -450.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
	SetObject(D3DXVECTOR3(410.0f, 0.0f, -450.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
	SetObject(D3DXVECTOR3(390.0f, 0.0f, 350.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
	SetObject(D3DXVECTOR3(200.0f, 0.0f, 250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetObject(D3DXVECTOR3(-200.0f, 0.0f, 250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetObject(D3DXVECTOR3(200.0f, 0.0f, -250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetObject(D3DXVECTOR3(-200.0f, 0.0f, -250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetObject(D3DXVECTOR3(-450.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
	SetObject(D3DXVECTOR3(450.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);


	InitBullet();
	InitWall();
	//囲む檻
	SetWall(D3DXVECTOR3(500.0f, 0.0f, 270.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(500.0f, 0.0f, -270.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-500.0f, 0.0f, 270.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-500.0f, 0.0f, -270.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(270.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-270.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(270.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-270.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	//上通路
	SetWall(D3DXVECTOR3(110.0f, 0.0f, 610.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(110.0f, 0.0f, 880.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-110.0f, 0.0f, 610.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-110.0f, 0.0f, 880.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(110.0f, 0.0f, 1150.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-110.0f, 0.0f, 1150.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	//下通路
	SetWall(D3DXVECTOR3(110.0f, 0.0f, -610.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(110.0f, 0.0f, -880.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-110.0f, 0.0f, -610.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-110.0f, 0.0f, -880.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(110.0f, 0.0f, -1150.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-110.0f, 0.0f, -1150.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	//右通路
	SetWall(D3DXVECTOR3(610.0f, 0.0f, 110.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(880.0f, 0.0f, 110.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(1150.0f, 0.0f, 110.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-610.0f, 0.0f, 110.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-880.0f, 0.0f, 110.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-1150.0f, 0.0f, 110.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	//左通路
	SetWall(D3DXVECTOR3(610.0f, 0.0f, -110.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(880.0f, 0.0f, -110.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(1150.0f, 0.0f, -110.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-610.0f, 0.0f, -110.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-880.0f, 0.0f, -110.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-1150.0f, 0.0f, -110.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);

	//下のボス部屋
	SetWall(D3DXVECTOR3(220.0f, 0.0f, -1270.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-220.0f, 0.0f, -1270.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(350.0f, 0.0f, -1400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-350.0f, 0.0f, -1400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(350.0f, 0.0f, -1670.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-350.0f, 0.0f, -1670.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(350.0f, 0.0f, -1940.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-350.0f, 0.0f, -1940.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(150.0f, 0.0f, -2100.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-150.0f, 0.0f, -2100.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);


	//下のボス部屋
	SetWall(D3DXVECTOR3(220.0f, 0.0f, 1270.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-220.0f, 0.0f, 1270.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(350.0f, 0.0f, 1400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-350.0f, 0.0f, 1400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(350.0f, 0.0f, 1670.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-350.0f, 0.0f, 1670.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(350.0f, 0.0f, 1940.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-350.0f, 0.0f, 1940.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetWall(D3DXVECTOR3(150.0f, 0.0f, 2100.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);
	SetWall(D3DXVECTOR3(-150.0f, 0.0f, 2100.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0);

	InitItem();
	for (int nCntStone = 0; nCntStone < 10; nCntStone++)
	{
		SetItem(D3DXVECTOR3(0.0f, 50.0f, 500.0f + (nCntStone * 100.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	for (int nCntStone = 0; nCntStone < 10; nCntStone++)
	{
		SetItem(D3DXVECTOR3(0.0f, 50.0f, -500.0f + (nCntStone * -100.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}


	return S_OK;
}

void UninitGame(void)
{
	UninitBillboard();
	UninitCamera();
	UninitLight();
	UninitModel();
	//UninitEnemy();
	UninitObject();
	UninitBullet();
	UninitCoin();
	//Uninitsph();
	UninitWall();
	UninitMeshfield();
	UninitItem();
}

void UpdateGame(void)
{ 
	//ポーズ画面の処理
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = g_bPause ? false : true;
	}
	if (g_bPause == true)
	{
	}
	else
	{
		UpdateLight();
		UpdateCamera();
		UpdateModel();
		//UpdateEnemy();
		UpdateObject();
		UpdateBullet();
		UpdateWall();
		UpdateBillboard();
		UpdateCoin();
		//Updatesph();
		UpdateMeshfield();
		UpdateItem();
	}
}

void DrawGame(void)
{
	SetCamera();
	//DrawEnemy();
	DrawMeshfield();
	DrawWall();
	DrawObject();
	DrawCoin();
	DrawBullet();
	DrawBillboard();
	DrawItem();
	DrawModel();
	//Drawsph();

	if (g_bPause == true)
	{
	}
}
int GetGameNumber(void)
{
	return g_pData;
}
int GetGameNumber2(void)
{
	return S_OK; 
}
