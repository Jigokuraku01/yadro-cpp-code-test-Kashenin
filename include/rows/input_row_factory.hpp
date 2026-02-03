#pragma once
#include "ireader.hpp"
#include <memory>
class InputRowFactory {
  public:
    static std::shared_ptr<IInputRowInfo> create_row(const std::string& line);
};