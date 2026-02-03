#pragma once

#include "ireader.hpp"
#include <fstream>
#include <memory>
class FileReader : public IReader {
  public:
    explicit FileReader(std::ifstream&& input_file);
    ~FileReader() override = default;

    std::optional<std::shared_ptr<IInputRowInfo>> read_row() override;
    StartInfo read_start_info() override;

  private:
    static std::uint32_t parse_single_value(std::istream& stream,
                                            const std::string& line);
    static std::pair<std::uint32_t, std::uint32_t>
    parse_time_range(std::istream& stream, const std::string& line);

    static std::string read_line(std::istream& stream);
    std::ifstream m_input_file;
};