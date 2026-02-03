#include "time_formatter.hpp"

#include "config.hpp"
#include "my_exception.hpp"
#include <algorithm>
#include <any>
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
    std::string hours_str = time_str.substr(0, 2);
    std::string minutes_str = time_str.substr(3, 2);
    if (!is_digit_only(hours_str) || !is_digit_only(minutes_str)) {
        throw MyException(Config::time_format_error_code,
                          "Time contains non-digit characters");
    }
    auto hours = static_cast<std::uint32_t>(std::stoul(hours_str));
    auto minutes = static_cast<std::uint32_t>(std::stoul(minutes_str));

    if (hours >= 24 || minutes >= 60) {
        throw MyException(Config::time_format_error_code,
                          "Time values out of range");
    }

    return hours * 60 + minutes;
}

bool TimeFormatter::is_digit_only(const std::string_view& str) {
    if (str.empty()) {
        return true;
    }

    return std::ranges::all_of(str, [](char chr) { return std::isdigit(chr); });
}