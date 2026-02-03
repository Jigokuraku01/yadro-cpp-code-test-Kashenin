#include "repository.hpp"

#include "my_exception.hpp"

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
