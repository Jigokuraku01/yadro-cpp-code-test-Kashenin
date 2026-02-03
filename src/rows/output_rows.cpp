#include "output_rows.hpp"

#include "time_formatter.hpp"
#include <format>

Type11RowInfo::Type11RowInfo(std::uint32_t time, std::string user_name)
    : IRowInfo(11, time), m_user_name(std::move(user_name)) {
}
std::string Type11RowInfo::get_user_name() const {
    return m_user_name;
}

std::string Type11RowInfo::to_string() const {
    return std::format("{} 11 {}", TimeFormatter::format_time(get_time()),
                       get_user_name());
}

Type12RowInfo::Type12RowInfo(std::uint32_t time, std::string user_name,
                             std::uint32_t table_id)
    : IRowInfo(12, time), m_user_name(std::move(user_name)),
      m_table_id(table_id) {
}
std::string Type12RowInfo::get_user_name() const {
    return m_user_name;
}
std::uint32_t Type12RowInfo::get_table_id() const {
    return m_table_id;
}

std::string Type12RowInfo::to_string() const {
    return std::format("{} 12 {} {}", TimeFormatter::format_time(get_time()),
                       get_user_name(), get_table_id());
}

Type13RowInfo::Type13RowInfo(std::uint32_t time, std::string error_message)
    : IRowInfo(13, time), m_error_message(std::move(error_message)) {
}
std::string Type13RowInfo::get_error_message() const {
    return m_error_message;
}

std::string Type13RowInfo::to_string() const {
    return std::format("{} 13 {}", TimeFormatter::format_time(get_time()),
                       get_error_message());
}