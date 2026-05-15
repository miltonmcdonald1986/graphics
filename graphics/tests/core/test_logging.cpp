#include <graphics/core/logging.hpp>
#include <gtest/gtest.h>

#include <graphics_internal/core/i_logger.hpp>

using namespace graphics::core;

class MockLogger : public ILogger
{
  public:
    struct Entry
    {
        LogLevel level;
        std::string message;
    };

    std::vector<Entry> entries;

    void log (LogLevel level, std::string_view message) override
    {
        entries.push_back ({level, std::string (message)});
    }
};

// Utility to reset logger before each test
class LoggingTest : public ::testing::Test
{
  protected:
    std::shared_ptr<MockLogger> mock;

    void SetUp() override
    {
        mock = std::make_shared<MockLogger>();
        get_logger() = mock;
    }
};

TEST_F (LoggingTest, forwards_to_logger)
{
    get_logger() = mock; // override the global logger

    log_message (LogLevel::Info, "Hello world");

    ASSERT_EQ (mock->entries.size(), 1);
    EXPECT_EQ (mock->entries[0].level, LogLevel::Info);
    EXPECT_EQ (mock->entries[0].message, "Hello world");
}

TEST_F (LoggingTest, multiple_messages_are_recorded)
{
    log_message (LogLevel::Debug, "A");
    log_message (LogLevel::Warn, "B");

    ASSERT_EQ (mock->entries.size(), 2);
    EXPECT_EQ (mock->entries[0].message, "A");
    EXPECT_EQ (mock->entries[1].message, "B");
}

TEST_F (LoggingTest, empty_message_is_allowed)
{
    log_message (LogLevel::Error, "");

    ASSERT_EQ (mock->entries.size(), 1);
    EXPECT_EQ (mock->entries[0].message, "");
}

TEST_F (LoggingTest, long_message_is_forwarded)
{
    std::string long_msg (5000, 'x');

    log_message (LogLevel::Info, long_msg);

    ASSERT_EQ (mock->entries.size(), 1);
    EXPECT_EQ (mock->entries[0].message, long_msg);
}

TEST_F (LoggingTest, all_levels_are_forwarded)
{
    log_message (LogLevel::Trace, "t");
    log_message (LogLevel::Debug, "d");
    log_message (LogLevel::Info, "i");
    log_message (LogLevel::Warn, "w");
    log_message (LogLevel::Error, "e");
    log_message (LogLevel::Critical, "c");

    ASSERT_EQ (mock->entries.size(), 6);

    EXPECT_EQ (mock->entries[0].level, LogLevel::Trace);
    EXPECT_EQ (mock->entries[1].level, LogLevel::Debug);
    EXPECT_EQ (mock->entries[2].level, LogLevel::Info);
    EXPECT_EQ (mock->entries[3].level, LogLevel::Warn);
    EXPECT_EQ (mock->entries[4].level, LogLevel::Error);
    EXPECT_EQ (mock->entries[5].level, LogLevel::Critical);
}

TEST_F (LoggingTest, logger_can_be_replaced)
{
    auto mock2 = std::make_shared<MockLogger>();
    get_logger() = mock2;

    log_message (LogLevel::Info, "X");

    ASSERT_EQ (mock2->entries.size(), 1);
    EXPECT_EQ (mock2->entries[0].message, "X");
}

TEST_F (LoggingTest, only_one_log_call_occurs)
{
    log_message (LogLevel::Warn, "Side effects?");

    ASSERT_EQ (mock->entries.size(), 1);
}