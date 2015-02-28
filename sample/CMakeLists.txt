
include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/sample/dxlib)
link_libraries(LuminoMath_Static)

# Additional library directory
link_directories(${PROJECT_SOURCE_DIR}/sample/dxlib)

# DxLib samples.
#if (MSVC)
#	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
#	
#	add_executable(Sample_DxLibModelViewer DxLibModelViewer/DxLibModelViewer.cpp)
#	set_target_properties(Sample_DxLibModelViewer PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
#
#	add_executable(Sample_DxLibFlightSim DxLibFlightSim/DxLibFlightSim.cpp)
#	set_target_properties(Sample_DxLibFlightSim PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
#endif()
