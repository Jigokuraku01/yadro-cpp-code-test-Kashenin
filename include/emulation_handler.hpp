#pragma once

#include "console_parser.hpp"
#include "ireader.hpp"
#include "iwriter.hpp"
#include "repository.hpp"
class EmulationHandler {
  public:
    EmulationHandler(StartInfo start_info, std::unique_ptr<IReader> reader,
                     std::unique_ptr<IWriter> writer);
    void run_emulation();
    void show_repository_history();

  private:
    std::uint32_t calculate_total_price(std::uint32_t time);
    std::unique_ptr<IReader> m_reader;
    std::unique_ptr<IWriter> m_writer;
    Repository m_repository;
};