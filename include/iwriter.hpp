#pragma once
#include <string>
class IWriter {
  public:
    virtual ~IWriter() = default;
    virtual void write_line(const std::string& line) = 0;
};