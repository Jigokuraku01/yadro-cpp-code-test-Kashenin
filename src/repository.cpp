#include "repository.hpp"

#include "config.hpp"
#include "my_exception.hpp"
#include <algorithm>
#include <string_view>

void TableInfo::set_user_name(std::string_view user_name) {
    m_user_name = user_name;
}
std::string TableInfo::get_user_name() const {
    return m_user_name;
}
void TableInfo::set_occupied(bool is_occupied) {
    m_is_occupied = is_occupied;
}
bool TableInfo::is_occupied() const {
    return m_is_occupied;
}
void TableInfo::add_occupied_time(std::uint32_t time_in_minutes) {
    m_total_occupied_time += time_in_minutes;
}
std::uint32_t TableInfo::get_total_occupied_time() const {
    return m_total_occupied_time;
}

void TableInfo::set_last_occupied_start_time(std::uint32_t start_time) {
    m_last_occupied_start_time = start_time;
}
std::uint32_t TableInfo::get_last_occupied_start_time() const {
    return m_last_occupied_start_time;
}

void TableInfo::add_money_spent(std::uint32_t amount) {
    m_total_money_spent += amount;
}
std::uint32_t TableInfo::get_total_money_spent() const {
    return m_total_money_spent;
}

Repository::Repository(StartInfo start_info) : m_start_info(start_info) {
    for (std::uint32_t i = 1; i <= start_info.get_tables_cnt(); ++i) {
        m_tables[i] = TableInfo();
        m_free_tables.insert(i);
    }
}

std::map<std::uint32_t, TableInfo>& Repository::get_tables() {
    return m_tables;
}

void Repository::add_current_user(const std::string& user_name) {
    m_current_users.insert(user_name);
}
void Repository::remove_current_user(const std::string& user_name) {
    m_current_users.erase(user_name);
}
bool Repository::is_user_currently_in(const std::string& user_name) const {
    return m_current_users.contains(user_name);
}
void Repository::add_waiting_user(const std::string& user_name) {
    m_waiting_users.push(user_name);
    m_waiting_users_set.insert(user_name);
}
void Repository::remove_waiting_user() {
    if (!m_waiting_users.empty()) {
        m_waiting_users_set.erase(m_waiting_users.front());
        m_waiting_users.pop();
    }
}
bool Repository::has_waiting_users() const {
    return !m_waiting_users.empty();
}
std::string Repository::get_next_waiting_user() const {
    if (m_waiting_users.empty()) {
        throw MyException(Config::universal_error_code,
                          "No waiting users available");
    }
    return m_waiting_users.front();
}

void Repository::add_table(std::uint32_t table_id,
                           const TableInfo& table_info) {
    m_tables[table_id] = table_info;
}
TableInfo Repository::remove_table(std::uint32_t table_id) {
    TableInfo removed_table = m_tables.at(table_id);
    m_tables.erase(table_id);
    return removed_table;
}
bool Repository::is_table_free(std::uint32_t table_id) const {
    if (!m_tables.contains(table_id)) {
        return false;
    }
    return !m_tables.at(table_id).is_occupied();
}

bool Repository::has_free_tables() const {
    return !m_free_tables.empty();
}

bool Repository::is_queue_full() const {
    return m_waiting_users.size() >= m_tables.size();
}

void Repository::set_start_info(const StartInfo& start_info) {
    m_start_info = start_info;
}

StartInfo Repository::get_start_info() const {
    return m_start_info;
}

void Repository::add_history_entry(std::shared_ptr<IRowInfo> row_info) {
    m_history.push(std::move(row_info));
}

std::priority_queue<std::shared_ptr<IRowInfo>,
                    std::vector<std::shared_ptr<IRowInfo>>, compare_row_info>
Repository::get_history() const {
    return m_history;
}

void Repository::add_user_table(const std::string& user_name,
                                std::uint32_t table_id) {
    m_user_table_map[user_name] = table_id;
}
void Repository::remove_user_table(const std::string& user_name) {
    m_user_table_map.erase(user_name);
}
bool Repository::has_user_table(const std::string& user_name) const {
    return m_user_table_map.contains(user_name);
}
std::uint32_t
Repository::get_user_table_id(const std::string& user_name) const {
    return m_user_table_map.at(user_name);
}

std::set<std::string>& Repository::get_current_users() {
    return m_current_users;
}

std::uint32_t Repository::calculate_total_price(std::uint32_t time) const {
    const auto& start_info = m_start_info;
    return ((time + 59) / 60) * start_info.get_hour_price();
}

bool Repository::is_user_waiting(const std::string& user_name) const {
    return m_waiting_users_set.contains(user_name);
}

void Repository::remove_waiting_user_by_name(
    const std::string_view& user_name) {
    std::queue<std::string> temp_queue;
    while (!m_waiting_users.empty()) {
        if (m_waiting_users.front() != user_name) {
            temp_queue.push(m_waiting_users.front());
        }
        m_waiting_users.pop();
    }
    m_waiting_users = std::move(temp_queue);
}

void Repository::mark_table_occupied(std::uint32_t table_id) {
    m_tables.at(table_id).set_occupied(true);
    m_free_tables.erase(table_id);
}

void Repository::mark_table_free(std::uint32_t table_id) {
    m_tables.at(table_id).set_occupied(false);
    m_free_tables.insert(table_id);
}