//=============================================================================
// DX ライブラリを使用したフライトシミュレータのサンプル
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

	int playerModel = MV1LoadModel(LOCAL_RESOURCE("Plane1.x"));
	int skyModel = MV1LoadModel(LOCAL_RESOURCE("BG_Sky1.x"));
	int groundModel = MV1LoadModel(LOCAL_RESOURCE("Ground1.x"));

	SetCameraNearFar(1.0f, 10000.0f);
	SetupCamera_Perspective(Math::PI / 2);

	Quaternion playerRot;			// 自機の回転情報
	Vector3 playerPos;				// 自機の位置
	float velocity = 5.0f;			// 自機の速度
	float rotVelocity = 0.02f;		// 自機の回転速度

	// メインループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//-----------------------------------------------------
		// 自機の姿勢更新

		// ↓ キー
		if (CheckHitKey(KEY_INPUT_DOWN) == 1)
		{
			// ピッチは自機が原点にいるとすると、X軸周り、つまり右手方向を軸とした回転と考えることができる。
			// X 軸を表すベクトルは (1, 0, 0) で、Vector3::UnitX 定数がこれを表す。
			// このベクトルを自機の現在の回転姿勢 (playerRot) で座標変換すると、
			// 現在の姿勢から見た右手方向がわかる。これを axis とする。
			Vector3 axis = Vector3::Transform(Vector3::UnitX, playerRot);

			// 得られた右手方向を軸として、-rotVelocity (ラジアン単位) 分、
			// 現在の回転姿勢をさらに回転させる。
			playerRot.RotateAxis(axis, -rotVelocity);
		}
		// ↑ キー
		if (CheckHitKey(KEY_INPUT_UP) == 1)
		{
			Vector3 axis = Vector3::Transform(Vector3::UnitX, playerRot);
			playerRot.RotateAxis(axis, rotVelocity);
		}
		// ← キー
		if (CheckHitKey(KEY_INPUT_LEFT) == 1)
		{
			// ロールは自機が原点にあるとすると、Z軸周りの回転となる。
			// Z軸を表すベクトルは (0, 0, 1) で、Vector3::UnitZ 定数がこれを表す。
			// 後はピッチの時と同じ。
			Vector3 axis = Vector3::Transform(Vector3::UnitZ, playerRot);
			playerRot.RotateAxis(axis, rotVelocity);
		}
		// → キー
		if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
		{
			Vector3 axis = Vector3::Transform(Vector3::UnitZ, playerRot);
			playerRot.RotateAxis(axis, -rotVelocity);
		}

		// 自機を前進させる。
		// まずロールと同じ方法で正面方向を求める。
		// これは単位ベクトルで、速度を掛けることで自機の位置をどれだけ進めればよいかがわかる。
		Vector3 front = Vector3::Transform(Vector3::UnitZ, playerRot);
		playerPos += front * velocity;


		//-----------------------------------------------------
		// カメラの姿勢更新
		
		// カメラの位置は自機から 5.0 だけ後ろに離れたところにしたい。
		// 自機のロールと同じ方法で、背面 -5.0 の位置を求めている。
		Vector3 back = Vector3::Transform(Vector3(0, 0, -5.0f), playerRot);
		Vector3 cameraPos = playerPos + back;

		// 上方向もこれまでと同じく座標変換で求める。
		// 元の方向は (0, 1, 0) で、Vector3::UnitY 定数がこれを表す。
		Vector3 cameraUp = Vector3::Transform(Vector3::UnitY, playerRot);

		// 必要な情報が揃った。カメラの姿勢を設定する
		SetCameraPositionAndTargetAndUpVec(cameraPos, playerPos, cameraUp);
		

		//-----------------------------------------------------
		// 描画処理

		ClearDrawScreen();

		// 背景球は、地形がめり込んで見えないように ZWrite を無効にして描画する
		MV1SetPosition(skyModel, playerPos);
		MV1SetWriteZBuffer(skyModel, FALSE);
		MV1DrawModel(skyModel);

		// 地面の描画 (半透明)
		MV1SetWireFrameDrawFlag(groundModel, FALSE);
		MV1SetOpacityRate(groundModel, 0.2f);
		MV1DrawModel(groundModel);

		// 地面の描画 (ワイヤーフレーム)
		MV1SetWireFrameDrawFlag(groundModel, TRUE);
		MV1SetOpacityRate(groundModel, 1.0f);
		MV1DrawModel(groundModel);

		// クォータニオンと位置ベクトルからワールド座標変換行列を求め、自機の姿勢として設定する
		Matrix mat = Matrix::AffineTransformation(
			Vector3::One,	// 拡大率は (1, 1, 1)
			Vector3::Zero,	// 回転の原点はモデルの原点
			playerRot,		// 回転情報
			playerPos);		// 位置
		MV1SetMatrix(playerModel, mat);
		MV1DrawModel(playerModel);

		// [おまけ] 光源の方向を向いたときに画面の輝度を上げる。
		// 光源の向きと自機の向きで内積を取ると、互いに完全に向き合うときは -1.0 を、垂直であれば 0.0 が返る。
		// ここでは -0.5 ～ -1.0 の間で徐々に明るくしている。
		Vector3 ligntDir(1, -1, -1);
		ligntDir.Normalize();
		float dot = Vector3::Dot(ligntDir, mat.GetFront());
		if (dot < -0.5f)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255 * (-(dot + 0.5f))));
			DrawBox(0, 0, 640, 480, GetColor(255, 255, 255), TRUE);
		}

		// info
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		DrawString(0, 0, "← → キーでロール", GetColor(100, 100, 100));
		DrawString(0, 16, "↑ ↓ キーでピッチ", GetColor(100, 100, 100));

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}