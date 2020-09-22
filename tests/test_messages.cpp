#include "gtest/gtest.h"
#include <bits/stdint-uintn.h>
#include "../src/web/messages.hpp"

TEST(MessageBuilderTest, writeLittleEndianWorks) {
    const uint32_t num = 0xD9B4BEF9;
    char toWrite[] = {' ', ' ', ' ', ' '};
    MessageBuilder::writeLittleEndian(num, toWrite);
    EXPECT_EQ(0xF9, (uint8_t) toWrite[0]);
    EXPECT_EQ(0xBE, (uint8_t) toWrite[1]);
    EXPECT_EQ(0xB4, (uint8_t) toWrite[2]);
    EXPECT_EQ(0xD9, (uint8_t) toWrite[3]);
}

TEST(MessageBuilderTest, writeIPAddrWorks) {
    char ip[] = "127.0.0.1";
    char toWrite[16];

    MessageBuilder::writeIPAddr(ip, false, toWrite);
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(0x00, (uint8_t) toWrite[i]);
    }
    EXPECT_EQ(0xFF, (uint8_t) toWrite[10]);
    EXPECT_EQ(0xFF, (uint8_t) toWrite[11]);
    EXPECT_EQ(127, (uint8_t) toWrite[12]);
    EXPECT_EQ(0, (uint8_t) toWrite[13]);
    EXPECT_EQ(0, (uint8_t) toWrite[14]);
    EXPECT_EQ(1, (uint8_t) toWrite[15]);
}

TEST(MessageBuilderTest, getVarStrSizeWorks) {
    const std::string empty = "";
    const std::string shortStr = "/Custom:0.10/";

    EXPECT_EQ(1, MessageBuilder::getVarStrSize(empty));
    EXPECT_EQ(1+shortStr.size(), MessageBuilder::getVarStrSize(shortStr));
}

TEST(MessageBuilderTest, getVerackMessageWorks) {
    char expected[] {
        static_cast<char>(0x0B), static_cast<char>(0x11), static_cast<char>(0x09), static_cast<char>(0x07),
            'v', 'e', 'r', 'a', 'c', 'k', '\0', '\0', '\0', '\0', '\0',
            0x00, 0x00, 0x00, 0x00,
            0x5D, static_cast<char>(0xF6), static_cast<char>(0xE0), static_cast<char>(0xE2)
            };
    ASSERT_STREQ(expected, MessageBuilder::getVerackMessage());
}

