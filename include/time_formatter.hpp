#pragma once

#include <cstdint>
#include <string>
class TimeFormatter {
  public:
    TimeFormatter() = delete;
    std::string static format_time(std::uint32_t time_in_minutes);
    std::uint32_t static parse_time(const std::string& time_str);
};