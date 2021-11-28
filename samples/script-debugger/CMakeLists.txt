cmake_minimum_required(VERSION 3.16)

project(script-debugger-sample
	LANGUAGES CXX
)

compiler_common_debug_flags(DEBUG_COMPILE_FLAGS)


maker_create_target(debugger_sample
	COMMON 
		BUILD_TYPE	TEST
		UNITY_BUILD OFF
		IDE_FOLDER "samples"

		SRCS
			"${CMAKE_CURRENT_SOURCE_DIR}/src"

		PRIVATE_INCLUDE_DIRS
			"${CMAKE_CURRENT_SOURCE_DIR}/src"

		PRECOMPILED_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h"

		LINK_TARGETS
			lua-toolkit

	WINDOWS
		PRECOMPILED_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.cpp"
		COMPILE_OPTIONS_DEBUG ${DEBUG_COMPILE_FLAGS}

		OUTPUT_DIRECTORY "${PROJECT_ROOT}/win"
		OUTPUT_NAME_RELEASE "debugger_sample"
		OUTPUT_NAME_DEBUG  "debugger_sample_debug"

	MACOSX
		BUILD_VARIANT               "Enterprise"
		OUTPUT_NAME                 "debugger_sample"

		DEFINITIONS
			MACOS_DEVELOPMENT=0

)


