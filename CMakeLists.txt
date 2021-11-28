cmake_minimum_required(VERSION 3.16)
project(runtime-samples
	LANGUAGES CXX
)

include(CheckCXXCompilerFlag)

get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)

set(ENGINE_UNITY_BUILD ON CACHE BOOL "Force Engine UNIT" FORCE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
	"${PROJECT_ROOT}/engine/cmake/modules"
	"${PROJECT_ROOT}/libs/lib-cppcommon/cmake/modules"
)

include(maker)
include(compiler_setup)

# Глобальные настройки Xcode под macOS
maker_set_global_xcode_properties( MACOSX
	SDKROOT macosx _
	MACOSX_DEPLOYMENT_TARGET "10.10" _
	VALID_ARCHS "x86_64"
)


maker_initialize(
	COMMON
		SUBDIRECTORIES
			"${PROJECT_ROOT}/engine"
			"${PROJECT_ROOT}/libs/lib-cppcommon"
			"${PROJECT_ROOT}/libs/lua-toolkit"
			"${PROJECT_ROOT}/samples/script-debugger"

	WINDOWS
		UNITY_DEACTIVATE_ORIGIN     ON
)

maker_finalize()