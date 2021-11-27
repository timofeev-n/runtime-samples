//◦ Playrix ◦
#include "ScriptDebug.h"
#include <runtime/runtime/runtime.h>

#include <runtime/remoting/socketaddress.h>


namespace ScriptDebug {

/*
*/
class ScriptDebugSupportImpl final : public ScriptDebugSupport
{
	SINGLETON_MEMOP(ScriptDebugSupportImpl)

public:

	ScriptDebugSupportImpl() {

		using namespace Runtime;
		using namespace Runtime::Async;


		// Задача запускается на Runtime планировщике, но для запуска сессий используется scriptScheduler.
		_remotingTask = Async::Run([this](Scheduler::Ptr scriptScheduler) -> Task<> {

			_remotingController = CreateSampleRemotingController(std::move(scriptScheduler));
			LOG_DEBUG("[ScriptDebugger] Run Lua remoting");

			co_await _remotingController->Run(GetRemoteServiceBindAddress());
			LOG_DEBUG("[ScriptDebugger] Stop Lua remoting");
		
		}, RuntimeState::Instance().Scheduler(), Scheduler::GetCurrent()); // вместо RuntimeState::Instance().Scheduler() можно попробовать Async::Scheduler::GetDefault()
	}

	~ScriptDebugSupportImpl() {
		using namespace Runtime;

		if (!_remotingController) {
			return;
		}


		// DisposeAsync не может быть вызван в контексте текущего потока.
		// К текущему потоку скорее всего привязан планировщик.
		// Т.к. поток текущего планировщика будет заблокирован для ожидания завершения операции, то код выполнится,
		// но управление физически не сможет быть возвращено в заблокированный поток.
		Async::WaitResult(Async::Run([this]() -> Async::Task<> {
			co_await _remotingController->DisposeAsync();
			co_await _remotingTask;
		}, RuntimeState::Instance().Scheduler()));
	}


private:

	static std::string GetRemoteServiceBindAddress() {
		// в реальном приложении адрес может быть получен из настроек/аргументов.
		static const char* RemoteServiceDefaultAddress = "tcp://:62847";

		return {RemoteServiceDefaultAddress};
	}


	Runtime::Async::Task<> _remotingTask;
	Lua::Remoting::RemoteController::Ptr _remotingController;
};


// ----------------------------------------------------------------------------
ScriptDebugSupport::Ptr ScriptDebugSupport::Create() {
	return std::make_unique<ScriptDebugSupportImpl>();
}

}
