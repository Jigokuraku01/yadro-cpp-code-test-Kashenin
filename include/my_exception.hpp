#pragma once

#include <cstdint>
#include <string>
class MyException {
  public:
    MyException(std::int64_t code, std::string error);

    [[nodiscard]]
    std::int64_t get_code() const;
    [[nodiscard]]
    std::string get_error() const;

  private:
    std::int64_t m_code;
    std::string m_error;
};