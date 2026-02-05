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

TEST_F(IntegrationTest, MainTest) {
    auto output = run_test_file("test.txt");

    std::vector<std::string> expected = {"09:00",
                                         "00:00 1 client1",
                                         "00:00 13 NotOpenYet",
                                         "08:48 1 client1",
                                         "08:48 13 NotOpenYet",
                                         "09:41 1 client1",
                                         "09:48 1 client2",
                                         "09:52 3 client1",
                                         "09:52 13 ICanWaitNoLonger!",
                                         "09:54 2 client1 1",
                                         "10:25 2 client2 2",
                                         "10:58 1 client3",
                                         "10:59 2 client3 3",
                                         "11:30 1 client4",
                                         "11:35 2 client4 2",
                                         "11:35 13 PlaceIsBusy",
                                         "11:45 3 client4",
                                         "12:33 4 client1",
                                         "12:33 12 client4 1",
                                         "12:43 4 client2",
                                         "15:52 4 client4",
                                         "19:00 11 client3",
                                         "22:00 1 client5",
                                         "22:00 13 NotOpenYet",
                                         "19:00",
                                         "1 70 05:58",
                                         "2 30 02:18",
                                         "3 90 08:01"};

    ASSERT_EQ(output.size(), expected.size())
        << "Output should have exactly " << expected.size() << " lines";

    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(output[i], expected[i]) << "Line " << (i + 1) << " mismatch";
    }
}

TEST_F(IntegrationTest, QueueFull) {
    auto output = run_test_file("test_queue_full.txt");

    std::vector<std::string> expected = {"09:00",
                                         "09:00 1 client1",
                                         "09:01 2 client1 1",
                                         "09:02 1 client2",
                                         "09:03 2 client2 2",
                                         "09:05 1 client3",
                                         "09:06 3 client3",
                                         "09:10 1 client4",
                                         "09:11 3 client4",
                                         "09:15 1 client5",
                                         "09:16 3 client5",
                                         "09:16 11 client5",
                                         "19:00 11 client1",
                                         "19:00 11 client2",
                                         "19:00 11 client3",
                                         "19:00 11 client4",
                                         "19:00",
                                         "1 100 09:59",
                                         "2 100 09:57"};

    ASSERT_EQ(output.size(), expected.size())
        << "Output should have exactly " << expected.size() << " lines";

    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(output[i], expected[i]) << "Line " << (i + 1) << " mismatch";
    }
}

TEST_F(IntegrationTest, ClientWaitingWithoutSeat) {
    auto output = run_test_file("test_client_waiting_with_seat.txt");

    std::vector<std::string> expected = {"09:00",
                                         "09:00 1 alice",
                                         "09:05 1 bob",
                                         "09:06 2 bob 1",
                                         "09:10 1 charlie",
                                         "09:11 2 charlie 2",
                                         "09:15 3 alice",
                                         "09:20 4 bob",
                                         "09:20 12 alice 1",
                                         "09:25 3 charlie",
                                         "09:25 13 ClientAlreadyHasTable",
                                         "19:00 11 alice",
                                         "19:00 11 charlie",
                                         "19:00",
                                         "1 110 09:54",
                                         "2 100 09:49"};

    ASSERT_EQ(output.size(), expected.size())
        << "Output should have exactly " << expected.size() << " lines";

    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(output[i], expected[i]) << "Line " << (i + 1) << " mismatch";
    }
}

TEST_F(IntegrationTest, UnknownTable) {
    auto output = run_test_file("test_unknown_table.txt");

    std::vector<std::string> expected = {
        "09:00",           "09:00 1 alice",
        "09:05 2 alice 5", "09:05 13 UnknownTable",
        "09:10 2 alice 1", "09:15 4 alice",
        "19:00",           "1 10 00:05",
        "2 0 00:00"};

    ASSERT_EQ(output.size(), expected.size())
        << "Output should have exactly " << expected.size() << " lines";

    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(output[i], expected[i]) << "Line " << (i + 1) << " mismatch";
    }
}

TEST_F(IntegrationTest, EndOfDaySorting) {
    auto output = run_test_file("test_end_of_day_sorting.txt");

    std::vector<std::string> expected = {
        "09:00",           "09:00 1 dave",      "09:01 2 dave 1",
        "09:05 1 charlie", "09:06 2 charlie 2", "09:10 1 alice",
        "09:11 2 alice 3", "09:15 1 bob",       "19:00 11 alice",
        "19:00 11 bob",    "19:00 11 charlie",  "19:00 11 dave",
        "19:00",           "1 100 09:59",       "2 100 09:54",
        "3 100 09:49"};

    ASSERT_EQ(output.size(), expected.size())
        << "Output should have exactly " << expected.size() << " lines";

    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(output[i], expected[i]) << "Line " << (i + 1) << " mismatch";
    }
}

TEST_F(IntegrationTest, OutsideWorkingHours) {
    auto output = run_test_file("test_outside_hours.txt");

    std::vector<std::string> expected = {"09:00",
                                         "08:45 1 alice",
                                         "08:45 13 NotOpenYet",
                                         "09:00 1 bob",
                                         "09:05 2 bob 1",
                                         "19:00 1 charlie",
                                         "19:00 13 NotOpenYet",
                                         "19:00 11 bob",
                                         "19:05 1 dave",
                                         "19:05 13 NotOpenYet",
                                         "19:00",
                                         "1 100 09:55",
                                         "2 0 00:00"};

    ASSERT_EQ(output.size(), expected.size())
        << "Output should have exactly " << expected.size() << " lines";

    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(output[i], expected[i]) << "Line " << (i + 1) << " mismatch";
    }
}

TEST_F(IntegrationTest, TableSwitching) {
    auto output = run_test_file("test_table_switching.txt");

    std::vector<std::string> expected = {"09:00",           "09:00 1 alice",
                                         "09:05 2 alice 1", "09:30 2 alice 2",
                                         "10:00 2 alice 3", "10:30 4 alice",
                                         "19:00",           "1 10 00:25",
                                         "2 10 00:30",      "3 10 00:30"};

    ASSERT_EQ(output.size(), expected.size())
        << "Output should have exactly " << expected.size() << " lines";

    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(output[i], expected[i]) << "Line " << (i + 1) << " mismatch";
    }
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
