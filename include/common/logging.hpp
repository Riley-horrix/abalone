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

#include "common/utils.hpp"

#define error(fmt, ...) (fprintf(stderr, "\e[31m[ERROR]\e[0m " fmt "!\n" , ##__VA_ARGS__ ), fflush(stderr), raise(SIGTERM))
#define warn(fmt, ...) fprintf(stdout, "\e[93m[WARN]\e[0m " fmt "!\n" , ##__VA_ARGS__ )
#define info(fmt, ...) fprintf(stdout, "\e[32m[info]\e[0m " fmt ".\n" , ##__VA_ARGS__ )
#define prompt(fmt, ...) fprintf(stdout, "\e[96m" fmt "\e[0m" , ##__VA_ARGS__)

#ifdef DEBUG
#define debug(fmt, ...) fprintf(stdout, "\e[31m[WARN]\e[0m " fmt "!\n" , ##__VA_ARGS__ )
#endif