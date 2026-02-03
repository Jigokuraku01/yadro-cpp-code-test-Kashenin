#pragma once

#include <cstdint>
#include <string>
class TimeFormatter {
  public:
    TimeFormatter() = delete;
    static std::string format_time(std::uint32_t time_in_minutes);
    static std::uint32_t parse_time(const std::string& time_str);

  private:
    static bool is_digit_only(const std::string_view& str);
};