//◦ Playrix ◦
#include "RuntimeSupplement.h"
#include "SampleApp.h"

#include <runtime/memory/allocator.h>
#include <runtime/runtime/runtime.h>
#include <runtime/runtime/internal/runtimecomponent.h>
#include <runtime/threading/event.h>


namespace App {

//-----------------------------------------------------------------------------
/*
	Отвечает за инициализацию runtime подсистем.
	Runtime отвечает за организацию работы глобальных компонент, сети, асинхронных таймеров.
	RuntimeState работает в выделенном потоке, за создание которого отвечает логика приложения.

	RuntimeSupplementImpl:
		- создаёт поток для runtime системы;
		- создаёт глобальный общий планировщик (для пула задач);
		- к текущему потоку привязывает планировщика для выполнения задач в очереди/потоке приложения.
*/
class RuntimeSupplementImpl final : public RuntimeSupplement
{
	SINGLETON_MEMOP(RuntimeSupplementImpl)

public:

	RuntimeSupplementImpl() {

		using namespace Core;
		using namespace Runtime;
		using namespace Runtime::Async;

		// Assert(ThisThread::IsMain());
		// подразумевается, что текущий поток = главный поток приложения.
		// Именно к этому потоку привзявается планировщик, связанный с очередью задач приложения.
		// Движковые проекты для реализации очереди используют boost::asio::io_context.
		// Для создания планировщика (Async::Scheduler) на базе asio::io_context необходимо использовать: Runtime::CreateIoContextScheduler(ctx).

		_mainThreadScheduler = CreateIoContextScheduler(SampleApp::Instance().GetIoContext());
		Scheduler::SetThisThreadScheduler(_mainThreadScheduler);

		Threading::Event signal;

		_runtimeThread = std::thread([&]{

			ThisThread::SetName("Runtime Thread");

			auto runtime = RuntimeState::Create();

			{
				constexpr size_t ThreadPoolSize = 4;
				auto poolScheduler = CreatePoolScheduler(ThreadPoolSize);
				Scheduler::SetDefault(poolScheduler);
				// RegisterComponent необходим, что бы runtime при разрушении дождался завершения работы компонента: в данном случае всех задач, которые могут выполняться в пуле.
				RuntimeComponent::RegisterRuntimeComponent("", std::move(poolScheduler));
			}

			signal.Set(); 

			runtime->Run();

			LOG_DEBUG("[RuntimeSupplement] Stopped");
		});

		signal.Wait(); // продолжим работу дальше только после того, как runtime state будет создан (см. выше signal.Set()).
		LOG_DEBUG("[RuntimeSupplement] Initialized");
	}


	~RuntimeSupplementImpl() {
		using namespace Runtime;
		using namespace Runtime::Async;

		if (RuntimeState::Exists()){
			RuntimeState::Instance().Stop();
		}

		if (_runtimeThread.joinable()) {
			_runtimeThread.join();
		}
	}


private:

	Runtime::Async::Scheduler::Ptr _mainThreadScheduler;
	std::thread _runtimeThread;
};


//-----------------------------------------------------------------------------
std::unique_ptr<RuntimeSupplement> RuntimeSupplement::Create() {

	return std::make_unique<RuntimeSupplementImpl>();
}

}
