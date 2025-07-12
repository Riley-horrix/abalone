/**
 * @file messages.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief API messages definitions.
 * @version 0.1
 * @date 2025-07-12
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once

#include <string>

#include "common/logging.hpp"
#include "common/utils.hpp"

#include "lib/json.hpp"

#define ABALONE_API_VERSION_MAJOR 0
#define ABALONE_API_VERSION_MINOR 1
#define ABALONE_API_VERSION_PATCH 0
#define ABALONE_API_VERSION_INVALID -1

#define ABALONE_API_CONNECT_MAGIC "ABALONE"

namespace Abalone {

/**
 * @brief Struct to hold version information.
 */
struct APIVersion {
    /// @brief Major version number.
    int major;

    /// @brief Minor version number.
    int minor;
    
    /// @brief Patch version number.
    int patch;

    /**
     * @brief Construct a new APIVersion object from a version string.
     * 
     * @param version Version string in the form "%d.%d.%d".
     */
    APIVersion(const std::string& version);

    /**
     * @brief Transform the version object into a string.
     * 
     * @return std::string Version string in the form "%d.%d.%d".
     */
    std::string toString(void);

    /**
     * @brief Returns if the APIVersion is compatible with the current compiled version.
     * 
     * @return bool If the version is valid. 
     */
    bool compatible(void);
};

class APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 0;

    /**
     * @brief Convert the message to a JSON formatted string.
     * 
     * @return std::string The JSON formatted string.
     */
    virtual std::string toString() = 0;

    /**
     * @brief Construct an API message from a string.
     * 
     * @return APIMessage Unique pointer to the API message. Returns nullptr if the string is invalid.
     */
    static std::unique_ptr<APIMessage> fromString(const std::string& message);

    /**
     * @brief Get the Current Version object.
     * 
     * @return std::string 
     */
    static inline constexpr const char* getCurrentVersion(void) {
        return STR(ABALONE_API_VERSION_MAJOR) "." STR(ABALONE_API_VERSION_MINOR) "." STR(ABALONE_API_VERSION_PATCH);
    }

    /**
     * @brief Destroy the APIMessage object.
     */
    virtual ~APIMessage();

protected:
    APIMessage();

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    virtual void toJSON(nlohmann::json& json) = 0;
};

/**
 * @brief Message used to connect to a running Abalone instance.
 * 
 */
class APIConnect : public APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 1;

    /**
     * @brief Construct a new APIConnect object with a username.
     * 
     * @param name The player's name.
     */
    APIConnect(const std::string& name);

    /**
     * @brief Construct a new APIConnect object with a username and version number.
     * 
     * @param name The player's name.
     * @param version The message API version.
     */
    APIConnect(const std::string& name, const APIVersion& version);

    /**
     * @brief Convert the message to a JSON formatted string.
     * 
     * @return std::string The JSON formatted string.
     */
    std::string toString(void) override;

    /// @brief The version of the API being used.
    APIVersion version;

    /// @brief Server magic code.
    std::string magic;

    /// @brief The name of the user connecting.
    std::string name;

    /**
     * @brief Create the message from a JSON object.
     * 
     * Assumes that the "id" field is set and correct.
     * 
     * @param json The JSON object to read from.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    void toJSON(nlohmann::json& json) override;
};

}