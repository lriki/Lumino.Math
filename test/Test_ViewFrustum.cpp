#include "TestConfig.h"

class Test_ViewFrustum : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_ViewFrustum, Basic)
{
	// コンストラクタ
	{
		ViewFrustum v1(Matrix::Identity);

		Matrix m2 = Matrix::LookAtLH(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0)) * Matrix::PerspectiveFovLH(Math::PI / 4, 640.0f / 480.0f, 1, 1000);
		ViewFrustum v2(m2);
		ASSERT_PLANE_NEAR(-0.000000, -0.000000, -1.000000, 1.000000, v2.GetPlane(FrustumPlane_Near));
		ASSERT_PLANE_NEAR(0.000000, 0.000000, 1.000000, -1000.000610, v2.GetPlane(FrustumPlane_Far));
		ASSERT_PLANE_NEAR(-0.875370, -0.000000, -0.483454, -0.000000, v2.GetPlane(FrustumPlane_Left));
		ASSERT_PLANE_NEAR(0.875370, 0.000000, -0.483454, 0.000000, v2.GetPlane(FrustumPlane_Right));
		ASSERT_PLANE_NEAR(0.000000, 0.923880, -0.382683, 0.000000, v2.GetPlane(FrustumPlane_Top));
		ASSERT_PLANE_NEAR(-0.000000, -0.923880, -0.382683, -0.000000, v2.GetPlane(FrustumPlane_Bottom));
		
		// Ortho
		ViewFrustum v3(Matrix::LookAtRH(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0)) * Matrix::OrthoRH(640, 480, 0, 1000));
		ASSERT_PLANE_NEAR(-0.000000, -0.000000, -1.000000, -0.000000, v3.GetPlane(FrustumPlane_Near));
		ASSERT_PLANE_NEAR(0.000000, 0.000000, 1.000000, -999.999939, v3.GetPlane(FrustumPlane_Far));
		ASSERT_PLANE_NEAR(1.000000, -0.000000, -0.000000, -320.000000, v3.GetPlane(FrustumPlane_Left));
		ASSERT_PLANE_NEAR(-1.000000, 0.000000, 0.000000, -320.000000, v3.GetPlane(FrustumPlane_Right));
		ASSERT_PLANE_NEAR(0.000000, 1.000000, 0.000000, -239.999985, v3.GetPlane(FrustumPlane_Top));
		ASSERT_PLANE_NEAR(-0.000000, -1.000000, -0.000000, -239.999985, v3.GetPlane(FrustumPlane_Bottom));
		//for (int i = 0; i < 6; i++)
		//	v3.GetPlane((FrustumPlane)i).Print();

		Vector3 points[8];
		v3.GetCornerPoints(points);
		ASSERT_VEC3_NEAR(320.000000, 239.999985, 0.000000, points[0]);
		ASSERT_VEC3_NEAR(-320.000000, 239.999985, 0.000000, points[1]);
		ASSERT_VEC3_NEAR(-320.000000, -239.999985, 0.000000, points[2]);
		ASSERT_VEC3_NEAR(320.000000, -239.999985, 0.000000, points[3]);
		ASSERT_VEC3_NEAR(320.000000, 239.999985, 1000.000000, points[4]);
		ASSERT_VEC3_NEAR(-320.000000, 239.999985, 1000.000000, points[5]);
		ASSERT_VEC3_NEAR(-320.000000, -239.999985, 1000.000000, points[6]);
		ASSERT_VEC3_NEAR(320.000000, -239.999985, 1000.000000, points[7]);
		//for (int i = 0; i < 8; i++)
		//	points[i].Print();
	}
}

TEST_F(Test_ViewFrustum, Intersects)
{
	Matrix proj1 = Matrix::PerspectiveFovLH(Math::PI / 4, 640.0f / 480.0f, 1, 1000);

	// Z+ 向き
	{
		Matrix view = Matrix::LookAtLH(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
		ViewFrustum vf1(view * proj1);

		Vector3 pos1(0, 0, 10);
		Vector3 pos2(10, 0, 0);
		ASSERT_TRUE(vf1.Intersects(pos1));
		ASSERT_FALSE(vf1.Intersects(pos2));

		Vector3 sppos1(0, 0, 1002);
		float spr1 = 1;
		ASSERT_FALSE(vf1.Intersects(sppos1, spr1));

		Vector3 sppos2(0, 0, 1000);
		float spr2 = 5;
		ASSERT_TRUE(vf1.Intersects(sppos2, spr2));	
	}
}
