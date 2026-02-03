#include <gtest/gtest.h>

#include "start_info.hpp"

TEST(StartInfoTest, GettersSetters) {
    StartInfo info;
    info.set_tables_cnt(3);
    info.set_start_time(540); // 09:00
    info.set_end_time(1140);  // 19:00
    info.set_hour_price(10);

    EXPECT_EQ(info.get_tables_cnt(), 3);
    EXPECT_EQ(info.get_start_time(), 540);
    EXPECT_EQ(info.get_end_time(), 1140);
    EXPECT_EQ(info.get_hour_price(), 10);
}
