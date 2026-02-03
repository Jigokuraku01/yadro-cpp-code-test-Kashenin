#pragma once

#include "rows/input_rows.hpp"
#include "start_info.hpp"
#include <memory>
#include <optional>
class IReader {
  public:
    virtual ~IReader() = default;
    virtual std::optional<std::shared_ptr<IInputRowInfo>> read_row() = 0;
    virtual StartInfo read_start_info() = 0;
};