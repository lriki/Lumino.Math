#include "TestConfig.h"

class Test_Matrix : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Matrix, Basic)
{
	// Matrix::Matrix(const SQTTransform&)
	{
		SQTTransform t;
		t.scale.Set(1, 2, 3);
		t.rotation = Quaternion::MakeFromYawPitchRoll(0.5, 0.75, 1.0);
		t.translation.Set(5, 6, 7);

		Matrix m(t);
		ASSERT_MAT_NEAR(
			0.749148, 0.615695, 0.244328, 0.000000,
			-1.123784, 0.790666, 1.453257, 0.000000,
			1.052371, -2.044916, 1.926352, 0.000000,
			5.000000, 6.000000, 7.000000, 1.000000,
			m);
		//(Matrix::Scaling(1, 2, 3) * Matrix::RotationYawPitchRoll(0.5, 0.75, 1.0) * Matrix::Translation(5, 6, 7)).Print();
	}
	// this->Set
	{
		Matrix m;
		m.Set(
			0, 1, 2, 3,
			4, 5, 6, 7,
			8, 9, 10, 11,
			12, 13, 14, 15);
		ASSERT_VEC4_NEAR(0, 1, 2, 3, m.GetRow(0));
		ASSERT_VEC4_NEAR(4, 5, 6, 7, m.GetRow(1));
		ASSERT_VEC4_NEAR(8, 9, 10, 11, m.GetRow(2));
		ASSERT_VEC4_NEAR(12, 13, 14, 15, m.GetRow(3));
	}
	// this->IsIdentity
	{
		Matrix m;
		ASSERT_EQ(true, m.IsIdentity());

		Matrix m2 = Matrix::Identity;
		ASSERT_EQ(true, m2.IsIdentity());
	}
	// this->GetRight/GetUp/GetFront/GetPosition
	{
		Matrix m;
		m.m[3][0] = 5;
		ASSERT_VEC3_NEAR(1, 0, 0, m.GetRight());
		ASSERT_VEC3_NEAR(0, 1, 0, m.GetUp());
		ASSERT_VEC3_NEAR(0, 0, 1, m.GetFront());
		ASSERT_VEC3_NEAR(5, 0, 0, m.GetPosition());
	}
	// this->SetRow/GetRow
	{
		Matrix m;
		m.SetRow(0, Vector4(0, 1, 2, 3));
		m.SetRow(1, Vector4(4, 5, 6, 7));
		m.SetRow(2, Vector4(8, 9, 10, 11));
		m.SetRow(3, Vector4(12, 13, 14, 15));
		ASSERT_VEC4_NEAR(0, 1, 2, 3, m.GetRow(0));
		ASSERT_VEC4_NEAR(4, 5, 6, 7, m.GetRow(1));
		ASSERT_VEC4_NEAR(8, 9, 10, 11, m.GetRow(2));
		ASSERT_VEC4_NEAR(12, 13, 14, 15, m.GetRow(3));
	}
	// this->Translate
	{
		Matrix m;
		m.Translate(Vector3(1, 2, 3));
		m.Translate(1, 2, 3);
		ASSERT_MAT_NEAR(
			1.000000, 0.000000, 0.000000, 0.000000,
			0.000000, 1.000000, 0.000000, 0.000000,
			0.000000, 0.000000, 1.000000, 0.000000,
			2.000000, 4.000000, 6.000000, 1.000000,
			m);
	}
	// this->RotateX
	{
		Matrix m;
		m.Translate(Vector3(1, 2, 3));
		m.RotateX(2);
		ASSERT_MAT_NEAR(
			1.000000, -0.000000, 0.000000, 0.000000,
			0.000000, -0.416147, 0.909297, 0.000000,
			0.000000, -0.909297, -0.416147, 0.000000,
			1.000000, -3.560186, 0.570154, 1.000000,
			m);
	}
	// this->RotateY
	{
		Matrix m;
		m.Translate(Vector3(1, 2, 3));
		m.RotateY(2);
		ASSERT_MAT_NEAR(
			-0.416147, 0.000000, -0.909297, 0.000000,
			0.000000, 1.000000, -0.000000, 0.000000,
			0.909297, 0.000000, -0.416147, 0.000000,
			2.311745, 2.000000, -2.157738, 1.000000,
			m);
	}
	// this->RotateZ
	{
		Matrix m;
		m.Translate(Vector3(1, 2, 3));
		m.RotateZ(2);
		ASSERT_MAT_NEAR(
			-0.416147, 0.909297, 0.000000, 0.000000,
			-0.909297, -0.416147, 0.000000, 0.000000,
			-0.000000, 0.000000, 1.000000, 0.000000,
			-2.234742, 0.077004, 3.000000, 1.000000,
			m);
	}
	// Matrix::RotationEulerAngles
	{
		Matrix m;
		m.RotateEulerAngles(Vector3(0.5, 1.0, 1.5), RotationOrder::XYZ);
		ASSERT_MAT_NEAR(
			0.038219, 0.538949, -0.841471, 0.000000,
			-0.846847, 0.464490, 0.259035, 0.000000,
			0.530461, 0.702697, 0.474160, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);

		//m = Matrix::RotationEulerAngles(Vector3(0.5, 1.0, 1.5), RotationOrder_XZY);
		//ASSERT_MAT_NEAR(
		//	0.038219, 0.997495, -0.059523, 0.000000,
		//	-0.069549, 0.062078, 0.995645, 0.000000,
		//	0.996846, -0.033913, 0.071748, 0.000000,
		//	0.000000, 0.000000, 0.000000, 1.000000,
		//	m);
		
		m = Matrix::Identity;
		m.RotateEulerAngles(Vector3(0.5, 1.0, 1.5), RotationOrder::YZX);
		ASSERT_MAT_NEAR(
			0.038219, 0.876395, -0.480074, 0.000000,
			-0.997495, 0.062078, 0.033913, 0.000000,
			0.059523, 0.477576, 0.876572, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);

		m = Matrix::Identity;
		m.RotateEulerAngles(Vector3(0.5, 1.0, 1.5), RotationOrder::ZXY);
		ASSERT_MAT_NEAR(
			0.440632, 0.875384, 0.198863, 0.000000,
			-0.510412, 0.062078, 0.857686, 0.000000,
			0.738460, -0.479426, 0.474160, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);

		// デフォルト引数の順序は YawPitchRoll と一致する
		m = Matrix::Identity;
		m.RotateEulerAngles(Vector3(0.5, 1.0, 1.5));
		ASSERT_MAT_NEAR(
			0.440632, 0.875384, 0.198863, 0.000000,
			-0.510412, 0.062078, 0.857686, 0.000000,
			0.738460, -0.479426, 0.474160, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);

		m = Matrix::Identity;
		m.RotateEulerAngles(0.5, 1.0, 1.5);
		ASSERT_MAT_NEAR(
			0.440632, 0.875384, 0.198863, 0.000000,
			-0.510412, 0.062078, 0.857686, 0.000000,
			0.738460, -0.479426, 0.474160, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);
	}
	// this->RotateAxis
	{
		Matrix m1 = Matrix::MakeTranslation(Vector3(5, 6, 7));
		Matrix m2 = Matrix::MakeRotationYawPitchRoll(1, 2, 3);
		Matrix m3 = Matrix::Multiply(m1, m2);
		m3.RotateAxis(Vector3(1, 2, 3), 2);
		ASSERT_MAT_NEAR(
			0.877841, -0.068563, 0.474019, 0.000000,
			-0.244635, -0.915048, 0.320689, 0.000000,
			0.411763, -0.397476, -0.820039, 0.000000,
			5.803734, -8.615429, -1.446045, 1.000000,
			m3);
	}
	// this->RotateQuaternion
	{
		Matrix m1 = Matrix::MakeTranslation(Vector3(5, 6, 7));
		Matrix m2 = Matrix::MakeRotationYawPitchRoll(1, 2, 3);
		Matrix m3 = Matrix::Multiply(m1, m2);
		Quaternion q = Quaternion::MakeFromYawPitchRoll(0.5, 0.75, 1.0);
		m3.RotateQuaternion(q);
		ASSERT_MAT_NEAR(
			0.029720, -0.901166, 0.432454, 0.000000,
			-0.985044, -0.099867, -0.140410, 0.000000,
			0.169720, -0.421813, -0.890656, 0.000000,
			-4.573622, -8.057723, -4.914780, 1.000000,
			m3);
	}
	// this->Scale
	{
		Matrix m = Matrix::MakeRotationYawPitchRoll(0.75f, 0.5f, 1.0f);
		m.Scale(1, 2, 3);
		m.Scale(Vector3(3, 2, 1));
		m.Scale(2);
		ASSERT_MAT_NEAR(
			4.021930, 5.907682, -0.438667, 0.000000,
			-2.634761, 3.793279, 4.578672, 0.000000,
			3.589166, -3.835404, 3.852704, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);
	}
	// this->Inverse
	{
		Matrix m;
		m = Matrix::Multiply(m, Matrix::MakeTranslation(Vector3(5, 6, 7)));
		m = Matrix::Multiply(m, Matrix::MakeRotationYawPitchRoll(1, 2, 3));
		m.Inverse();
		ASSERT_MAT_NEAR(
			-0.426918, -0.833738, -0.350176, 0.000000,
			-0.058727, 0.411982, -0.909297, 0.000000,
			0.902382, -0.367630, -0.224845, -0.000000,
			-5.000000, -5.999999, -6.999999, 1.000000,
			m);
	}
	// this->Transpose
	{
		Matrix m;
		m = Matrix::Multiply(m, Matrix::MakeTranslation(Vector3(5, 6, 7)));
		m = Matrix::Multiply(m, Matrix::MakeRotationYawPitchRoll(1, 2, 3));
		m.Transpose();
		ASSERT_MAT_NEAR(
			-0.426918, -0.833738, -0.350176, -9.588243,
			-0.058727, 0.411982, -0.909297, -4.186821,
			0.902382, -0.367630, -0.224845, 0.732209,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);
	}
	// this->Decompose
	{
		Matrix mS = Matrix::MakeScaling(2, 3, 4);
		Matrix mT = Matrix::MakeTranslation(Vector3(5, 6, 7));
		Matrix mR = Matrix::MakeRotationYawPitchRoll(0.5, 0.75, 1.0);
		Matrix m = Matrix::Multiply(mS, mR);
		m = Matrix::Multiply(m, mT);

		Vector3 s;
		Quaternion q;
		Vector3 t;
		m.Decompose(&s, &q, &t);
		ASSERT_VEC3_NEAR(2.000000, 3.000000, 4.000000, s);
		ASSERT_QUA_NEAR(0.421811, 0.031888, 0.352717, 0.834655, q);
		ASSERT_VEC3_NEAR(5.000000, 6.000000, 7.000000, t);
	}
	// this->ToEulerAngles
	{
		// デフォルト引数ではこの回転順序
		Matrix m;
		m = Matrix::Multiply(m, Matrix::MakeRotationZ(1.0));
		m = Matrix::Multiply(m, Matrix::MakeRotationX(0.5));
		m = Matrix::Multiply(m, Matrix::MakeRotationY(0.75));
		Vector3 angles = m.ToEulerAngles();
		ASSERT_VEC3_NEAR(0.5, 0.75, 1.0, angles);

		// デフォルト引数で YawPitchRoll したものが取り出せる
		m = Matrix::MakeRotationYawPitchRoll(0.75f, 0.5f, 1.0f);
		angles = m.ToEulerAngles();
		ASSERT_VEC3_NEAR(0.5, 0.75, 1.0, angles);

		// XYZ
		m = Matrix::MakeRotationEulerAngles(Vector3(0.5, 0.75, 1.0), RotationOrder::XYZ);
		angles = m.ToEulerAngles(RotationOrder::XYZ);
		ASSERT_VEC3_NEAR(0.5, 0.75, 1.0, angles);

		// YZX
		m = Matrix::MakeRotationEulerAngles(Vector3(0.5, 0.75, 1.0), RotationOrder::YZX);
		angles = m.ToEulerAngles(RotationOrder::YZX);
		ASSERT_VEC3_NEAR(0.5, 0.75, 1.0, angles);

		// ZXY
		m = Matrix::MakeRotationEulerAngles(Vector3(0.5, 0.75, 1.0), RotationOrder::ZXY);
		angles = m.ToEulerAngles(RotationOrder::ZXY);
		ASSERT_VEC3_NEAR(0.5, 0.75, 1.0, angles);
	}
	// this->GetRotationMatrix
	{
		Matrix m1(
			Vector4(0, 1, 2, 3),
			Vector4(4, 5, 6, 7),
			Vector4(8, 9, 10, 11),
			Vector4(12, 13, 14, 15));
		Matrix m2 = m1.GetRotationMatrix();
		ASSERT_VEC4_NEAR(0, 1, 2, 0, m2.GetRow(0));
		ASSERT_VEC4_NEAR(4, 5, 6, 0, m2.GetRow(1));
		ASSERT_VEC4_NEAR(8, 9, 10, 0, m2.GetRow(2));
		ASSERT_VEC4_NEAR(0, 0, 0, 1, m2.GetRow(3));
	}
	// IsNaNOrInf
	{
		Matrix m = Matrix::MakeTranslation(5, 6, 7) * Matrix::MakeRotationYawPitchRoll(1, 2, 3);
		ASSERT_FALSE(m.IsNaNOrInf());
		volatile  float d = 0.0f;
		m.m[3][2] /= d;
		ASSERT_TRUE(m.IsNaNOrInf());
	}

