//=============================================================================
// DX ライブラリを使用したモデルビューアのサンプル
//-----------------------------------------------------------------------------
//=============================================================================

#include <DxLib.h>
#include <LuminoMathDxLib.h>
#include "../Common.h"
using namespace Lumino;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)	
		return -1;

	SetCameraNearFar(1.0f, 150.0f);

	int model = MV1LoadModel(LOCAL_RESOURCE("model.x"));

	Quaternion modelRot;	// モデルの回転姿勢
	int lastMouseX = -1;	// マウスドラッグ量を調べるための変数 (前回のクリック位置)
	int lastMouseY = -1;	// マウスドラッグ量を調べるための変数 (前回のクリック位置)
	float cameraZ = -5.0f;	// カメラの前後位置

	// メインループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//-----------------------------------------------------
		// マウス移動量から回転姿勢を更新

		int mouseState = GetMouseInput();
		if (mouseState & MOUSE_INPUT_LEFT)
		{
			int mouseX, mouseY;
			GetMousePoint(&mouseX, &mouseY);

			if (lastMouseX >= 0)
			{
				// 前回のマウスX座標との差分を角度として、現在の姿勢をY軸周りで回転する
				modelRot.RotateY(0.01f * (lastMouseX - mouseX));
			}
			
			if (lastMouseY >= 0)
			{
				// 前回のマウスY座標との差分を角度として、現在の姿勢をX軸周りで回転する
				modelRot.RotateX(0.01f * (lastMouseY - mouseY));
			}
			
			lastMouseX = mouseX;
			lastMouseY = mouseY;
		}
		else
		{
			// マウスドラッグ解除
			lastMouseX = -1;
			lastMouseY = -1;
		}

		// カメラ位置はマウスホイールの移動量から適当に設定
		cameraZ += GetMouseWheelRotVol();


		//-----------------------------------------------------
		// 描画処理

		ClearDrawScreen();

		// カメラ位置の設定
		SetCameraPositionAndTargetAndUpVec(
			VGet(0, 0, cameraZ),
			VGet(0, 0, 0),
			VGet(0, 1, 0));

		// クォータニオン(回転姿勢)を行列に変換してモデルに設定、描画する
		Matrix modelMat = Matrix::RotationQuaternion(modelRot);
		MV1SetMatrix(model, modelMat);
		MV1DrawModel(model);

		// info
		DrawString(0, 0, "マウス左ドラッグでモデルを回転します。", GetColor(255, 255, 255));
		DrawString(0, 16, "マウスホイールでカメラを前後に移動します。", GetColor(255, 255, 255));

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}