#include <gtest/gtest.h>

#include "time_formatter.hpp"

TEST(TimeFormatterTest, FormatTimeBasic) {
    EXPECT_EQ(TimeFormatter::format_time(0), "00:00");
    EXPECT_EQ(TimeFormatter::format_time(60), "01:00");
    EXPECT_EQ(TimeFormatter::format_time(90), "01:30");
    EXPECT_EQ(TimeFormatter::format_time(540), "09:00");
    EXPECT_EQ(TimeFormatter::format_time(1140), "19:00");
}

TEST(TimeFormatterTest, ParseTimeBasic) {
    EXPECT_EQ(TimeFormatter::parse_time("00:00"), 0);
    EXPECT_EQ(TimeFormatter::parse_time("01:00"), 60);
    EXPECT_EQ(TimeFormatter::parse_time("09:00"), 540);
    EXPECT_EQ(TimeFormatter::parse_time("19:00"), 1140);
    EXPECT_EQ(TimeFormatter::parse_time("23:59"), 1439);
}
