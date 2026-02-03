#pragma once

#include "console_parser.hpp"
#include "ireader.hpp"
#include "iwriter.hpp"
#include "repository.hpp"
class EmulationHandler {
  public:
    EmulationHandler(StartInfo start_info, std::shared_ptr<IReader> reader,
                     std::shared_ptr<IWriter> writer);
    void run_emulation();
    void show_repository_history();

  private:
    std::uint32_t calculate_total_price(std::uint32_t time) const;
    std::shared_ptr<IReader> m_reader;
    std::shared_ptr<IWriter> m_writer;
    Repository m_repository;
};