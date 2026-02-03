#include "console_parser.hpp"

#include "config.hpp"
#include "my_exception.hpp"
#include <stdexcept>

void ConsoleParsedInfo::set_parsed_filename(const std::string& filename) {
    m_parsed_filename = filename;
}

std::string ConsoleParsedInfo::get_parsed_filename() const {
    return m_parsed_filename;
}

ConsoleParsedInfo ConsoleParser::parse(const std::vector<std::string>& args) {
    ConsoleParsedInfo parsed_info;
    if (args.size() != 1) {
        throw MyException(Config::universal_error_code,
                          "Invalid number of arguments");
    }

    parsed_info.set_parsed_filename(args[0]);
    return parsed_info;
}