	// Matrix::Multiply
	{
		Matrix mT = Matrix::MakeTranslation(Vector3(5, 6, 7));
		Matrix mR = Matrix::MakeRotationYawPitchRoll(1, 2, 3);
		Matrix m = Matrix::Multiply(mT, mR);
		ASSERT_MAT_NEAR(
			-0.426918, -0.058727, 0.902382, 0.000000,
			-0.833738, 0.411982, -0.367630, 0.000000,
			-0.350176, -0.909297, -0.224845, 0.000000,
			-9.588243, -4.186821, 0.732209, 1.000000,
			m);
	}
	// Matrix::MakeTranslation
	{
		Matrix m = Matrix::MakeTranslation(Vector3(5, 6, 7));
		ASSERT_MAT_NEAR(
			1.000000, 0.000000, 0.000000, 0.000000,
			0.000000, 1.000000, 0.000000, 0.000000,
			0.000000, 0.000000, 1.000000, 0.000000,
			5.000000, 6.000000, 7.000000, 1.000000,
			m);
	}
	// Matrix::MakeRotationX
	{
		Matrix m = Matrix::MakeRotationX(2);
		ASSERT_MAT_NEAR(
			1.000000, 0.000000, 0.000000, 0.000000,
			0.000000, -0.416147, 0.909297, 0.000000,
			0.000000, -0.909297, -0.416147, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);
	}
	// Matrix::MakeRotationY
	{
		Matrix m = Matrix::MakeRotationY(2);
		ASSERT_MAT_NEAR(
			-0.416147, 0.000000, -0.909297, 0.000000,
			0.000000, 1.000000, 0.000000, 0.000000,
			0.909297, 0.000000, -0.416147, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);
	}
	// Matrix::MakeRotationZ
	{
		Matrix m = Matrix::MakeRotationZ(2);
		ASSERT_MAT_NEAR(
			-0.416147, 0.909297, 0.000000, 0.000000,
			-0.909297, -0.416147, 0.000000, 0.000000,
			0.000000, 0.000000, 1.000000, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);
	}
	// Matrix::MakeRotationAxis
	{
		Matrix m = Matrix::MakeRotationAxis(Vector3(1, 2, 3), 2);
		ASSERT_MAT_NEAR(
			-0.314994, 0.931367, -0.182580, 0.000000,
			-0.526753, -0.011533, 0.849940, 0.000000,
			0.789500, 0.363900, 0.494233, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);
	}
	// Matrix::MakeRotationQuaternion
	{
		Quaternion q = Quaternion::MakeFromYawPitchRoll(1, 2, 3);
		Matrix m = Matrix::MakeRotationQuaternion(q);
		ASSERT_MAT_NEAR(
			-0.426918, -0.058727, 0.902382, 0.000000,
			-0.833738, 0.411982, -0.367630, 0.000000,
			-0.350176, -0.909297, -0.224845, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);
	}
	// Matrix::MakeRotationEulerAngles
	{
		Matrix m;
		m = Matrix::MakeRotationEulerAngles(Vector3(0.5, 1.0, 1.5), RotationOrder::XYZ);
		ASSERT_MAT_NEAR(
			0.038219, 0.538949, -0.841471, 0.000000,
			-0.846847, 0.464490, 0.259035, 0.000000,
			0.530461, 0.702697, 0.474160, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);

		//m = Matrix::RotationEulerAngles(Vector3(0.5, 1.0, 1.5), RotationOrder::XZY);
		//ASSERT_MAT_NEAR(
		//	0.038219, 0.997495, -0.059523, 0.000000,
		//	-0.069549, 0.062078, 0.995645, 0.000000,
		//	0.996846, -0.033913, 0.071748, 0.000000,
		//	0.000000, 0.000000, 0.000000, 1.000000,
		//	m);

		m = Matrix::MakeRotationEulerAngles(Vector3(0.5, 1.0, 1.5), RotationOrder::YZX);
		ASSERT_MAT_NEAR(
			0.038219, 0.876395, -0.480074, 0.000000,
			-0.997495, 0.062078, 0.033913, 0.000000,
			0.059523, 0.477576, 0.876572, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);

		m = Matrix::MakeRotationEulerAngles(Vector3(0.5, 1.0, 1.5), RotationOrder::ZXY);
		ASSERT_MAT_NEAR(
			0.440632, 0.875384, 0.198863, 0.000000,
			-0.510412, 0.062078, 0.857686, 0.000000,
			0.738460, -0.479426, 0.474160, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);

		// デフォルト引数の順序は YawPitchRoll と一致する
		m = Matrix::MakeRotationEulerAngles(Vector3(0.5, 1.0, 1.5));
		ASSERT_MAT_NEAR(
			0.440632, 0.875384, 0.198863, 0.000000,
			-0.510412, 0.062078, 0.857686, 0.000000,
			0.738460, -0.479426, 0.474160, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);
		m = Matrix::MakeRotationYawPitchRoll(1.0, 0.5, 1.5);
		ASSERT_MAT_NEAR(
			0.440632, 0.875384, 0.198863, 0.000000,
			-0.510412, 0.062078, 0.857686, 0.000000,
			0.738460, -0.479426, 0.474160, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);

		m = Matrix::MakeRotationEulerAngles(0.5, 1.0, 1.5);
		ASSERT_MAT_NEAR(
			0.440632, 0.875384, 0.198863, 0.000000,
			-0.510412, 0.062078, 0.857686, 0.000000,
			0.738460, -0.479426, 0.474160, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);
	}
	// Matrix::MakeRotationYawPitchRoll
	{
		Matrix m = Matrix::MakeRotationYawPitchRoll(0.75f, 0.5f, 1.0f);
		ASSERT_MAT_NEAR(
			0.670322, 0.738460, -0.073111, 0.000000,
			-0.439127, 0.474160, 0.763112, 0.000000,
			0.598194, -0.479426, 0.642117, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);
	}
	// Matrix::MakeScaling
	{
		Matrix m1 = Matrix::MakeScaling(1, 2, 3);
		Matrix m2 = Matrix::MakeScaling(Vector3(4, 5, 6));
		Matrix m3 = Matrix::MakeScaling(2);
		ASSERT_MAT_NEAR(
			1.000000, 0.000000, 0.000000, 0.000000,
			0.000000, 2.000000, 0.000000, 0.000000,
			0.000000, 0.000000, 3.000000, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m1);
		ASSERT_MAT_NEAR(
			4.000000, 0.000000, 0.000000, 0.000000,
			0.000000, 5.000000, 0.000000, 0.000000,
			0.000000, 0.000000, 6.000000, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m2);
		ASSERT_MAT_NEAR(
			2.000000, 0.000000, 0.000000, 0.000000,
			0.000000, 2.000000, 0.000000, 0.000000,
			0.000000, 0.000000, 2.000000, 0.000000,
			0.000000, 0.000000, 0.000000, 1.000000,
			m3);
	}
	// Matrix::MakeInverse
	{
		Matrix m;
		m = Matrix::Multiply(m, Matrix::MakeTranslation(Vector3(5, 6, 7)));
		m = Matrix::Multiply(m, Matrix::MakeRotationYawPitchRoll(1, 2, 3));
		m = Matrix::MakeInverse(m);
		ASSERT_MAT_NEAR(
			-0.426918, -0.833738, -0.350176, 0.000000,
			-0.058727, 0.411982, -0.909297, 0.000000,
			0.902382, -0.367630, -0.224845, -0.000000,
			-5.000000, -5.999999, -6.999999, 1.000000,
			m);
	}
	// Matrix::MakeTranspose
	{
		Matrix m;
		m = Matrix::Multiply(m, Matrix::MakeTranslation(Vector3(5, 6, 7)));
		m = Matrix::Multiply(m, Matrix::MakeRotationYawPitchRoll(1, 2, 3));
		m = Matrix::MakeTranspose(m);
		ASSERT_MAT_NEAR(
			-0.426918, -0.833738, -0.350176, -9.588243,
			-0.058727, 0.411982, -0.909297, -4.186821,
			0.902382, -0.367630, -0.224845, 0.732209,
			0.000000, 0.000000, 0.000000, 1.000000,
			m);
	}
	// Matrix::MakeReflection
	{
		Vector3 v1(1, 2, 3);
		Vector3 v2(-4, -5, -6);
		Vector3 v3(0.1f, 0.2f, 0.3f);
		Plane pl(v1, v2, v3);
		Matrix m = Matrix::MakeReflection(pl);
		ASSERT_MAT_NEAR(
			0.666667, 0.666667, -0.333333, 0.000000,
			0.666667, -0.333333, 0.666667, 0.000000,
			-0.333333, 0.666667, 0.666667, 0.000000,
			0.000000, -0.000000, 0.000000, 1.000000,
			m);
	}
	// Matrix::MakeLookAtLH / Matrix::MakeLookAtRH
	{
		Vector3 pos(-10, -20, -30);
		Vector3 at(40, 50, 60);
		Vector3 up(0, 1, 0);

		Matrix mat1 = Matrix::MakeLookAtLH(pos, at, up);
		ASSERT_MAT_NEAR(
			0.874157, -0.273054, 0.401610, 0.000000,
			0.000000, 0.826965, 0.562254, 0.000000,
			-0.485643, -0.491498, 0.722897, 0.000000,
			-5.827715, -0.936188, 36.948090, 1.000000,
			mat1);

		Matrix mat2 = Matrix::MakeLookAtRH(pos, at, up);
		ASSERT_MAT_NEAR(
			-0.874157, -0.273054, -0.401610, 0.000000,
			0.000000, 0.826965, -0.562254, 0.000000,
			0.485643, -0.491498, -0.722897, 0.000000,
			5.827715, -0.936188, -36.948090, 1.000000,
			mat2);
	}
	// Matrix::MakePerspectiveFovLH / Matrix::MakePerspectiveFovRH
	{
		Matrix mat1 = Matrix::MakePerspectiveFovLH(0.7f, 1.33f, 1.0, 1000.0);
		ASSERT_MAT_NEAR(
			2.059783, 0.000000, 0.000000, 0.000000,
			0.000000, 2.739512, 0.000000, 0.000000,
			0.000000, 0.000000, 1.001001, 1.000000,
			0.000000, 0.000000, -1.001001, 0.000000,
			mat1);

		Matrix mat2 = Matrix::MakePerspectiveFovRH(0.7f, 1.33f, 1.0, 1000.0);
		ASSERT_MAT_NEAR(
			2.059783, 0.000000, 0.000000, 0.000000,
			0.000000, 2.739512, 0.000000, 0.000000,
			0.000000, 0.000000, -1.001001, -1.000000,
			0.000000, 0.000000, -1.001001, 0.000000,
			mat2);
	}
	// Matrix::MakeOrthoLH / Matrix::MakeOrthoRH
	{
		Matrix mat1 = Matrix::MakeOrthoLH(640, 480, 10, 1000);
		ASSERT_MAT_NEAR(
			0.003125, 0.000000, 0.000000, 0.000000,
			0.000000, 0.004167, 0.000000, 0.000000,
			0.000000, 0.000000, 0.001010, 0.000000,
			0.000000, 0.000000, -0.010101, 1.000000,
			mat1);

		Matrix mat2 = Matrix::MakeOrthoRH(640, 480, 10, 1000);
		ASSERT_MAT_NEAR(
			0.003125, 0.000000, 0.000000, 0.000000,
			0.000000, 0.004167, 0.000000, 0.000000,
			0.000000, 0.000000, -0.001010, 0.000000,
			0.000000, 0.000000, -0.010101, 1.000000,
			mat2);
	}
	// Matrix::MakeAffineTransformation
	{
		Vector3 center(2, 3, 4);
		Quaternion q = Quaternion::MakeFromYawPitchRoll(0.5, 0.75, 1.0);
		Vector3 trans(5, 6, 7);
		Matrix m = Matrix::MakeAffineTransformation(Vector3(3.0f, 3.0f, 3.0f), center, q, trans);
		ASSERT_MAT_NEAR(
			2.247445, 1.847085, 0.732985, 0.000000,
			-1.685677, 1.185999, 2.179885, 0.000000,
			1.052371, -2.044916, 1.926352, 0.000000,
			5.784219, 9.309166, 5.762988, 1.000000,
			m);
	}
	// operator
	{
		Matrix m = Matrix::MakeTranslation(5, 6, 7);
		m *= Matrix::MakeRotationYawPitchRoll(1, 2, 3);
		ASSERT_MAT_NEAR(
			-0.426918, -0.058727, 0.902382, 0.000000,
			-0.833738, 0.411982, -0.367630, 0.000000,
			-0.350176, -0.909297, -0.224845, 0.000000,
			-9.588243, -4.186821, 0.732209, 1.000000,
			m);

		Matrix m2 = Matrix::MakeTranslation(5, 6, 7);
		m2 = m2 * Matrix::MakeRotationYawPitchRoll(1, 2, 3);
		ASSERT_MAT_NEAR(
			-0.426918, -0.058727, 0.902382, 0.000000,
			-0.833738, 0.411982, -0.367630, 0.000000,
			-0.350176, -0.909297, -0.224845, 0.000000,
			-9.588243, -4.186821, 0.732209, 1.000000,
			m2);

		ASSERT_TRUE(m2 == m);
		ASSERT_FALSE(m2 != m);
		m.m[0][1] = 5;
		ASSERT_FALSE(m2 == m);
		ASSERT_TRUE(m2 != m);
	}

#ifdef D3DX9_TEST
	{
		D3DXMATRIX dm1;
		D3DXMATRIX dm2;
		D3DXMatrixTranslation(&dm1, 1, 2, 3);
		D3DXMatrixTranslation(&dm2, 1, 2, 3);
		//dumpD3DXMATRIX("D3DXMatrixTranslation", dm1 * dm2);
	}
	{
		D3DXMATRIX dm1;
		D3DXMATRIX dm2;
		D3DXMatrixTranslation(&dm1, 1, 2, 3);
		D3DXMatrixRotationX(&dm2, 2);
		//dumpD3DXMATRIX("D3DXMatrixRotationX", dm1 * dm2);
	}
	{
		D3DXMATRIX dm1;
		D3DXMATRIX dm2;
		D3DXMatrixTranslation(&dm1, 1, 2, 3);
		D3DXMatrixRotationY(&dm2, 2);
		//dumpD3DXMATRIX("D3DXMatrixRotationY", dm1 * dm2);
	}
	{
		D3DXMATRIX dm1;
		D3DXMATRIX dm2;
		D3DXMatrixTranslation(&dm1, 1, 2, 3);
		D3DXMatrixRotationZ(&dm2, 2);
		//dumpD3DXMATRIX("D3DXMatrixRotationZ", dm1 * dm2);
	}
	{
		D3DXMATRIX dxRx;
		D3DXMatrixRotationX(&dxRx, 2);
		//dumpD3DXMATRIX("D3DXMatrixRotationX", dxRx);
		D3DXMATRIX dxRy;
		D3DXMatrixRotationY(&dxRy, 2);
		//dumpD3DXMATRIX("D3DXMatrixRotationY", dxRy);
		D3DXMATRIX dxRz;
		D3DXMatrixRotationZ(&dxRz, 2);
		//dumpD3DXMATRIX("D3DXMatrixRotationZ", dxRz);
	}
	{
		D3DXMATRIX dxRx;
		D3DXMatrixRotationX(&dxRx, 0.5);
		//dumpD3DXMATRIX("D3DXMatrixRotationX", dxRx);
		D3DXMATRIX dxRy;
		D3DXMatrixRotationY(&dxRy, 1.0);
		//dumpD3DXMATRIX("D3DXMatrixRotationY", dxRy);
		D3DXMATRIX dxRz;
		D3DXMatrixRotationZ(&dxRz, 1.5);
		//dumpD3DXMATRIX("D3DXMatrixRotationZ", dxRz);
		//dumpD3DXMATRIX("Rx*Ry*Rz", dxRx * dxRy * dxRz);
		//dumpD3DXMATRIX("Rx*Rz*Ry", dxRx * dxRz * dxRy);
		//dumpD3DXMATRIX("Ry*Rz*Rx", dxRy * dxRz * dxRx);
		//dumpD3DXMATRIX("Rz*Rx*Ry", dxRz * dxRx * dxRy);
	}
	{
		D3DXMATRIX dxR;
		D3DXVECTOR3 axis(1, 2, 3);
		D3DXMatrixRotationAxis(&dxR, &axis, 2);
		//dumpD3DXMATRIX("D3DXMatrixRotationAxis", dxR);
	}
	{
		D3DXMATRIX dxT;
		D3DXMatrixTranslation(&dxT, 5, 6, 7);
		D3DXMATRIX dxR;
		D3DXMatrixRotationYawPitchRoll(&dxR, 1, 2, 3);

		D3DXMATRIX dxR2;
		D3DXVECTOR3 axis(1, 2, 3);
		D3DXMatrixRotationAxis(&dxR2, &axis, 2);

		//dumpD3DXMATRIX("D3DXMatrixRotationAxis*", dxT * dxR * dxR2);
	}
	{
		// 適当に移動と回転の後、クォータニオン→行列を乗算
		D3DXMATRIX dxT;
		D3DXMatrixTranslation(&dxT, 5, 6, 7);
		D3DXMATRIX dxR;
		D3DXMatrixRotationYawPitchRoll(&dxR, 1, 2, 3);

		D3DXQUATERNION dxq;
		D3DXQuaternionRotationYawPitchRoll(&dxq, 0.5, 0.75, 1.0);
		D3DXMATRIX dxRQ;
		D3DXMatrixRotationQuaternion(&dxRQ, &dxq);
		//dumpD3DXMATRIX("D3DXMatrixRotationQuaternion*", dxT * dxR * dxRQ);
	}
	{
		D3DXQUATERNION dxq;
		D3DXQuaternionRotationYawPitchRoll(&dxq, 1, 2, 3);
		D3DXMATRIX dxm;
		D3DXMatrixRotationQuaternion(&dxm, &dxq);
		//dumpD3DXMATRIX("D3DXMatrixRotationQuaternion", dxm);
	}
	{
		D3DXMATRIX dxT;
		D3DXMatrixTranslation(&dxT, 5, 6, 7);
		D3DXMATRIX dxR;
		D3DXMatrixRotationYawPitchRoll(&dxR, 1, 2, 3);
		D3DXMATRIX dxm;
		D3DXMatrixMultiply(&dxm, &dxT, &dxR);
		//dumpD3DXMATRIX("D3DXMatrixMultiply", dxm);
	}

	
	{
		// D3DXMatrixRotationYawPitchRoll の回転順序は Z > X > Y
		D3DXMATRIX dxRx;
		D3DXMatrixRotationX(&dxRx, 0.5f);
		D3DXMATRIX dxRy;
		D3DXMatrixRotationY(&dxRy, 0.75f);
		D3DXMATRIX dxRz;
		D3DXMatrixRotationZ(&dxRz, 1.0f);
		D3DXMATRIX dxR = dxRz * dxRx * dxRy;
		//dumpD3DXMATRIX("Z*X*Y", dxR);

		D3DXMATRIX  dxm;
		D3DXMatrixRotationYawPitchRoll(&dxm, 0.75f, 0.5f, 1.0f);
		//dumpD3DXMATRIX("D3DXMatrixRotationYawPitchRoll", dxm);

		Vector3 a = ((Matrix*)&dxm)->ToEulerAngles();
		ASSERT_VEC3_NEAR(0.5, 0.75, 1.0, a);
		//a.Print();
	}
	{
		D3DXMATRIX  dxm;
		D3DXMatrixRotationYawPitchRoll(&dxm, 0.75f, 0.5f, 1.0f);
		D3DXMATRIX dm1;
		D3DXMATRIX dm2;
		D3DXMATRIX dm3;
		D3DXMatrixScaling(&dm1, 1, 2, 3);
		D3DXMatrixScaling(&dm2, 3, 2, 1);
		D3DXMatrixScaling(&dm3, 2, 2, 2);
		//dumpD3DXMATRIX("D3DXMatrixScaling", dxm * dm1 * dm2 * dm3);
	}

