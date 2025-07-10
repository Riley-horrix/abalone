/**
 * @file connection.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Defines a class to manage incoming game connections.
 * @version 0.1
 * @date 2025-07-01
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once

#include <string>

namespace Abalone {

class Connection {
    /**
     * @brief Construct a new Connection object on a fifo pipe.
     * 
     * @param fifoPipe The path to the fifo pipe.
     */
    Connection(const std::string& fifoPipe);
};

}