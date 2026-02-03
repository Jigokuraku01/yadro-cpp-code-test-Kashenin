#pragma once

#include "irow_info.hpp"
#include <string>
class Type11RowInfo : public IRowInfo {
  public:
    Type11RowInfo(std::uint32_t time, std::string user_name);
    [[nodiscard]]
    std::string get_user_name() const;

    [[nodiscard]]
    std::string to_string() const override;

  private:
    std::string m_user_name;
};

class Type12RowInfo : public IRowInfo {
  public:
    Type12RowInfo(std::uint32_t time, std::string user_name,
                  std::uint32_t table_id);
    [[nodiscard]]
    std::string get_user_name() const;
    [[nodiscard]]
    std::uint32_t get_table_id() const;

    [[nodiscard]]
    std::string to_string() const override;

  private:
    std::string m_user_name;
    std::uint32_t m_table_id;
};

class Type13RowInfo : public IRowInfo {
  public:
    Type13RowInfo(std::uint32_t time, std::string error_message);
    [[nodiscard]]
    std::string get_error_message() const;

    [[nodiscard]]
    std::string to_string() const override;

  private:
    std::string m_error_message;
};