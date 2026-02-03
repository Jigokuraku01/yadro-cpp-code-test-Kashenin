#include "iwriter.hpp"

class ConsoleWriter : public IWriter {
  public:
    void write_line(const std::string& line) override;
};