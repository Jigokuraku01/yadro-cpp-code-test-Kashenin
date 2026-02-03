#include "file_reader.hpp"

#include "config.hpp"
#include "input_row_factory.hpp"
#include "ireader.hpp"
#include "my_exception.hpp"
#include "start_info.hpp"
#include "time_formatter.hpp"
#include <memory>
#include <sstream>
FileReader::FileReader(std::ifstream&& input_file)
    : m_input_file(std::move(input_file)) {
}

std::uint32_t FileReader::parse_single_value(std::istream& stream,
                                             const std::string& line) {
    std::uint32_t value;
    std::string remaining;

    if (!(stream >> value)) {
        throw MyException(Config::format_error_code, line);
    }

    if (stream >> remaining) {
        throw MyException(Config::format_error_code, line);
    }

    return value;
}

std::pair<std::uint32_t, std::uint32_t>
FileReader::parse_time_range(std::istream& stream, const std::string& line) {
    std::string start_time_str;
    std::string end_time_str;
    std::string remaining;

    if (!(stream >> start_time_str >> end_time_str)) {
        throw MyException(Config::format_error_code, line);
    }

    if (stream >> remaining) {
        throw MyException(Config::format_error_code, line);
    }

    return {TimeFormatter::parse_time(start_time_str),
            TimeFormatter::parse_time(end_time_str)};
}

std::string FileReader::read_line(std::istream& stream) {
    std::string line;
    if (!std::getline(stream, line)) {
        throw MyException(Config::format_error_code, line);
    }
    return line;
}

StartInfo FileReader::read_start_info() {
    StartInfo start_info;

    {
        std::string line = read_line(m_input_file);
        std::istringstream iss(line);
        auto tables_cnt = parse_single_value(iss, line);
        start_info.set_tables_cnt(tables_cnt);
    }

    {
        std::string line = read_line(m_input_file);
        std::istringstream iss(line);
        auto [start_time, end_time] = parse_time_range(iss, line);
        start_info.set_start_time(start_time);
        start_info.set_end_time(end_time);
    }

    {
        std::string line = read_line(m_input_file);
        std::istringstream iss(line);
        auto hour_price = parse_single_value(iss, line);
        start_info.set_hour_price(hour_price);
    }

    return start_info;
}

std::optional<std::shared_ptr<IInputRowInfo>> FileReader::read_row() {
    if (m_input_file.eof()) {
        return std::nullopt;
    }

    std::string line = read_line(m_input_file);
    std::shared_ptr<IInputRowInfo> row_info = InputRowFactory::create_row(line);
    return row_info;
}