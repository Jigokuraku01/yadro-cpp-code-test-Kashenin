#pragma once

#include <cstdint>
#include <string>
class IRowInfo {
  public:
    virtual ~IRowInfo() = default;
    explicit IRowInfo(std::uint32_t row_type);

    [[nodiscard]]
    std::uint32_t get_row_type() const;

    [[nodiscard]]
    virtual std::string to_string() const;

  private:
    std::uint32_t m_row_type;
};