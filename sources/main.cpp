/**
 * Copyright 2021 Marc SIBERT
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// #define LOG		1

#include "computer.h"

#include <iostream>

#if __cplusplus < 201703L
#error "Need C++17 compiler for using <filesystem>"
#endif
#include <filesystem>

#include <fstream>
#include <exception>

int main(int argc, char** argv) {
   setvbuf(stdin, NULL, _IONBF, 0); //turn off buffering
   setvbuf(stdout, NULL, _IONBF, 0);

#ifdef LOG
	std::filesystem::path path(argv[0]);
	std::ofstream out(path.stem().string() + ".log");
	auto old_rdbuf = std::clog.rdbuf();
	std::clog.rdbuf(out.rdbuf());
#endif

	try {
		if (argc > 2) {
			std::cerr << "Invalid number of arguments!" << std::endl;
			return EXIT_FAILURE;
		}
		const char* prog = argc > 1 ? argv[1] : "CCP-DR.64K";
		const unsigned addr = argc > 1 ? 0x0100 : 0xF400;
		Computer<64, 0xFC00, 0xFE00> computer(prog, addr);
		computer.start();
	}
	catch (std::exception& e) {
		std::cerr << "Exception " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
