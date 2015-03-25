# Note:
#	CMake Useful Variables
#	http://www.cmake.org/Wiki/CMake_Useful_Variables

cmake_minimum_required (VERSION 2.8)
project (LuminoMath_UnitTest)

#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------
#--------------------------------------
# Source files.
set(LN_SOURCE_FILES
	gtest/src/gtest-all.cc
	gtest/src/gtest_main.cc
	Test_Vector4.cpp
	Test_Math.cpp
	Test_Matrix.cpp
	Test_Plane.cpp
	Test_Quaternion.cpp
	Test_SQTTransform.cpp
	Test_Vector2.cpp
	Test_Vector3.cpp)

#------------------------------------------------------------------------------
# Output
#------------------------------------------------------------------------------
#--------------------------------------
# UnitTest
include_directories(
	${PROJECT_SOURCE_DIR}/../include
	${PROJECT_SOURCE_DIR}/gtest/include
	${PROJECT_SOURCE_DIR}/gtest)
add_definitions(-DGTEST_HAS_TR1_TUPLE=0)		# gtest avoidance of problem.
link_libraries(pthread)							# for gtest.
link_libraries(LuminoMath_Static)
add_executable(${PROJECT_NAME} ${LN_SOURCE_FILES})
add_dependencies(${PROJECT_NAME} LuminoMath_Static)
