#pragma once

#include <cstdint>
#include <string>
class IRowInfo {
  public:
    virtual ~IRowInfo() = default;
    explicit IRowInfo(std::uint32_t row_type, std::uint32_t time);

    [[nodiscard]]
    std::uint32_t get_row_type() const;

    void set_time(std::uint32_t time);
    [[nodiscard]]
    std::uint32_t get_time() const;

    [[nodiscard]]
    virtual std::string to_string() const;

    [[nodiscard]]
    std::uint32_t get_seq_counter() const;

  private:
    std::uint32_t m_row_type;
    std::uint32_t m_time;
    std::uint32_t m_seq_counter;
};