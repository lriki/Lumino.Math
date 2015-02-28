cmake_minimum_required (VERSION 2.8)
project (LuminoMath)

set(LN_STATIC_LIB_OUTPUT_NAME "LuminoMath")
set(LN_UNIT_TEST_LINK_LIBS "")

#------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------
# Unicode char set.
#option(USING_UNICODE_CHAR_SET "Use unicode char set." ON)

# MSVC Runtime library.
option(MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME "static runtime (/MT, /MTd) or DLL runtime (/MD, /MDd) or to link either." ON)

#------------------------------------------------------------------------------
# Apply options
#------------------------------------------------------------------------------
# Unicode char set.
#if (USING_UNICODE_CHAR_SET)
#	add_definitions(-DUNICODE)
#	add_definitions(-D_UNICODE)
#endif()

if (WIN32)
	# MSVC Version name
	if (MSVC_VERSION EQUAL 1400)
		string(CONCAT LN_STATIC_LIB_OUTPUT_NAME ${LN_STATIC_LIB_OUTPUT_NAME} "-msvc80")
	elseif (MSVC_VERSION EQUAL 1500)
		string(CONCAT LN_STATIC_LIB_OUTPUT_NAME ${LN_STATIC_LIB_OUTPUT_NAME} "-msvc90")
	elseif (MSVC_VERSION EQUAL 1600)
		string(CONCAT LN_STATIC_LIB_OUTPUT_NAME ${LN_STATIC_LIB_OUTPUT_NAME} "-msvc100")
	elseif (MSVC_VERSION EQUAL 1700)
		string(CONCAT LN_STATIC_LIB_OUTPUT_NAME ${LN_STATIC_LIB_OUTPUT_NAME} "-msvc110")
	elseif (MSVC_VERSION EQUAL 1800)
		string(CONCAT LN_STATIC_LIB_OUTPUT_NAME ${LN_STATIC_LIB_OUTPUT_NAME} "-msvc120")
	endif()
	
	# MSVC Runtime library.
	if (MSVC_MULTI_THREAD_DLL_RUNTIME)
		#string(CONCAT LN_STATIC_LIB_OUTPUT_NAME ${LN_STATIC_LIB_OUTPUT_NAME} "-MD")
	else()
		#string(CONCAT LN_STATIC_LIB_OUTPUT_NAME ${LN_STATIC_LIB_OUTPUT_NAME} "-MT")
		# Overwrite options.
		foreach (flag
			CMAKE_C_FLAGS
			CMAKE_C_FLAGS_DEBUG
			CMAKE_C_FLAGS_RELEASE
			CMAKE_CXX_FLAGS
			CMAKE_CXX_FLAGS_DEBUG
			CMAKE_CXX_FLAGS_RELEASE)
			if (${flag} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
			endif()
			if (${flag} MATCHES "/MDd")
				string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
			endif()
		endforeach()
	endif()

elseif (APPLE)
	set(LN_UNIT_TEST_LINK_LIBS "pthread")
	
elseif (UNIX)
	set(LN_UNIT_TEST_LINK_LIBS "pthread")
	
else()
	# Not support.
    message(FATAL_ERROR "No supported platform was detected.")
endif()



#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------
# Header files.
set(LN_MATH_HEADERS
	include/LuminoMath.h
	include/Lumino/Math/Quaternion.inl
	include/Lumino/Math/Random.h
	include/Lumino/Math/SQTTransform.h
	include/Lumino/Math/Vector2.h
	include/Lumino/Math/Vector2.inl
	include/Lumino/Math/Vector3.h
	include/Lumino/Math/Vector3.inl
	include/Lumino/Math/Vector4.h
	include/Lumino/Math/Vector4.inl
	include/Lumino/Math/Common.h
	include/Lumino/Math/MathUtils.h
	include/Lumino/Math/Matrix.h
	include/Lumino/Math/Matrix.inl
	include/Lumino/Math/Plane.h
	include/Lumino/Math/Plane.inl
	include/Lumino/Math/Quaternion.h)

# Source files.
set(LN_MATH_SOURCES
	src/Math/Random.cpp
	src/Math/SQTTransform.cpp
	src/Math/Vector2.cpp
	src/Math/Vector3.cpp
	src/Math/Vector4.cpp
	src/Math/MathUtils.cpp
	src/Math/Matrix.cpp
	src/Math/Plane.cpp
	src/Math/Quaternion.cpp)

#------------------------------------------------------------------------------
# Output
#------------------------------------------------------------------------------
#if (NOT LN_EXTERNAL)

# Static library.
set(LN_STATIC_LIB_OUTPUT_NAME "${LN_STATIC_LIB_OUTPUT_NAME}-static")
add_library(LuminoMath_Static STATIC ${LN_MATH_SOURCES} ${LN_MATH_HEADERS})
set_target_properties(LuminoMath_Static PROPERTIES OUTPUT_NAME ${LN_STATIC_LIB_OUTPUT_NAME})
set_target_properties(LuminoMath_Static PROPERTIES DEBUG_POSTFIX "_d")
add_custom_command(
	TARGET LuminoMath_Static
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:LuminoMath_Static> "${PROJECT_SOURCE_DIR}/lib")

# Shared library.
add_library(LuminoMath_Shared SHARED ${LN_MATH_SOURCES} ${LN_MATH_HEADERS})
set_target_properties(LuminoMath_Shared PROPERTIES OUTPUT_NAME "LuminoMath")
set_target_properties(LuminoMath_Shared PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties (LuminoMath_Shared PROPERTIES COMPILE_DEFINITIONS "LUMINO_BUILD_DLL")
add_custom_command(
	TARGET LuminoMath_Shared
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:LuminoMath_Shared> "${PROJECT_SOURCE_DIR}/lib")
add_custom_command(
	TARGET LuminoMath_Shared
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_LINKER_FILE:LuminoMath_Shared> "${PROJECT_SOURCE_DIR}/lib")

# Unit test.
add_executable(
	UnitTest
	test/gtest/src/gtest-all.cc
	test/gtest/src/gtest_main.cc
	test/Test_Vector4.cpp
	test/Test_Math.cpp
	test/Test_Matrix.cpp
	test/Test_Plane.cpp
	test/Test_Quaternion.cpp
	test/Test_SQTTransform.cpp
	test/Test_Vector2.cpp
	test/Test_Vector3.cpp)
	
set(LIST_OF_DIRECTORIES
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/test/gtest/include"
	"${PROJECT_SOURCE_DIR}/test/gtest")
set_target_properties(
	UnitTest
	PROPERTIES INCLUDE_DIRECTORIES
	"${LIST_OF_DIRECTORIES}")
	
#set(my_definitions 
set_target_properties(UnitTest PROPERTIES COMPILE_DEFINITIONS "GTEST_HAS_TR1_TUPLE=0")
	
#GTEST_HAS_TR1_TUPLE=0

#target_include_directories(
#	UnitTest
#	PUBLIC include
#	PUBLIC test/gtest/include
#	PUBLIC test/gtest)
target_link_libraries(UnitTest LuminoMath_Static ${LN_UNIT_TEST_LINK_LIBS})
add_dependencies(UnitTest LuminoMath_Static)

# samples.
#add_subdirectory(sample)
