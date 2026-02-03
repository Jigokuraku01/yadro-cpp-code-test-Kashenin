#include "config.hpp"
#include "console_parser.hpp"
#include "console_writer.hpp"
#include "emulation_handler.hpp"
#include "file_reader.hpp"
#include "ireader.hpp"
#include "my_exception.hpp"
#include <cstdint>
#include <fstream>
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
        std::ifstream input_file(parsed_info.get_parsed_filename());
        if (!input_file.is_open()) {
            throw MyException(Config::universal_error_code,
                              "Failed to open input file");
        }
        std::unique_ptr<IReader> reader =
            std::make_unique<FileReader>(std::move(input_file));
        std::unique_ptr<IWriter> writer = std::make_unique<ConsoleWriter>();
        EmulationHandler emulation_handler(
            reader->read_start_info(), std::move(reader), std::move(writer));
        emulation_handler.run_emulation();
        emulation_handler.show_repository_history();
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