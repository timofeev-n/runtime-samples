//◦ Playrix ◦
#include "App/SampleApp.h"
#include <runtime/utils/strings.h>
#include <Utils/Thread.h>

#include <runtime/remoting/socketaddress.h>

#include <tuple>
#include <regex>
#include <string_view>

using namespace Core;
using namespace Runtime;


constexpr auto DefaultMatchOptions = std::regex_constants::ECMAScript | std::regex_constants::icase;

std::string_view ToStringView(std::sub_match<std::string_view::iterator> str) {
	const size_t len = str.second - str.first;
	if (len == 0) {
		return {};
	}

	const char* const ptr = &(*str.first);

	return std::string_view{ptr, len};
}

std::tuple<std::string_view, std::string_view> ParseAddressString(std::string_view address)
{
	std::match_results<std::string_view::iterator> match;

	if (!std::regex_match(address.begin(), address.end(), match, std::regex {"^([A-Za-z0-9\\+_-]+)://(.+)$", DefaultMatchOptions}))
	{
		
	}

	Assert(match.size() == 3);


	std::string_view protocol = ToStringView(match[1]);
	std::string_view addressString = ToStringView(match[2]);

	return {protocol, addressString};
}

int main() {

	using namespace App;

	Threading::Event signal;

	std::thread appThread([&] {
		// ThisThread::SetAsMain(); // это нельзя вызвать: "спасибо" неявному GlobalInitialization.  :-[
		ThisThread::SetName("App Thread");

		auto app = CreateSampleApplication();
		signal.Set();

		app->Run();
	});

	signal.Wait();

	std::cout << "enter command:\n\n";

	do {
		std::string command;
		std::cout << "> ";
		std::cin >> command;

		if (Strings::icaseEqual(command, "quit") || Strings::icaseEqual(command, "exit")) {

			std::cout << "quiting...\n";

			if (SampleApp::Exists()) {
				SampleApp::Instance().Stop();
			}
			break;
		}
	}
	while(true);


	appThread.join();


	return 0;
}
