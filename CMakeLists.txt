# Note:
#	CMake Useful Variables
#	http://www.cmake.org/Wiki/CMake_Useful_Variables
#
# Debug build on Command line.
# $ cmake -DCMAKE_BUILD_TYPE=Debug ..
#

cmake_minimum_required (VERSION 2.8)
project (LuminoMath)

set(LN_STATIC_LIB_OUTPUT_NAME ${PROJECT_NAME})
set(LN_UNIT_TEST_LINK_LIBS "")

#------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------

# MSVC Runtime library.
option(LN_MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME "static runtime (/MT, /MTd) or DLL runtime (/MD, /MDd) or to link either." ON)

#------------------------------------------------------------------------------
# Apply options
#------------------------------------------------------------------------------

if (WIN32)
	#------------------------------------------------------
	# Make static lib name.
	#	{ProjectName}_{msvcVer}{Arch}_static_{Runtime}
	#	e.g)	LuminoMath_msvc120x86_static_MTd.lib
	if (MSVC_VERSION EQUAL 1400)
		set(LN_STATIC_LIB_OUTPUT_NAME "${LN_STATIC_LIB_OUTPUT_NAME}_msvc80")
	elseif (MSVC_VERSION EQUAL 1500)
		set(LN_STATIC_LIB_OUTPUT_NAME "${LN_STATIC_LIB_OUTPUT_NAME}_msvc90")
	elseif (MSVC_VERSION EQUAL 1600)
		set(LN_STATIC_LIB_OUTPUT_NAME "${LN_STATIC_LIB_OUTPUT_NAME}_msvc100")
	elseif (MSVC_VERSION EQUAL 1700)
		set(LN_STATIC_LIB_OUTPUT_NAME "${LN_STATIC_LIB_OUTPUT_NAME}_msvc110")
	elseif (MSVC_VERSION EQUAL 1800)
		set(LN_STATIC_LIB_OUTPUT_NAME "${LN_STATIC_LIB_OUTPUT_NAME}_msvc120")
	elseif (MSVC_VERSION EQUAL 1900)
		set(LN_STATIC_LIB_OUTPUT_NAME "${LN_STATIC_LIB_OUTPUT_NAME}_msvc140")
	endif()
	
	# Architecture.
	# http://stackoverflow.com/questions/5334095/cmake-multiarchitecture-compilation
	if (${CMAKE_EXE_LINKER_FLAGS} MATCHES "/machine:x64")	# /machine:x64 
		set(LN_STATIC_LIB_OUTPUT_NAME "${LN_STATIC_LIB_OUTPUT_NAME}x64")
	else()
		set(LN_STATIC_LIB_OUTPUT_NAME "${LN_STATIC_LIB_OUTPUT_NAME}x86")
	endif()
	
	set(LN_STATIC_LIB_OUTPUT_NAME "${LN_STATIC_LIB_OUTPUT_NAME}_static")
	
	# MSVC Runtime library.
	if (LN_MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME)
		set(LN_STATIC_LIB_OUTPUT_NAME "${LN_STATIC_LIB_OUTPUT_NAME}_MT")
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
	else()
		set(LN_STATIC_LIB_OUTPUT_NAME "${LN_STATIC_LIB_OUTPUT_NAME}_MD")
	endif()
	set(CMAKE_DEBUG_POSTFIX "d")
	# end   Make static lib name.
	#------------------------------------------------------

elseif (APPLE)
	set(LN_STATIC_LIB_OUTPUT_NAME ${CMAKE_PROJECT_NAME})
	set(CMAKE_DEBUG_POSTFIX "_d")
	
elseif (UNIX)
	set(LN_STATIC_LIB_OUTPUT_NAME ${CMAKE_PROJECT_NAME})
	set(CMAKE_DEBUG_POSTFIX "_d")
	
else()
	# Not support.
    message(FATAL_ERROR "No supported platform was detected.")
endif()

#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------
#--------------------------------------
# Header files.
set(LN_HEADER_FILES
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
	include/Lumino/Math/Quaternion.h
	include/Lumino/Math/Version.h
	include/Lumino/Math/ViewFrustum.h)

#--------------------------------------
# Source files.
set(LN_SOURCE_FILES
	src/Math/Random.cpp
	src/Math/SQTTransform.cpp
	src/Math/Vector2.cpp
	src/Math/Vector3.cpp
	src/Math/Vector4.cpp
	src/Math/MathUtils.cpp
	src/Math/Matrix.cpp
	src/Math/Plane.cpp
	src/Math/Quaternion.cpp
	src/Math/ViewFrustum.cpp)

#------------------------------------------------------------------------------
# Output
#------------------------------------------------------------------------------
#--------------------------------------
# Static library.
set(LN_STATIC_LIB_OUTPUT_NAME "${LN_STATIC_LIB_OUTPUT_NAME}")
add_library(LuminoMath_Static STATIC ${LN_SOURCE_FILES} ${LN_HEADER_FILES})
set_target_properties(LuminoMath_Static PROPERTIES OUTPUT_NAME ${LN_STATIC_LIB_OUTPUT_NAME})
add_custom_command(
	TARGET LuminoMath_Static
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:LuminoMath_Static> "${CMAKE_SOURCE_DIR}/lib")

#--------------------------------------
# Shared library.
add_library(LuminoMath_Shared SHARED ${LN_SOURCE_FILES} ${LN_HEADER_FILES})
set_target_properties(LuminoMath_Shared PROPERTIES OUTPUT_NAME "LuminoMath")
set_target_properties(LuminoMath_Shared PROPERTIES COMPILE_DEFINITIONS "LUMINO_BUILD_DLL")
set_target_properties(LuminoMath_Shared PROPERTIES DEBUG_POSTFIX "_d")		# Invalid CMake 2.8.7 (Ubuntu12.04).  Use "set(CMAKE_DEBUG_POSTFIX "_d")" Instand
add_custom_command(
	TARGET LuminoMath_Shared
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:LuminoMath_Shared> "${CMAKE_SOURCE_DIR}/lib")
add_custom_command(
	TARGET LuminoMath_Shared
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_LINKER_FILE:LuminoMath_Shared> "${CMAKE_SOURCE_DIR}/lib")

#--------------------------------------
# Unit test.
add_subdirectory(test)