	{
		D3DXMATRIX dxT;
		D3DXMatrixTranslation(&dxT, 5, 6, 7);
		D3DXMATRIX dxR;
		D3DXMatrixRotationYawPitchRoll(&dxR, 1, 2, 3);
		D3DXMATRIX dxm;
		D3DXMatrixMultiply(&dxm, &dxT, &dxR);
		D3DXMATRIX dxm2;
		D3DXMatrixInverse(&dxm2, NULL, &dxm);
		//dumpD3DXMATRIX("D3DXMatrixInverse", dxm2);
	}
	{
		D3DXMATRIX dxT;
		D3DXMatrixTranslation(&dxT, 5, 6, 7);
		D3DXMATRIX dxR;
		D3DXMatrixRotationYawPitchRoll(&dxR, 1, 2, 3);
		D3DXMATRIX dxm;
		D3DXMatrixMultiply(&dxm, &dxT, &dxR);
		D3DXMATRIX dxm2;
		D3DXMatrixTranspose(&dxm2, &dxm);
		//dumpD3DXMATRIX("D3DXMatrixTranspose", dxm2);
	}
	// D3DXMatrixReflect
	{
		D3DXVECTOR3 v1(1, 2, 3);
		D3DXVECTOR3 v2(-4, -5, -6);
		D3DXVECTOR3 v3(0.1f, 0.2f, 0.3f);
		D3DXPLANE pl;
		D3DXPlaneFromPoints(&pl, &v1, &v2, &v3);
		D3DXMATRIX dxm;
		D3DXMatrixReflect(&dxm, &pl);
		//dumpD3DXMATRIX("D3DXMatrixReflect", dxm);
	}
	// D3DXMatrixDecompose
	{
		D3DXMATRIX dxS;
		D3DXMatrixScaling(&dxS, 2, 3, 4);
		D3DXMATRIX dxT;
		D3DXMatrixTranslation(&dxT, 5, 6, 7);
		D3DXMATRIX dxR;
		D3DXMatrixRotationYawPitchRoll(&dxR, 0.5, 0.75, 1.0);
		D3DXMATRIX dxm = dxS * dxR * dxT;

		D3DXVECTOR3 dxvS;
		D3DXQUATERNION dxq;
		D3DXVECTOR3 dxvT;
		D3DXMatrixDecompose(&dxvS, &dxq, &dxvT, &dxm);

		//dumpD3DXVECTOR3("S", dxvS);
		//dumpD3DXQUATERNION("Q", dxq);
		//dumpD3DXVECTOR3("T", dxvT);
	}
	// D3DXMatrixLookAtLH / D3DXMatrixLookAtRH
	{
		D3DXVECTOR3 pos(-10, -20, -30);
		D3DXVECTOR3 at(40, 50, 60);
		D3DXVECTOR3 up(0, 1, 0);

		D3DXMATRIX mat1;
		D3DXMatrixLookAtLH(&mat1, &pos, &at, &up);
		//dumpD3DXMATRIX("D3DXMatrixLookAtLH", mat1);

		D3DXMATRIX mat2;
		D3DXMatrixLookAtRH(&mat2, &pos, &at, &up);
		//dumpD3DXMATRIX("D3DXMatrixLookAtRH", mat2);
	}
	// D3DXMatrixPerspectiveFovLH / D3DXMatrixPerspectiveFovRH
	{
		D3DXMATRIX mat1;
		D3DXMatrixPerspectiveFovLH(&mat1, 0.7f, 1.33f, 1.0, 1000.0);
		//dumpD3DXMATRIX("D3DXMatrixPerspectiveFovLH", mat1);

		D3DXMATRIX mat2;
		D3DXMatrixPerspectiveFovRH(&mat2, 0.7f, 1.33f, 1.0, 1000.0);
		//dumpD3DXMATRIX("D3DXMatrixPerspectiveFovRH", mat2);
	}
	// D3DXMatrixOrthoLH / D3DXMatrixOrthoRH
	{
		D3DXMATRIX mat1;
		D3DXMatrixOrthoLH(&mat1, 640, 480, 10, 1000);
		//dumpD3DXMATRIX("D3DXMatrixOrthoLH", mat1);

		D3DXMATRIX mat2;
		D3DXMatrixOrthoRH(&mat2, 640, 480, 10, 1000);
		//dumpD3DXMATRIX("D3DXMatrixOrthoRH", mat2);
	}
	// D3DXMatrixAffineTransformation
	{
		D3DXVECTOR3 center(2, 3, 4);
		//D3DXVECTOR3 center(0,0,0);
		D3DXQUATERNION q;
		D3DXQuaternionRotationYawPitchRoll(&q, 0.5, 0.75, 1.0);
		D3DXVECTOR3 trans(5, 6, 7);
		//D3DXVECTOR3 trans(0, 0, 0);
		D3DXMATRIX m;
		D3DXMatrixAffineTransformation(&m, 3.0f, &center, &q, &trans);
		//dumpD3DXMATRIX("D3DXMatrixAffineTransformation", m);
	}

