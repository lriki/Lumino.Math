Lumino.Math
===========

##はじめに
Lumino.Math はゲーム等の C++ 言語による 3D グラフィックスアプリケーション開発のための、
マルチプラットフォーム数学ライブラリです。
3Dシーンを構築するための基本的なベクトル、行列、クォータニオン等の
線形代数、幾何学演算の機能を提供しています。


##ビルド方法

複数のプラットフォーム向けのビルドシステムとして、CMake を使用します。
CMake がインストールされていなければ、ご使用の OS 用の CMake をダウンロードしてインストールする必要があります。
- http://www.cmake.org/

ビルドに成功すると、"lib" フォルダにライブラリファイルがコピーされます。

###VisualStudio でビルド (Windows)
1. CMake GUI を起動します。
2. "Where is the source code:" に、このファイルがあるディレクトリパスを指定します。
3. エクスプローラで "build" フォルダを作成し、"Where to build the binaried:" にパスを指定します。
4. "Configure" ボタンをクリックして VisualStudio のバージョンを選択し、"Finish" をクリックします。
5. "Generate" ボタンをクリックします。
6. "build" フォルダ内に生成されたソリューションファイル(.sln) を開き、ビルドします。

###XCode でビルド (Mac)
$ cd <このファイルがあるディレクトリ>  
$ mkdir build  
$ cd build  
$ cmake .. -G "Xcode"  
$ open LuminoMath.xcodeproj  

###コマンドラインからのビルド (Windows/Mac/Linux)
$ cd <このファイルがあるディレクトリ>  
$ mkdir build  
$ cd build  
$ cmake ..  
$ make  


##今後のロードマップ
- SIMD 対応 … SIMD コンパイラ組み込み関数を使用した演算の高速化。
- 3Dジオメトリの衝突判定 … 平面や球、有向境界ボックス同士のシンプルな衝突判定。


##D3DXMath との比較表
D3DX の Matrix、Plane、Quaternion、Vector2/3/4 の算術関数との機能比較表です。

