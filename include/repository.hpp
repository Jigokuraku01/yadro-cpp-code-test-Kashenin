#pragma once
#include <cstdint>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
class TableInfo {
  public:
    TableInfo() = default;

  private:
    std::string m_user_name;
};

class Repository {
  public:
    Repository() = default;

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

  private:
    std::unordered_map<std::uint32_t, TableInfo> m_tables;
    std::queue<std::uint32_t> m_waiting_users;
    std::unordered_set<std::string> m_current_users;
};
