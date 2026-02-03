#include "emulation_handler.hpp"

#include "repository.hpp"
#include "start_info.hpp"
#include "time_formatter.hpp"
#include <format>

EmulationHandler::EmulationHandler(StartInfo start_info,
                                   std::unique_ptr<IReader> reader,
                                   std::unique_ptr<IWriter> writer)
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
}

void EmulationHandler::show_repository_history() {
    auto history = m_repository.get_history();
    m_writer->write_line(
        std::to_string(m_repository.get_start_info().get_start_time()));
    while (!history.empty()) {
        m_writer->write_line(history.front()->to_string());
        history.pop();
    }
    m_writer->write_line(
        std::to_string(m_repository.get_start_info().get_end_time()));
    auto tables = m_repository.get_tables();
    for (const auto& [table_id, table_info] : tables) {
        m_writer->write_line(
            std::format(
                "{} {} {}",
            table_id,
            calculate_total_price(table_info.get_total_occupied_time()),
            TimeFormatter::format_time(table_info.get_total_occupied_time()));
    }
}

std::uint32_t EmulationHandler::calculate_total_price(std::uint32_t time) {
    const auto& start_info = m_repository.get_start_info();
    return ((time + 59) / 60) * start_info.get_hour_price();
}