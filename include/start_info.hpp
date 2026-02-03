#pragma once

#include <cstdint>
class StartInfo {
  public:
    StartInfo() = default;
    void set_tables_cnt(std::uint32_t tables_cnt);
    void set_start_time(std::uint32_t start_time);
    void set_end_time(std::uint32_t end_time);
    void set_hour_price(std::uint32_t hour_price);

    [[nodiscard]]
    std::uint32_t get_tables_cnt() const;
    [[nodiscard]]
    std::uint32_t get_start_time() const;
    [[nodiscard]]
    std::uint32_t get_end_time() const;
    [[nodiscard]]
    std::uint32_t get_hour_price() const;

  private:
    //time in minutes from 00:00
    std::uint32_t m_tables_cnt = 0;
    std::uint32_t m_start_time = 0;
    std::uint32_t m_end_time = 0;
    std::uint32_t m_hour_price = 0;
};