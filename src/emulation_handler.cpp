#include "emulation_handler.hpp"

#include "output_rows.hpp"
#include "repository.hpp"
#include "start_info.hpp"
#include "time_formatter.hpp"
#include <format>
#include <memory>

EmulationHandler::EmulationHandler(StartInfo start_info,
                                   std::shared_ptr<IReader> reader,
                                   std::shared_ptr<IWriter> writer)
    : m_reader(std::move(reader)), m_writer(std::move(writer)),
      m_repository(start_info) {
}

void EmulationHandler::run_emulation() {
    while (true) {
        auto inp_row = m_reader->read_row();
        if (!inp_row.has_value()) {
            break;
        }
        inp_row.value()->do_step(m_repository);
    }

    auto cur_users = m_repository.get_current_users();
    for (const auto& user_name : cur_users) {
        if (m_repository.has_user_table(user_name)) {
            std::uint32_t table_id = m_repository.get_user_table_id(user_name);
            TableInfo& table_info = m_repository.get_tables().at(table_id);
            table_info.set_occupied(false);
            std::uint32_t occupied_time =
                m_repository.get_start_info().get_end_time() -
                table_info.get_last_occupied_start_time();
            table_info.add_occupied_time(occupied_time);
            m_repository.remove_user_table(user_name);
        }

        m_repository.remove_current_user(user_name);
        m_repository.add_history_entry(std::make_shared<Type11RowInfo>(
            m_repository.get_start_info().get_end_time(), user_name));
    }
}

void EmulationHandler::show_repository_history() {
    m_writer->write_line(TimeFormatter::format_time(
        m_repository.get_start_info().get_start_time()));
    auto history = m_repository.get_history();
    while (!history.empty()) {
        m_writer->write_line(history.top()->to_string());
        history.pop();
    }
    m_writer->write_line(TimeFormatter::format_time(
        m_repository.get_start_info().get_end_time()));
    auto tables = m_repository.get_tables();
    for (const auto& [table_id, table_info] : tables) {
        m_writer->write_line(std::format(
            "{} {} {}", table_id,
            calculate_total_price(table_info.get_total_occupied_time()),
            TimeFormatter::format_time(table_info.get_total_occupied_time())));
    }
}

std::uint32_t
EmulationHandler::calculate_total_price(std::uint32_t time) const {
    const auto& start_info = m_repository.get_start_info();
    return ((time + 59) / 60) * start_info.get_hour_price();
}