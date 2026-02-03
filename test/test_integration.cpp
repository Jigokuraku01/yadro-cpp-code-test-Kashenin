#include <gtest/gtest.h>

#include "emulation_handler.hpp"
#include "file_reader.hpp"
#include "iwriter.hpp"
#include "my_exception.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Helper writer class for tests
class StringWriter : public IWriter {
  public:
    explicit StringWriter(std::ostream& out) : m_output(out) {
    }

    void write_line(const std::string& line) override {
        m_output << line << '\n';
    }

  private:
    std::ostream& m_output;
};

class IntegrationTest : public ::testing::Test {
  protected:
    static std::string get_test_file_path(const std::string& filename) {
        return "test/" + filename;
    }

    static std::vector<std::string> run_test_file(const std::string& filename) {
        std::string filepath = get_test_file_path(filename);

        // Read file
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open test file: " + filepath);
        }

        try {
            auto reader = std::make_shared<FileReader>(std::move(file));
            StartInfo start_info = reader->read_start_info();

            // Create string writer to capture output
            std::ostringstream output;
            auto writer = std::make_shared<StringWriter>(output);

            // Run emulation
            EmulationHandler handler(start_info, reader, writer);
            handler.run_emulation();
            handler.show_repository_history();

            // Parse output into lines
            std::vector<std::string> lines;
            std::string line;
            std::istringstream iss(output.str());
            while (std::getline(iss, line)) {
                lines.push_back(line);
            }

            return lines;
        }
        catch (const MyException& e) {
            throw std::runtime_error(std::string("MyException in file ") +
                                     filepath + ": " + e.get_error());
        }
        catch (const std::exception& e) {
            throw std::runtime_error(std::string("Error processing file ") +
                                     filepath + ": " + e.what());
        }
    }
};

TEST_F(IntegrationTest, ClientWaitingWithoutSeat) {
    auto output = run_test_file("test_client_waiting_without_seat.txt");

    bool found_error = false;
    for (const auto& line : output) {
        if (line.find("13 ICanWaitNoLonger") != std::string::npos) {
            found_error = true;
            break;
        }
    }
    EXPECT_TRUE(found_error)
        << "Should generate error when client waits without seat";
}

TEST_F(IntegrationTest, UnknownTable) {
    auto output = run_test_file("test_unknown_table.txt");

    bool found_table_error = false;
    for (const auto& line : output) {
        if (line.find("13") != std::string::npos &&
            line.find("09:05") != std::string::npos) {
            found_table_error = true;
            break;
        }
    }
    EXPECT_TRUE(found_table_error) << "Should generate error for unknown table";
}

TEST_F(IntegrationTest, EndOfDaySorting) {
    auto output = run_test_file("test_end_of_day_sorting.txt");

    std::vector<std::string> type11_events;
    for (const auto& line : output) {
        if (line.find("19:00 11") != std::string::npos) {
            type11_events.push_back(line);
        }
    }

    // Check alphabetical order
    if (type11_events.size() > 1) {
        for (size_t i = 1; i < type11_events.size(); ++i) {
            size_t pos1 = type11_events[i - 1].rfind(' ');
            size_t pos2 = type11_events[i].rfind(' ');
            std::string user1 = type11_events[i - 1].substr(pos1 + 1);
            std::string user2 = type11_events[i].substr(pos2 + 1);
            EXPECT_LT(user1, user2)
                << "Type11 events should be in alphabetical order";
        }
    }
}

TEST_F(IntegrationTest, OutsideWorkingHours) {
    auto output = run_test_file("test_outside_hours.txt");

    int not_open_yet_count = 0;
    for (const auto& line : output) {
        if (line.find("13 NotOpenYet") != std::string::npos) {
            not_open_yet_count++;
        }
    }

    EXPECT_GE(not_open_yet_count, 2)
        << "Should have at least 2 NotOpenYet errors";
}

TEST_F(IntegrationTest, TableSwitching) {
    auto output = run_test_file("test_table_switching.txt");

    bool found_2alice2 = false;
    bool found_2alice3 = false;

    for (const auto& line : output) {
        if (line.find("2 alice 2") != std::string::npos) {
            found_2alice2 = true;
        }
        if (line.find("2 alice 3") != std::string::npos) {
            found_2alice3 = true;
        }
    }

    EXPECT_TRUE(found_2alice2) << "alice should move to table 2";
    EXPECT_TRUE(found_2alice3) << "alice should move to table 3";
}

TEST_F(IntegrationTest, BasicScenario) {
    std::string filepath = get_test_file_path("test.txt");
    std::ifstream file(filepath);
    if (!file.good()) {
        GTEST_SKIP() << "test.txt not found";
    }

    auto output = run_test_file("test.txt");

    EXPECT_FALSE(output.empty());
    EXPECT_EQ(output.front().size(), 5) << "First line should be time HH:MM";
    EXPECT_GE(output.size(), 4);
}
