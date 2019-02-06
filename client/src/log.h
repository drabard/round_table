#pragma once

#include <stdint.h>

enum log_category {
  LOG_MAIN = 0,
  LOG_INPUT,
  LOG_NETWORK,
  LOG_RENDER,
  LOG_SCENE,
  LOG_NUM_CATEGORIES
};

void log_enable_all_categories();
void log_enable_category(uint32_t category);
void log_disable_category(uint32_t category);

void log_enable_trace();
void log_disable_trace();

void log_enable_warning();
void log_disable_warning();

void log_enable_error();
void log_disable_error();

void log_trace(uint32_t category, const char* fmt, ...);
void log_warning(uint32_t category, const char* fmt, ...);
void log_error(uint32_t category, const char* fmt, ...);