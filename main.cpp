#include "config.hpp"
#include "console_parser.hpp"
#include "my_exception.hpp"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv) {
    try {
        std::vector<std::string> tmp_vec;
        tmp_vec.reserve(static_cast<std::uint64_t>(argc - 1));
        for (std::uint64_t i = 0; i < static_cast<std::uint64_t>(argc - 1);
             ++i) {
            tmp_vec.emplace_back(argv[i + 1]);
        }

        ConsoleParsedInfo parsed_info = ConsoleParser::parse(tmp_vec);
    }
    catch (const MyException& e) {
        if (e.get_code() == Config::format_error_code) {
            std::cout << e.get_error() << std::endl;
        }
    }
    catch (...) {
        std::cerr << "An unexpected error occurred." << std::endl;
    }
    return 0;
}