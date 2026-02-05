#pragma once
#include "rows/irow_info.hpp"
#include "start_info.hpp"
#include <cstdint>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <string>
#include <string_view>
#include <unordered_set>
class TableInfo {
  public:
    TableInfo() = default;

    void set_user_name(std::string_view user_name);
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

    void add_money_spent(std::uint32_t amount);
    [[nodiscard]]
    std::uint32_t get_total_money_spent() const;

  private:
    std::string m_user_name;
    bool m_is_occupied = false;
    std::uint32_t m_total_occupied_time = 0;
    std::uint32_t m_last_occupied_start_time = 0;
    std::uint32_t m_total_money_spent = 0;
};

struct compare_row_info {
    bool operator()(const std::shared_ptr<IRowInfo>& lhs,
                    const std::shared_ptr<IRowInfo>& rhs) const {
        if (lhs->get_time() != rhs->get_time()) {
            return lhs->get_time() > rhs->get_time();
        }
        return lhs->get_seq_counter() > rhs->get_seq_counter();
    }
};

class Repository {
  public:
    explicit Repository(StartInfo start_info);

    void add_current_user(const std::string& user_name);
    void remove_current_user(const std::string& user_name);
    [[nodiscard]]
    bool is_user_currently_in(const std::string& user_name) const;

    void add_waiting_user(const std::string& user_name);
    void remove_waiting_user();
    void remove_waiting_user_by_name(const std::string_view& user_name);
    [[nodiscard]]
    bool has_waiting_users() const;
    [[nodiscard]]
    std::string get_next_waiting_user() const;
    [[nodiscard]]
    bool is_queue_full() const;
    [[nodiscard]]
    bool is_user_waiting(const std::string& user_name) const;

    [[nodiscard]]
    bool has_free_tables() const;

    void add_table(std::uint32_t table_id, const TableInfo& table_info);
    TableInfo remove_table(std::uint32_t table_id);
    [[nodiscard]]
    bool is_table_free(std::uint32_t table_id) const;

    std::map<std::uint32_t, TableInfo>& get_tables();

    void set_start_info(const StartInfo& start_info);
    [[nodiscard]]
    StartInfo get_start_info() const;

    void add_history_entry(std::shared_ptr<IRowInfo> row_info);
    [[nodiscard]]
    std::priority_queue<std::shared_ptr<IRowInfo>,
                        std::vector<std::shared_ptr<IRowInfo>>,
                        compare_row_info> get_history() const;

    void add_user_table(const std::string& user_name, std::uint32_t table_id);
    void remove_user_table(const std::string& user_name);
    [[nodiscard]]
    bool has_user_table(const std::string& user_name) const;
    [[nodiscard]]
    std::uint32_t get_user_table_id(const std::string& user_name) const;

    std::set<std::string>& get_current_users();

    [[nodiscard]]
    std::uint32_t calculate_total_price(std::uint32_t time) const;

    void mark_table_occupied(std::uint32_t table_id);
    void mark_table_free(std::uint32_t table_id);

    void free_table_from_user(const std::string& user_name, std::uint32_t time);

  private:
    //table_id -> TableInfo
    std::map<std::uint32_t, TableInfo> m_tables;
    //user_name -> table_id
    std::map<std::string, std::uint32_t> m_user_table_map;
    StartInfo m_start_info;
    std::queue<std::string> m_waiting_users;
    std::unordered_set<std::string> m_waiting_users_set;

    std::set<std::string> m_current_users;

    std::priority_queue<std::shared_ptr<IRowInfo>,
                        std::vector<std::shared_ptr<IRowInfo>>,
                        compare_row_info>
        m_history;

    std::unordered_set<std::uint32_t> m_free_tables;
};
