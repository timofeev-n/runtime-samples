//◦ Playrix ◦
#include "SampleApp.h"
#include "RuntimeSupplement.h"
#include "ScriptDebug/ScriptDebug.h"

#include <runtime/memory/allocator.h>
#include <runtime/runtime/runtime.h>
#include <runtime/remoting/socketaddress.h>


namespace App {

class SampleAppImpl;

namespace {

struct ApplicationInstanceState
{
	SampleAppImpl* instance = nullptr;

	static ApplicationInstanceState& Get() {
		static ApplicationInstanceState state;

		return (state);
	}
};


} // namespace


//-----------------------------------------------------------------------------
class SampleAppImpl final : public SampleApp
{
	SINGLETON_MEMOP(SampleAppImpl)

public:
	SampleAppImpl() {
		Assert(ApplicationInstanceState::Get().instance == nullptr);
		ApplicationInstanceState::Get().instance = this;

		_lua = luaL_newstate();
		luaL_openlibs(_lua);

	}

	~SampleAppImpl() {

		if (_lua) {
			lua_close(_lua);
		}

		Assert(ApplicationInstanceState::Get().instance == this);
		ApplicationInstanceState::Get().instance = nullptr;
	}

private:

	void Run() override {

		// RuntimeSupplement должен быть создан при инициализации в момент когда глобальный экземпляр приложения уже доступен.
		// Уничтожится при завершении приложения. В данном случае это произойдёт автоматически,
		// но в реальном приложении скорее всего будут события AppInit/AppTermnated (в идеале само приложение ничего не должно знать о существовании Runtime подсистемы).
		const auto runtimeSupplement = RuntimeSupplement::Create();

		// Должен быть создан после инициализации приложения и создания экземпляра Runtime.
		const auto scriptDebug = ScriptDebug::ScriptDebugSupport::Create();

		//Runtime::Async::Run([] {

		//	auto [protocol, address] = Runtime::Remoting::SocketAddress::ParseAddressString("tcp://:62847");

		//}, Runtime::RuntimeState::Instance().Scheduler());

		{
			const auto execGuard =  boost::asio::make_work_guard(_ioContext);

			while (!_ioContext.stopped()) {
				_ioContext.run_one();
			}
		}

		std::cout << "app done.\n";
	}

	void Stop() override {
		_ioContext.stop();
	}

	boost::asio::io_context& GetIoContext() override {
		return _ioContext;
	}

	lua_State* GetLua() const override {
		Assert(_lua);
		return _lua;
	}

	LuaMutex& GetLuaMutex() override {
		return (_luaMutex);
	}


	boost::asio::io_context _ioContext;
	lua_State* _lua;
	LuaMutex _luaMutex;
	
};


//-----------------------------------------------------------------------------


bool SampleApp::Exists() {
	return ApplicationInstanceState::Get().instance != nullptr;
}


SampleApp& SampleApp::Instance() {

	SampleAppImpl* const instance = ApplicationInstanceState::Get().instance;
	Assert(instance);
	return *instance;
}

std::unique_ptr<SampleApp> CreateSampleApplication() {
	return std::make_unique<SampleAppImpl>();
}

}
