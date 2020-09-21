#include "gtest/gtest.h"
#include "../src/web/messages.hpp"

class MessageBuilderTest: public ::testing::Test {
    protected:
        MessageBuilder messageBuilder;
};

TEST_F(MessageBuilderTest, getVarStrSizeWorks) {
    const std::string empty = "";
    const std::string shortStr = "/Custom:0.10/";

    EXPECT_EQ(1, messageBuilder.getVarStrSize(empty));
    EXPECT_EQ(1+shortStr.size(), messageBuilder.getVarStrSize(shortStr));
}

TEST_F(MessageBuilderTest, getVerackMessageWorks) {
    char expected[] {
        static_cast<char>(0x0B), static_cast<char>(0x11), static_cast<char>(0x09), static_cast<char>(0x07),
            'v', 'e', 'r', 'a', 'c', 'k', '\0', '\0', '\0', '\0', '\0',
            0x00, 0x00, 0x00, 0x00,
            0x5D, static_cast<char>(0xF6), static_cast<char>(0xE0), static_cast<char>(0xE2)
            };
    ASSERT_STREQ(expected, messageBuilder.getVerackMessage());
}

// TODO more tests
