#include "console_writer.hpp"

#include <iostream>

void ConsoleWriter::write_line(const std::string& line) {
    std::cout << line << std::endl;
}