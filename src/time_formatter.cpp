#include "time_formatter.hpp"

#include "config.hpp"
#include "my_exception.hpp"
#include <format>

std::string TimeFormatter::format_time(std::uint32_t time_in_minutes) {
    std::uint32_t hours = time_in_minutes / 60;
    std::uint32_t minutes = time_in_minutes % 60;

    return std::format("{:02d}:{:02d}", hours, minutes);
}

std::uint32_t TimeFormatter::parse_time(const std::string& time_str) {
    if (time_str.size() != 5 || time_str[2] != ':') {
        throw MyException(Config::time_format_error_code,
                          "Invalid time format");
    }

    auto hours = static_cast<std::uint32_t>(std::stoul(time_str.substr(0, 2)));
    auto minutes =
        static_cast<std::uint32_t>(std::stoul(time_str.substr(3, 2)));

    if (hours >= 24 || minutes >= 60) {
        throw MyException(Config::time_format_error_code,
                          "Time values out of range");
    }

    return hours * 60 + minutes;
}