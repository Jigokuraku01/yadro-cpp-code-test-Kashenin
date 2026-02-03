#include "my_exception.hpp"

#include <string>
MyException::MyException(int64_t code, std::string error)
    : m_code(code), m_error(std::move(error)) {
}

[[nodiscard]]
int64_t MyException::get_code() const {
    return m_code;
}

[[nodiscard]]
std::string MyException::get_error() const {
    return m_error;
}