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
	Test_Math.cpp
	Test_Matrix.cpp
	Test_Plane.cpp
	Test_Quaternion.cpp
	Test_SQTTransform.cpp
	Test_Vector2.cpp
	Test_Vector3.cpp
	Test_Vector4.cpp
	Test_ViewFrustum.cpp
)

#------------------------------------------------------------------------------
# Output
#------------------------------------------------------------------------------
#--------------------------------------
# UnitTest
include_directories(
	${PROJECT_SOURCE_DIR}/../include
	${PROJECT_SOURCE_DIR}/gtest/include
	${PROJECT_SOURCE_DIR}/gtest)

set(LN_TEST_LIBS LuminoMath_Static)
if (WIN32)
elseif (APPLE)
	list(APPEND LN_TEST_LIBS "pthread")	# for gtest.
elseif (UNIX)
	list(APPEND LN_TEST_LIBS "pthread")	# for gtest.
else()
endif()
link_libraries(${LN_TEST_LIBS})

add_definitions(-DGTEST_HAS_TR1_TUPLE=0)		# gtest avoidance of problem.
add_executable(${PROJECT_NAME} ${LN_SOURCE_FILES})
add_dependencies(${PROJECT_NAME} LuminoMath_Static)

