#include "irow_info.hpp"

IRowInfo::IRowInfo(std::uint32_t row_type) : m_row_type(row_type) {
}

std::uint32_t IRowInfo::get_row_type() const {
    return m_row_type;
}

std::string IRowInfo::to_string() const {
    return "";
}