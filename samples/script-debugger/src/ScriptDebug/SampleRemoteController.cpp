//◦ Playrix ◦
#include "ScriptDebug.h"
#include "App/SampleApp.h"

#include <lua-toolkit/remotecontroller.h>
#include <runtime/com/comclass.h>


namespace ScriptDebug {

using namespace Runtime;

/**
	Логика сервиса для удалённо взаимодействия с Lua в рамках приложения.
	Основная задача: создать отладочную сессию на запрос внешнего клиента (UI).
	
*/
class SampleRemoteController final : public Lua::Remoting::RemoteController
{
	COMCLASS_(Lua::Remoting::RemoteController)

public:

	SampleRemoteController(Runtime::Async::Scheduler::Ptr scheduler): _schedulerRef(scheduler)
	{}

	std::vector<DebugLocation> GetDebugLocations() const override {
		return std::vector<DebugLocation> {
			{"default_id", "default"}
		};
	}

	/*
		Может быть вызван из неопределённого потока, но сессия должна быть создана именно в главном потоке приложения (т.к. сессия захватывает планировщик через вызов Scheduler::GetCurrent()).
		(планировщик может быть так же передан в качестве параметра - это уже специфично для каждой реализации).
	*/
	Runtime::Async::Task<Runtime::Debug::DebugSessionController::Ptr> CreateDebugSession(std::string id) override {
		using namespace Runtime;

		if (auto scheduler = _schedulerRef.Acquire()) {
			co_await scheduler;
		}
		else {
			co_return nothing;
		}

		co_return CreateSampleDebugSession();
	}

private:

	Runtime::Async::Scheduler::WeakPtr _schedulerRef;
};

//-----------------------------------------------------------------------------
Lua::Remoting::RemoteController::Ptr CreateSampleRemotingController(Runtime::Async::Scheduler::Ptr scheduler) {
	Assert(scheduler);
	return Runtime::Com::CreateInstance<SampleRemoteController>(std::move(scheduler));
}

}
