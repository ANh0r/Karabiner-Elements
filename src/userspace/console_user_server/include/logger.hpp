#pragma once

#include "constants.hpp"
#include "filesystem.hpp"
#include <memory>
#include <spdlog/spdlog.h>
#include <string>

class logger final {
public:
  static spdlog::logger& get_logger(void) {
    static std::shared_ptr<spdlog::logger> logger;
    if (!logger) {
      if (auto p = constants::get_home_dot_karabiner_directory()) {
        std::string log_directory = p;
        mkdir(log_directory.c_str(), 0700);

        log_directory += "/log/";
        mkdir(log_directory.c_str(), 0700);

        static char hostname[_POSIX_HOST_NAME_MAX + 1];
        if (gethostname(hostname, sizeof(hostname)) == 0) {
          log_directory += hostname;
          mkdir(log_directory.c_str(), 0700);

          if (filesystem::is_directory(log_directory)) {
            std::string log_file_path = log_directory + "/karabiner_console_user_server_log";
            logger = spdlog::rotating_logger_mt("karabiner_console_user_server", log_file_path.c_str(), 5 * 1024 * 1024, 3, true);
          }
        }
      }

      if (!logger) {
        // fallback
        logger = spdlog::stdout_logger_mt("karabiner_console_user_server", true);
      }
    }
    return *logger;
  }
};
