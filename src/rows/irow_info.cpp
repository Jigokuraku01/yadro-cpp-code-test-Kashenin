#include "irow_info.hpp"

IRowInfo::IRowInfo(std::uint32_t row_type, std::uint32_t time)
    : m_row_type(row_type), m_time(time) {
    static std::uint32_t global_seq_counter = 0;
    m_seq_counter = global_seq_counter++;
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

std::uint32_t IRowInfo::get_seq_counter() const {
    return m_seq_counter;
}