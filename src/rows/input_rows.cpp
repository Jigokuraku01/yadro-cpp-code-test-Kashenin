#include "input_rows.hpp"

#include "irow_info.hpp"
#include "output_rows.hpp"
#include "time_formatter.hpp"
#include <format>
#include <memory>

IInputRowInfo::IInputRowInfo(std::uint32_t row_type) : IRowInfo(row_type) {
}
std::shared_ptr<IRowInfo>
IInputRowInfo::generate_exception_row(std::uint32_t time, std::string message) {
    return std::make_shared<Type13RowInfo>(time, std::move(message));
}

Type1RowInfo::Type1RowInfo(std::uint32_t time, std::string user_name)
    : IInputRowInfo(1), m_time(time), m_user_name(std::move(user_name)) {
}

std::uint32_t Type1RowInfo::get_time() const {
    return m_time;
}

std::string Type1RowInfo::get_user_name() const {
    return m_user_name;
}

std::string Type1RowInfo::to_string() const {
    return std::format("{} 1 {}", TimeFormatter::format_time(get_time()),
                       get_user_name());
}

Type2RowInfo::Type2RowInfo(std::uint32_t time, std::string user_name,
                           std::uint32_t table_id)
    : IInputRowInfo(2), m_time(time), m_user_name(std::move(user_name)),
      m_table_id(table_id) {
}
std::uint32_t Type2RowInfo::get_time() const {
    return m_time;
}
std::string Type2RowInfo::get_user_name() const {
    return m_user_name;
}

std::uint32_t Type2RowInfo::get_table_id() const {
    return m_table_id;
}

std::string Type2RowInfo::to_string() const {
    return std::format("{} 2 {} {}", TimeFormatter::format_time(get_time()),
                       get_user_name(), get_table_id());
}

Type3RowInfo::Type3RowInfo(std::uint32_t time, std::string user_name)
    : IInputRowInfo(3), m_time(time), m_user_name(std::move(user_name)) {
}
std::uint32_t Type3RowInfo::get_time() const {
    return m_time;
}

std::string Type3RowInfo::get_user_name() const {
    return m_user_name;
}

std::string Type3RowInfo::to_string() const {
    return std::format("{} 3 {}", TimeFormatter::format_time(get_time()),
                       get_user_name());
}

Type4RowInfo::Type4RowInfo(std::uint32_t time, std::string user_name)
    : IInputRowInfo(4), m_time(time), m_user_name(std::move(user_name)) {
}
std::uint32_t Type4RowInfo::get_time() const {
    return m_time;
}

std::string Type4RowInfo::get_user_name() const {
    return m_user_name;
}

std::string Type4RowInfo::to_string() const {
    return std::format("{} 4 {}", TimeFormatter::format_time(get_time()),
                       get_user_name());
}