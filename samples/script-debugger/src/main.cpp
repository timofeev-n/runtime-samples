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
