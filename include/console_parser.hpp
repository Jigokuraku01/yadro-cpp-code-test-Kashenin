#pragma once

#include <string>
#include <vector>
class ConsoleParsedInfo {
  public:
    ConsoleParsedInfo() = default;
    void set_parsed_filename(const std::string& filename);

    [[nodiscard]]
    std::string get_parsed_filename() const;

  private:
    std::string m_parsed_filename;
};

class ConsoleParser {
  public:
    static ConsoleParsedInfo parse(const std::vector<std::string>& args);
    ConsoleParser() = delete;
};
