/**
 * @file messageStream.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief MessageStream class definition.
 * @version 0.1
 * @date 2025-07-12
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once

#include "connect/stream.hpp"
#include "connect/messages.hpp"

namespace Abalone {

class MessageStream : public Stream<std::unique_ptr<APIMessage>> {

};

}