	/*
	
	×D3DXMatrixDeterminant
	D3DXMatrixIdentity
	D3DXMatrixIsIdentity
	D3DXMatrixMultiply
	×D3DXMatrixMultiplyTranspose
	
	D3DXMatrixRotationX
	D3DXMatrixRotationY
	D3DXMatrixRotationZ
	D3DXMatrixRotationAxis
	D3DXMatrixRotationQuaternion
	D3DXMatrixRotationYawPitchRoll
	D3DXMatrixScaling


	D3DXMatrixInverse
	D3DXMatrixReflect
	×D3DXMatrixShadow

	D3DXMatrixLookAtLH
	D3DXMatrixLookAtRH
	D3DXMatrixOrthoLH
	D3DXMatrixOrthoRH
	×D3DXMatrixOrthoOffCenterLH
	×D3DXMatrixOrthoOffCenterRH
	D3DXMatrixPerspectiveFovLH
	D3DXMatrixPerspectiveFovRH
	×D3DXMatrixPerspectiveLH
	×D3DXMatrixPerspectiveRH
	×D3DXMatrixPerspectiveOffCenterLH
	×D3DXMatrixPerspectiveOffCenterRH

	D3DXMatrixAffineTransformation
	×D3DXMatrixTransformation
	D3DXMatrixTranslation
	D3DXMatrixTranspose
	*/
#endif
}
