#pragma once
#include "../repository.hpp"
#include "irow_info.hpp"
#include <cstdint>

class IInputRowInfo : public IRowInfo {
  public:
    explicit IInputRowInfo(std::uint32_t row_type, std::uint32_t time);
    virtual void do_step(Repository& cur_repo) = 0;
    [[nodiscard]]
    static std::shared_ptr<IRowInfo>
    generate_exception_row(std::uint32_t time, std::string message);
};

class Type1RowInfo : public IInputRowInfo {
  public:
    Type1RowInfo(std::uint32_t time, std::string user_name);
    void do_step(Repository& cur_repo) override;
    [[nodiscard]]
    std::string get_user_name() const;

    [[nodiscard]]
    std::string to_string() const override;

  private:
    std::string m_user_name;
};

class Type2RowInfo : public IInputRowInfo {
  public:
    Type2RowInfo(std::uint32_t time, std::string user_name,
                 std::uint32_t table_id);
    void do_step(Repository& cur_repo) override;
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

class Type3RowInfo : public IInputRowInfo {
  public:
    Type3RowInfo(std::uint32_t time, std::string user_name);
    void do_step(Repository& cur_repo) override;
    [[nodiscard]]
    std::string get_user_name() const;

    [[nodiscard]]
    std::string to_string() const override;

  private:
    std::string m_user_name;
};

class Type4RowInfo : public IInputRowInfo {
  public:
    Type4RowInfo(std::uint32_t time, std::string user_name);
    void do_step(Repository& cur_repo) override;
    [[nodiscard]]
    std::string get_user_name() const;

    [[nodiscard]]
    std::string to_string() const override;

  private:
    std::string m_user_name;
};
