#include <graphics/core/logging.hpp>

#include <memory>
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
  public:
    struct Entry
    {
        LogLevel level;
        std::string message;
    };

    auto get_entries() const -> std::span<const Entry> { return m_entries; }

    void log (LogLevel level, std::string_view message) override
    {
        m_entries.push_back (Entry{
            .level = level, .message = std::string (message)});
    }

  private:
    std::vector<Entry> m_entries;
};

class LoggingTest : public ::testing::Test
{
  public:
    void SetUp() override
    {
        mock = std::make_shared<MockLogger>();
        get_logger() = mock;
    }

    auto get_mock_logger() const -> std::shared_ptr<MockLogger> { return mock; }

  private:
    std::shared_ptr<MockLogger> mock;
};

} // namespace

TEST_F (LoggingTest, ForwardsToLogger)
{
    auto mock = get_mock_logger();
    get_logger() = mock; // override the global logger

    log_message (LogLevel::Info, "Hello world");

    const auto& entries = mock->get_entries();
    ASSERT_EQ (entries.size(), 1);
    EXPECT_EQ (entries.front().level, LogLevel::Info);
    EXPECT_EQ (entries.front().message, "Hello world");
}

TEST_F (LoggingTest, MultipleMessagesAreRecorded)
{
    log_message (LogLevel::Debug, "A");
    log_message (LogLevel::Warn, "B");

    auto mock = get_mock_logger();
    const auto& entries = mock->get_entries();
    ASSERT_EQ (entries.size(), 2);
    for (auto it = entries.begin(); it != entries.end(); ++it)
    {
        auto index = std::distance (it, entries.begin());
        switch (index)
        {
        case 0:
            EXPECT_EQ ((*it).message, "A");
            break;
        case 1:
            EXPECT_EQ ((*it).message, "B");
            break;
        }
    }
}

TEST_F (LoggingTest, EmptyMessageIsAllowed)
{
    log_message (LogLevel::Error, "");

    auto mock = get_mock_logger();
    const auto& entries = mock->get_entries();
    ASSERT_EQ (entries.size(), 1);
    EXPECT_EQ (entries.front().message, "");
}

TEST_F (LoggingTest, LongMessageIsForwarded)
{
    std::string long_msg (5000, 'x');

    log_message (LogLevel::Info, long_msg);

    auto mock = get_mock_logger();
    const auto& entries = mock->get_entries();
    ASSERT_EQ (entries.size(), 1);
    EXPECT_EQ (entries.front().message, long_msg);
}

TEST_F (LoggingTest, AllLevelsAreForwarded)
{
    log_message (LogLevel::Trace, "t");
    log_message (LogLevel::Debug, "d");
    log_message (LogLevel::Info, "i");
    log_message (LogLevel::Warn, "w");
    log_message (LogLevel::Error, "e");
    log_message (LogLevel::Critical, "c");

    auto mock = get_mock_logger();
    const auto& entries = mock->get_entries();

    ASSERT_EQ (entries.size(), 6);

    for (auto it = entries.begin(); it != entries.end(); ++it)
    {
        auto index = std::distance (it, entries.begin());
        switch (index)
        {
        case 0:
            EXPECT_EQ ((*it).level, LogLevel::Trace);
            break;
        case 1:
            EXPECT_EQ ((*it).level, LogLevel::Debug);
            break;
        case 2:
            EXPECT_EQ ((*it).level, LogLevel::Info);
            break;
        case 3:
            EXPECT_EQ ((*it).level, LogLevel::Warn);
            break;
        case 4:
            EXPECT_EQ ((*it).level, LogLevel::Error);
            break;
        case 5:
            EXPECT_EQ ((*it).level, LogLevel::Critical);
            break;
        }
    }
}

TEST_F (LoggingTest, LoggerCanBeReplaced)
{
    auto mock2 = std::make_shared<MockLogger>();
    get_logger() = mock2;

    log_message (LogLevel::Info, "X");

    const auto& entries = mock2->get_entries();
    ASSERT_EQ (entries.size(), 1);
    EXPECT_EQ (entries.front().message, "X");
}

TEST_F (LoggingTest, OnlyOneLogCallOccurs)
{
    log_message (LogLevel::Warn, "Side effects?");

    ASSERT_EQ (get_mock_logger()->get_entries().size(), 1);
}