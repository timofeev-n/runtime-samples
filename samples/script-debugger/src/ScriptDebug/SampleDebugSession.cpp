//◦ Playrix ◦
#include "ScriptDebug.h"
#include "App/SampleApp.h"

#include <lua-toolkit/debug/luadebug.h>
#include <runtime/com/comclass.h>

namespace ScriptDebug {

using namespace Runtime;
using namespace Lua::Debug;

using namespace App;

/**
	Логика сессии отладчика Lua.
	Экземпляр создаётся когда клиент (UI) подключается к приложению.
	В рамках проекта основная задача класса: предоставить безопасный (с учётом локов) доступ к Lua.
	В каждом прилоежнии эта логика своя, реализуется через:
		* LuaDebugSessionController::AcquireLua() - вызывается системой для получения и лока Lua стейта;
		* LuaDebugSessionController::ReleaseLua() - вызывается системой для освобожения лока Lua стейта;
		* Async::Scheduler::Ptr LuaDebugSessionController::GetScheduler() - возвращает планировщика, который перелючит выполнение в Lua поток (главный поток пиложения);
		* LuaDebugSessionController::Stop() - обработка логики завершения отладочной сессии;
	
	
*/
class SampleDebugSession final : public Lua::Debug::LuaDebugSessionController
{
	COMCLASS_(LuaDebugSessionController)

public:

	/*
		Должен вызываться в главном потоке приложения.
		К этому потоку уже привязан нужный планровщик: Scheduler::GetCurrent() вернёт именно его (см. App/RuntimeSupplement.cpp)
	*/
	SampleDebugSession(): _schedulerRef(Runtime::Async::Scheduler::GetCurrent())
	{
		// Assert(Core::ThisThread::IsMain());
	}

	~SampleDebugSession() {
		LOG_INFO("[LuaDebugSession][Destruct]");
	}

public:

	lua_State* AcquireLua() noexcept override {
		Assert(!_luaLock);
		_luaLock.emplace(SampleApp::Instance().GetLuaMutex());

		return SampleApp::Instance().GetLua();
	}

	void ReleaseLua() noexcept override {
		Assert(_luaLock);
		_luaLock.reset();
	}

	Async::Scheduler::Ptr GetScheduler() override {
		return _schedulerRef.Acquire();
	}

	Async::Task<> Start([[maybe_unused]] StartMode mode, [[maybe_unused]] Runtime::RuntimeValue::Ptr config) override {
		LOG_INFO("[LuaDebugSession][Start] Enable lua debug");
		this->EnableDebug();

		return Async::Task<>::MakeResolved();
	}

	Async::Task<> Stop() override {
		LOG_INFO("[LuaDebugSession][Disconnect] Disable lua debug");
		this->DisableDebug();

		return Async::Task<>::MakeResolved();
	}

	Async::Scheduler::WeakPtr _schedulerRef;
	boost::optional<App::LuaUniqueLock> _luaLock;
};


//-----------------------------------------------------------------------------
Runtime::Debug::DebugSessionController::Ptr CreateSampleDebugSession() {
	return Runtime::Com::CreateInstance<SampleDebugSession>();
}

}
