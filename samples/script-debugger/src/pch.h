//◦ Playrix ◦
#pragma once

#include <EngineConfig.h>

#if ENGINE_OS_WINDOWS
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define __unused
#include <Platform/Windows/WindowsHeaders.h>

#endif // ENGINE_OS_WINDOWS

#include <runtime/threading/event.h>

#include <lua.hpp>

#include <boost/asio.hpp>

#include <iostream>
#include <memory>
#include <mutex>
#include <thread>


#include <EngineAssert.h>
#include <Core/Log.h>
#include <PlayrixEngine.h>
