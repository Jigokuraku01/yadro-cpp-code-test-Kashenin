#include "irow_info.hpp"

IRowInfo::IRowInfo(std::uint32_t row_type, std::uint32_t time)
    : m_row_type(row_type), m_time(time) {
}

std::uint32_t IRowInfo::get_row_type() const {
    return m_row_type;
}

void IRowInfo::set_time(std::uint32_t time) {
    m_time = time;
}

std::uint32_t IRowInfo::get_time() const {
    return m_time;
}

std::string IRowInfo::to_string() const {
    return "";
}