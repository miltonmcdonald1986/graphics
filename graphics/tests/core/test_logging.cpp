#include <graphics/core/logging.hpp>

#include <cstddef>
#include <iterator>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include <gtest/gtest.h>

#include <graphics/core/log_level.hpp>

#include <internal/core/i_logger.hpp>

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

    [[nodiscard]] auto get_entries() const -> std::span<const Entry>
    {
        return m_entries;
    }

    auto log (LogLevel level, std::string_view message) -> void override
    {
        m_entries.push_back (Entry{
            .level = level, .message = std::string (message) });
    }

  private:
    std::vector<Entry> m_entries;
};

class LoggingTest : public ::testing::Test
{
  protected:
    auto SetUp() -> void override
    {
        mock = std::make_shared<MockLogger>();
        get_logger() = mock;
    }

    [[nodiscard]] auto get_mock_logger() const -> std::shared_ptr<MockLogger>
    {
        return mock;
    }

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
        auto index = std::distance (entries.begin(), it);
        switch (index)
        {
        case 0:
            EXPECT_EQ ((*it).message, "A");
            break;
        case 1:
            EXPECT_EQ ((*it).message, "B");
            break;
        default:
            FAIL();
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
    const std::size_t num_chars = 5000;
    const std::string long_msg (num_chars, 'x');

    log_message (LogLevel::Info, long_msg);

    auto mock = get_mock_logger();
    const auto& entries = mock->get_entries();
    ASSERT_EQ (entries.size(), 1);
    EXPECT_EQ (entries.front().message, long_msg);
}

TEST_F (LoggingTest, TraceIsForwarded)
{
    log_message (LogLevel::Trace, "t");
    auto entries = get_mock_logger()->get_entries();
    ASSERT_EQ (entries.size(), 1);
    EXPECT_EQ (entries.front().level, LogLevel::Trace);
    EXPECT_EQ (entries.front().message, "t");
}

TEST_F (LoggingTest, DebugIsForwarded)
{
    log_message (LogLevel::Debug, "d");
    auto entries = get_mock_logger()->get_entries();
    ASSERT_EQ (entries.size(), 1);
    EXPECT_EQ (entries.front().level, LogLevel::Debug);
    EXPECT_EQ (entries.front().message, "d");
}

TEST_F (LoggingTest, InfoIsForwarded)
{
    log_message (LogLevel::Info, "i");
    auto entries = get_mock_logger()->get_entries();
    ASSERT_EQ (entries.size(), 1);
    EXPECT_EQ (entries.front().level, LogLevel::Info);
    EXPECT_EQ (entries.front().message, "i");
}

TEST_F (LoggingTest, WarnIsForwarded)
{
    log_message (LogLevel::Warn, "w");
    auto entries = get_mock_logger()->get_entries();
    ASSERT_EQ (entries.size(), 1);
    EXPECT_EQ (entries.front().level, LogLevel::Warn);
    EXPECT_EQ (entries.front().message, "w");
}

TEST_F (LoggingTest, ErrorIsForwarded)
{
    log_message (LogLevel::Error, "e");
    auto entries = get_mock_logger()->get_entries();
    ASSERT_EQ (entries.size(), 1);
    EXPECT_EQ (entries.front().level, LogLevel::Error);
    EXPECT_EQ (entries.front().message, "e");
}

TEST_F (LoggingTest, CriticalIsForwarded)
{
    log_message (LogLevel::Critical, "c");
    auto entries = get_mock_logger()->get_entries();
    ASSERT_EQ (entries.size(), 1);
    EXPECT_EQ (entries.front().level, LogLevel::Critical);
    EXPECT_EQ (entries.front().message, "c");
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