|D3DX Functions						|Lumino.Math	|
|-----------------------------------|---------------|
|D3DXMatrixAffineTransformation		|Matrix::AffineTransformation|
|D3DXMatrixAffineTransformation2D	||
|D3DXMatrixDecompose				|Matrix::Decompose|
|D3DXMatrixDeterminant				||
|D3DXMatrixInverse					|Matrix::Inverse|
|D3DXMatrixIsIdentity				|Matrix::IsIdentity|
|D3DXMatrixLookAtLH					|Matrix::LookAtLH|
|D3DXMatrixLookAtRH					|Matrix::LookAtRH|
|D3DXMatrixMultiply					|Matrix::Multiply|
|D3DXMatrixMultiplyTranspose		||
|D3DXMatrixOrthoLH					|Matrix::OrthoLH|
|D3DXMatrixOrthoRH					|Matrix::OrthoRH|
|D3DXMatrixOrthoOffCenterLH			||
|D3DXMatrixOrthoOffCenterRH			||
|D3DXMatrixPerspectiveFovLH			|Matrix::PerspectiveFovLH|
|D3DXMatrixPerspectiveFovRH			|Matrix::PerspectiveFovRH|
|D3DXMatrixPerspectiveLH			||
|D3DXMatrixPerspectiveRH			||
|D3DXMatrixReflect					|Matrix::Reflection|
|D3DXMatrixRotationAxis				|Matrix::RotationAxis|
|D3DXMatrixRotationQuaternion		|Matrix::RotationQuaternion|
|D3DXMatrixRotationX				|Matrix::RotationX|
|D3DXMatrixRotationY				|Matrix::RotationY|
|D3DXMatrixRotationZ				|Matrix::RotationZ|
|D3DXMatrixRotationYawPitchRoll		|Matrix::RotationYawPitchRoll|
|D3DXMatrixScaling					|Matrix::Scaling|
|D3DXMatrixShadow					||
|D3DXMatrixTransformation			|Matrix::Transformation|
|D3DXMatrixTransformation2D			||
|D3DXMatrixTranslation				|Matrix::Translation|
|D3DXMatrixTranspose				|Matrix::Transpose|
|									|Matrix::EulerAngles|
|									|Matrix::TransformBasis|
|									|Matrix::ContainsNaNOrInf|
|D3DXPlaneDot						|Plane::Dot|
|D3DXPlaneDotCoord					|Plane::DotCoord|
|D3DXPlaneDotNormal					|Plane::DotNormal|
|D3DXPlaneFromPointNormal			|Plane::Plane|
|D3DXPlaneFromPoints				|Plane::Plane|
|D3DXPlaneIntersectLine				|Plane::Intersect|
|D3DXPlaneNormalize					|Plane::Normalize|
|D3DXPlaneTransform					|Plane::Transform|
|D3DXPlaneTransformArray			||
|D3DXQuaternionBaryCentric			||
|D3DXQuaternionConjugate			|Quaternion::Conjugate|
|D3DXQuaternionDot					|Quaternion::Dot|
|D3DXQuaternionExp					||
|D3DXQuaternionIdentity				|Quaternion::Identity|
|D3DXQuaternionInverse				|Quaternion::Inverse|
|D3DXQuaternionIsIdentity			|Quaternion::IsIdentity|
|D3DXQuaternionLength				|Quaternion::GetLength|
|D3DXQuaternionLengthSq				|Quaternion::GetLengthSquared|
|D3DXQuaternionLn					||
|D3DXQuaternionMultiply				|Quaternion::Multiply|
|D3DXQuaternionNormalize			|Quaternion::Normalize|
|D3DXQuaternionRotationAxis			|Quaternion::RotationAxis|
|D3DXQuaternionRotationMatrix		|Quaternion::RotationMatrix|
|D3DXQuaternionRotationYawPitchRoll	|Quaternion::RotationYawPitchRoll|
|									|Quaternion::RotationEulerAngles|
|D3DXQuaternionSlerp				|Quaternion::Slerp|
|D3DXQuaternionSquad				||
|D3DXQuaternionSquadSetup			||
|D3DXQuaternionToAxisAngle			|Quaternion::ToAxisAngle|
|									|Quaternion::ToEulerAngles|
|D3DXVec2Add						|Vector2::operator+|
|D3DXVec2BaryCentric				||
|D3DXVec2CatmullRom					|Vector2::CatmullRom|
|D3DXVec2CCW						||
|D3DXVec2Dot						|Vector2::Dot|
|D3DXVec2Hermite					|Vector2::Hermite|
|D3DXVec2Length						|Vector2::GetLength|
|D3DXVec2LengthSq					|Vector2::GetLengthSquared|
|D3DXVec2Lerp						|Vector2::Lerp|
|D3DXVec2Maximize					|Vector2::Max|
|D3DXVec2Minimize					|Vector2::Min|
|D3DXVec2Normalize					|Vector2::Normalize|
|D3DXVec2Scale						|Vector2::operator*|
|D3DXVec2Subtract					|Vector2::operator-|
|D3DXVec2Transform					|Vector2::Transform|
|D3DXVec2TransformArray				||
|D3DXVec2TransformCoord				|Vector2::TransformCoord|
|D3DXVec2TransformCoordArray		||
|D3DXVec2TransformNormal			||
|D3DXVec2TransformNormalArray		||
|D3DXVec3Add						|Vector3::operator+|
|D3DXVec3BaryCentric				||
|D3DXVec3CatmullRom					|Vector3::CatmullRom|
|D3DXVec3Cross						|Vector3::Cross|
|D3DXVec3Dot						|Vector3::Dot|
|D3DXVec3Hermite					|Vector3::Hermite|
|D3DXVec3Length						|Vector3::GetLength|
|D3DXVec3LengthSq					|Vector3::GetLengthSquared|
|D3DXVec3Lerp						|Vector3::Lerp|
|D3DXVec3Maximize					|Vector3::Max|
|D3DXVec3Minimize					|Vector3::Min|
|D3DXVec3Normalize					|Vector3::Normalize|
|D3DXVec3Project					|Vector3::Project|
|D3DXVec3ProjectArray				||
|D3DXVec3Scale						|Vector3::operator*|
|D3DXVec3Subtract					|Vector3::operator-|
|D3DXVec3Transform					|Vector3::Transform|
|D3DXVec3TransformArray				||
|D3DXVec3TransformCoord				|Vector3::TransformCoord|
|D3DXVec3TransformCoordArray		||
|D3DXVec3TransformNormal			||
|D3DXVec3TransformNormalArray		||
|D3DXVec3Unproject					|Vector3::Unproject|
|D3DXVec3UnprojectArray				||
|									|Vector3::Reflect|
|									|Vector3::Slide|
|D3DXVec4Add						|Vector4::operator+|
|D3DXVec4BaryCentric				||
|D3DXVec4CatmullRom					|Vector4::CatmullRom|
|D3DXVec4Cross						||
|D3DXVec4Dot						|Vector4::Dot|
|D3DXVec4Hermite					|Vector4::Hermite|
|D3DXVec4Length						|Vector4::GetLength|
|D3DXVec4LengthSq					|Vector4::GetLengthSquared|
|D3DXVec4Lerp						|Vector4::Lerp|
|D3DXVec4Maximize					|Vector4::Max|
|D3DXVec4Minimize					|Vector4::Min|
|D3DXVec4Normalize					|Vector4::Normalize|
|D3DXVec4Scale						|Vector4::operator*|
|D3DXVec4Subtract					|Vector4::operator+|
|D3DXVec4Transform					|Vector4::Transform|
|D3DXVec4TransformArray				||
|									|Vector4::Clamp|


##Licence
[zlib/libpng License](http://opensource.org/licenses/zlib-license.php)
([日本語訳](http://sourceforge.jp/projects/opensource/wiki/licenses/zlib_libpng_license))

