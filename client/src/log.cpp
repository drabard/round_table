#include "log.h"

#include <unordered_set>

std::unordered_set<uint32_t> enabled_categories;
bool trace_enabled = false;
bool warning_enabled = false;
bool error_enabled = false;

void log_enable_all_categories() {
  for (uint32_t i = 0; i < LOG_NUM_CATEGORIES; ++i) {
    log_enable_category(i);
  }
}

void log_enable_category(uint32_t category) {
  enabled_categories.insert(category);
}

void log_disable_category(uint32_t category) {
  enabled_categories.erase(category);
}

void log_enable_trace() { trace_enabled = true; }

void log_disable_trace() { trace_enabled = false; }

void log_enable_warning() { warning_enabled = true; }

void log_disable_warning() { warning_enabled = false; }

void log_enable_error() { error_enabled = true; }

void log_disable_error() { error_enabled = false; }

void log_trace(uint32_t category, const char* fmt, ...) {
  if (!trace_enabled)
    return;
  if (enabled_categories.find(category) == enabled_categories.end())
    return;

  va_list argp;
  va_start(argp, fmt);
  vfprintf(stdout, fmt, argp);
  va_end(argp);
  fflush(stdout);
}

void log_warning(uint32_t category, const char* fmt, ...) {
  if (!warning_enabled)
    return;
  if (enabled_categories.find(category) == enabled_categories.end())
    return;

  va_list argp;
  va_start(argp, fmt);
  vfprintf(stdout, fmt, argp);
  va_end(argp);
  fflush(stdout);
}

void log_error(uint32_t category, const char* fmt, ...) {
  if (!error_enabled)
    return;
  if (enabled_categories.find(category) == enabled_categories.end())
    return;

  va_list argp;
  va_start(argp, fmt);
  vfprintf(stdout, fmt, argp);
  va_end(argp);
  fflush(stdout);
}