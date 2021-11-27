//◦ Playrix ◦
#pragma once
#include <lua-toolkit/debug/debugsessioncontroller.h>
#include <lua-toolkit/remotecontroller.h>
#include <memory>


namespace ScriptDebug {

struct ABSTRACT_TYPE ScriptDebugSupport
{
	using Ptr = std::unique_ptr<ScriptDebugSupport>;

	static Ptr Create();

	virtual ~ScriptDebugSupport() = default;
};


Runtime::Debug::DebugSessionController::Ptr CreateSampleDebugSession();

/*
	Метод вызывается из произвольного потока, поэтому целевой планировщик ожидается явным параметром
*/
Lua::Remoting::RemoteController::Ptr CreateSampleRemotingController(Runtime::Async::Scheduler::Ptr);


}



