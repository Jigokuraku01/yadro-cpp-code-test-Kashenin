#include "input_row_factory.hpp"

#include "config.hpp"
#include "ireader.hpp"
#include "my_exception.hpp"
#include "time_formatter.hpp"
#include <memory>
#include <sstream>

std::unique_ptr<IInputRowInfo>
InputRowFactory::create_row(const std::string& line) {
    std::istringstream stream(line);

    try {
        std::string time;
        std::uint32_t row_type;
        if (!(stream >> time >> row_type)) {
            throw MyException(Config::format_error_code, line);
        }

        std::uint32_t parsed_time = TimeFormatter::parse_time(time);
        switch (row_type) {
            case 1: {
                std::string user_name;
                if (!(stream >> user_name)) {
                    throw MyException(Config::format_error_code, line);
                }
                return std::make_unique<Type1RowInfo>(parsed_time, user_name);
            }
            case 2: {
                std::string user_name;
                std::uint32_t table_id;
                if (!(stream >> user_name >> table_id)) {
                    throw MyException(Config::format_error_code, line);
                }
                return std::make_unique<Type2RowInfo>(parsed_time, user_name,
                                                      table_id);
            }
            case 3: {
                std::string user_name;
                if (!(stream >> user_name)) {
                    throw MyException(Config::format_error_code, line);
                }
                return std::make_unique<Type3RowInfo>(parsed_time, user_name);
            }
            case 4: {
                std::string user_name;
                if (!(stream >> user_name)) {
                    throw MyException(Config::format_error_code, line);
                }
                return std::make_unique<Type4RowInfo>(parsed_time, user_name);
            }
            default:
                throw MyException(Config::format_error_code, line);
        }
    }
    catch (MyException& e) {
        if (e.get_code() == Config::format_error_code ||
            e.get_code() == Config::time_format_error_code) {
            throw MyException(Config::format_error_code, line);
        }
    }

    throw MyException(Config::format_error_code, line);
}