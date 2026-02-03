#include "repository.hpp"

#include "my_exception.hpp"

void TableInfo::set_user_name(const std::string& user_name) {
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

Repository::Repository(StartInfo start_info) : m_start_info(start_info) {
    for (std::uint32_t i = 1; i <= start_info.get_tables_cnt(); ++i) {
        m_tables[i] = TableInfo();
    }
}

std::unordered_map<std::uint32_t, TableInfo>& Repository::get_tables() {
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
void Repository::add_waiting_user(std::uint32_t user_id) {
    m_waiting_users.push(user_id);
}
void Repository::remove_waiting_user() {
    if (!m_waiting_users.empty()) {
        m_waiting_users.pop();
    }
}
bool Repository::has_waiting_users() const {
    return !m_waiting_users.empty();
}
std::uint32_t Repository::get_next_waiting_user() const {
    if (m_waiting_users.empty()) {
        throw MyException(1, "No waiting users available");
    }
    return m_waiting_users.front();
}

void Repository::add_table(std::uint32_t table_id,
                           const TableInfo& table_info) {
    m_tables[table_id] = table_info;
}
void Repository::remove_table(std::uint32_t table_id) {
    m_tables.erase(table_id);
}
bool Repository::has_table(std::uint32_t table_id) const {
    return m_tables.contains(table_id);
}

void Repository::set_start_info(const StartInfo& start_info) {
    m_start_info = start_info;
}

StartInfo Repository::get_start_info() const {
    return m_start_info;
}

void Repository::add_history_entry(std::unique_ptr<IRowInfo> row_info) {
    m_history.push(std::move(row_info));
}

std::queue<std::unique_ptr<IRowInfo>> Repository::get_history() const {
    return m_history;
}