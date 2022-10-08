#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Vkl {
	const char* Log::_LOGGER_NAME = "VKL";
	const Log* Log::_instance = nullptr;

	const Log* Log::getInstance() {
		return _instance;
	}

	bool Log::initInstance(LogLevel logLevel) {
		if (_instance) {
			return false;
		}

		_instance = new Log(logLevel);
		return true;
	}

	const Log* Log::setInstance(Log* instance) {
		_instance = instance;
		return _instance;
	}

	bool Log::destroyInstance() {
		if (!_instance) {
			return false;
		}

		VKL_INFO("Log destroyed");
		delete _instance;
		return true;
	}

	Log::Log(LogLevel logLevel) noexcept :
		_logLevel(logLevel),
		_logger(spdlog::stdout_color_st(_LOGGER_NAME))
	{
		_logger->set_level(getSpdLogLevel(_logLevel));
		info("Log initialized");
	}

	Log::SpdLogLevel Log::getSpdLogLevel(LogLevel logLevel) {
		switch (logLevel) {
		case LogLevel::LOG_LEVEL_INFO: return Log::SpdLogLevel::info;
		case LogLevel::LOG_LEVEL_WARNING: return Log::SpdLogLevel::warn;
		case LogLevel::LOG_LEVEL_ERROR: return Log::SpdLogLevel::err;
		default: return Log::SpdLogLevel::info;
		}
	}
};