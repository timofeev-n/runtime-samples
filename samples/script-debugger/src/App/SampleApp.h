//◦ Playrix ◦
#pragma once
#include <runtime/com/interface.h>
#include <boost/asio/io_context.hpp>

#include <ThreadSafety.h>

#include <lua.hpp>
#include <memory>


namespace App {

/*
	Приемр приложения.
	Основные задачи:
		- создание и доступ к Lua;
		- запуск кглавного цикла приложеня;
*/
struct ABSTRACT_TYPE SampleApp
{
	using LuaMutex = Core::ThreadSafety::RecursiveMutex;

	static bool Exists();

	static SampleApp& Instance();


	virtual ~SampleApp() = default;

	virtual void Run() = 0;

	virtual void Stop() = 0;

	virtual boost::asio::io_context& GetIoContext() = 0;

	virtual lua_State* GetLua() const = 0;

	virtual LuaMutex& GetLuaMutex() = 0;
};


using LuaLockGuard = Core::ThreadSafety::LockGuard<SampleApp::LuaMutex>;
using LuaUniqueLock = Core::ThreadSafety::UniqueLock<SampleApp::LuaMutex>;


std::unique_ptr<SampleApp> CreateSampleApplication();


}
