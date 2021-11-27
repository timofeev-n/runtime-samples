//◦ Playrix ◦
#pragma once
#include <runtime/com/interface.h>
#include <memory>

namespace App {


struct ABSTRACT_TYPE RuntimeSupplement
{
	static std::unique_ptr<RuntimeSupplement> Create();

	virtual ~RuntimeSupplement() = default;
};



}
