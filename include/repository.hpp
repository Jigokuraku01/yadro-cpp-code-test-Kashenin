#pragma once
#include "irow_info.hpp"
#include "start_info.hpp"
#include <cstdint>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
class TableInfo {
  public:
    TableInfo() = default;

    void set_user_name(const std::string& user_name);
    [[nodiscard]]
    std::string get_user_name() const;
    void set_occupied(bool is_occupied);
    [[nodiscard]]
    bool is_occupied() const;
    void add_occupied_time(std::uint32_t time_in_minutes);
    [[nodiscard]]
    std::uint32_t get_total_occupied_time() const;
    void set_last_occupied_start_time(std::uint32_t start_time);
    [[nodiscard]]
    std::uint32_t get_last_occupied_start_time() const;

  private:
    std::string m_user_name;
    bool m_is_occupied = false;
    std::uint32_t m_total_occupied_time = 0;
    std::uint32_t m_last_occupied_start_time = 0;
};

class Repository {
  public:
    explicit Repository(StartInfo start_info);

    void add_current_user(const std::string& user_name);
    void remove_current_user(const std::string& user_name);
    bool is_user_currently_in(const std::string& user_name) const;

    void add_waiting_user(std::uint32_t user_id);
    void remove_waiting_user();
    bool has_waiting_users() const;
    std::uint32_t get_next_waiting_user() const;

    void add_table(std::uint32_t table_id, const TableInfo& table_info);
    void remove_table(std::uint32_t table_id);
    bool has_table(std::uint32_t table_id) const;

    std::unordered_map<std::uint32_t, TableInfo>& get_tables();

    void set_start_info(const StartInfo& start_info);
    StartInfo get_start_info() const;

    void add_history_entry(std::unique_ptr<IRowInfo> row_info);
    std::queue<std::unique_ptr<IRowInfo>> get_history() const;

  private:
    std::unordered_map<std::uint32_t, TableInfo> m_tables;
    StartInfo m_start_info;
    std::queue<std::uint32_t> m_waiting_users;
    std::unordered_set<std::string> m_current_users;
    std::queue<std::unique_ptr<IRowInfo>> m_history;
};
