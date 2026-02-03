#include "start_info.hpp"

void StartInfo::set_tables_cnt(std::uint32_t tables_cnt) {
    m_tables_cnt = tables_cnt;
}
void StartInfo::set_start_time(std::uint32_t start_time) {
    m_start_time = start_time;
}
void StartInfo::set_end_time(std::uint32_t end_time) {
    m_end_time = end_time;
}
void StartInfo::set_hour_price(std::uint32_t hour_price) {
    m_hour_price = hour_price;
}
std::uint32_t StartInfo::get_tables_cnt() const {
    return m_tables_cnt;
}
std::uint32_t StartInfo::get_start_time() const {
    return m_start_time;
}
std::uint32_t StartInfo::get_end_time() const {
    return m_end_time;
}
std::uint32_t StartInfo::get_hour_price() const {
    return m_hour_price;
}