/**
 * @file logging.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Logging functionality definitions.
 * @version 0.1
 * @date 2025-06-26
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once

#include <cstdio>
#include <ctime>
#include <csignal>

#define error(fmt, ...) (fprintf(stderr, "\033[31m[ERROR]\033[0m " fmt "!\n" __VA_OPT__(,) __VA_ARGS__ ), fflush(stderr), raise(SIGTERM))
#define warn(fmt, ...) fprintf(stdout, "\033[93m[WARN]\033[0m " fmt "!\n" __VA_OPT__(,) __VA_ARGS__ )
#define info(fmt, ...) fprintf(stdout, "\033[32m[info]\033[0m " fmt ".\n" __VA_OPT__(,) __VA_ARGS__ )
#define prompt(fmt, ...) fprintf(stdout, "\033[96m" fmt "\033[0m" __VA_OPT__(,) __VA_ARGS__)

#define stl_error(code, fmt, ...) error(fmt ", cause %d: %s" __VA_OPT__(,) __VA_ARGS__, code, strerror(code))
#define stl_warn(code, fmt, ...) warn(fmt ", cause %d: %s" __VA_OPT__(,) __VA_ARGS__, code, strerror(code))

#ifdef DEBUG
#define debug(fmt, ...) fprintf(stdout, "\033[95m[debug]\033[0m " fmt "!\n" __VA_OPT__(,) __VA_ARGS__ )
#else
#define debug(fmt, ...)
#endif
