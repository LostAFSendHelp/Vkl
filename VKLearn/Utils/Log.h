#pragma once
#include<memory>
#include<spdlog/spdlog.h>

namespace Vkl {
	enum class LogLevel: char {
		LOG_LEVEL_INFO,
		LOG_LEVEL_WARNING,
		LOG_LEVEL_ERROR,
	};

	class Log final {
		using SpdLogLevel = spdlog::level::level_enum;

	public:
		static const Log* getInstance();
		static bool initInstance(LogLevel logLevel = LogLevel::LOG_LEVEL_INFO);
		static const Log* setInstance(Log* instance);
		static bool destroyInstance();

		Log(LogLevel logLevel = LogLevel::LOG_LEVEL_INFO) noexcept;

		template<class ... Args>
		void info(const char* baseString, Args&&... args) const;

		template<class ... Args>
		void warning(const char* baseString, Args&&... args) const;

		template<class ... Args>
		void error(const char* baseString, Args&&... args) const;

	private:
		static const Log* _instance;
		static const char* _LOGGER_NAME;
		static SpdLogLevel getSpdLogLevel(LogLevel logLevel);

		const LogLevel _logLevel;
		const std::shared_ptr<spdlog::logger> _logger;
	};
};

template<class ... Args>
void Vkl::Log::info(const char* baseString, Args&&... args) const {
	_logger->info(baseString, args...);
}

template<class ... Args>
void Vkl::Log::warning(const char* baseString, Args&&... args) const {
	_logger->warn(baseString, args...);
}

template<class ... Args>
void Vkl::Log::error(const char* baseString, Args&&... args) const {
	_logger->error(baseString, args...);
}

#define VKL_INFO(...) Vkl::Log::getInstance()->info(__VA_ARGS__)
#define VKL_WARN(...) Vkl::Log::getInstance()->warning(__VA_ARGS__)
#define VKL_ERR(...) Vkl::Log::getInstance()->error(__VA_ARGS__)