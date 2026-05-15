#include <graphics/core/logging.hpp>

#include <string>
#include <string_view>
#include <vector>

#include <gtest/gtest.h>

#include <graphics/core/log_level.hpp>

#include <graphics_internal/core/i_logger.hpp>

using graphics::core::get_logger;
using graphics::core::ILogger;
using graphics::core::LogLevel;

namespace
{

struct MockLogger : public ILogger
{
    struct Entry
    {
        LogLevel level;
        std::string message;
    };

    std::vector<Entry> entries;

    void log (LogLevel level, std::string_view message) override
    {
        entries.push_back (Entry{
            .level = level, .message = std::string (message)});
    }
};

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

} // namespace

TEST_F (LoggingTest, ForwardsToLogger)
{
    get_logger() = mock; // override the global logger

    log_message (LogLevel::Info, "Hello world");

    ASSERT_EQ (mock->entries.size(), 1);
    EXPECT_EQ (mock->entries[0].level, LogLevel::Info);
    EXPECT_EQ (mock->entries[0].message, "Hello world");
}

TEST_F (LoggingTest, MultipleMessagesAreRecorded)
{
    log_message (LogLevel::Debug, "A");
    log_message (LogLevel::Warn, "B");

    ASSERT_EQ (mock->entries.size(), 2);
    EXPECT_EQ (mock->entries[0].message, "A");
    EXPECT_EQ (mock->entries[1].message, "B");
}

TEST_F (LoggingTest, EmptyMessageIsAllowed)
{
    log_message (LogLevel::Error, "");

    ASSERT_EQ (mock->entries.size(), 1);
    EXPECT_EQ (mock->entries[0].message, "");
}

TEST_F (LoggingTest, LongMessageIsForwarded)
{
    std::string long_msg (5000, 'x');

    log_message (LogLevel::Info, long_msg);

    ASSERT_EQ (mock->entries.size(), 1);
    EXPECT_EQ (mock->entries[0].message, long_msg);
}

TEST_F (LoggingTest, AllLevelsAreForwarded)
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

TEST_F (LoggingTest, LoggerCanBeReplaced)
{
    auto mock2 = std::make_shared<MockLogger>();
    get_logger() = mock2;

    log_message (LogLevel::Info, "X");

    ASSERT_EQ (mock2->entries.size(), 1);
    EXPECT_EQ (mock2->entries[0].message, "X");
}

TEST_F (LoggingTest, OnlyOneLogCallOccurs)
{
    log_message (LogLevel::Warn, "Side effects?");

    ASSERT_EQ (mock->entries.size(), 1);
}