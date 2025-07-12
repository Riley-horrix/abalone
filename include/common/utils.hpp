/**
 * @file utils.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Utility function definitions.
 * @version 0.1
 * @date 2025-06-26
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once

#include <cstdint>

#include "common/logging.hpp"

#include "lib/json.hpp"

namespace Abalone {

#define STR(x) #x

/**
 * @brief Utility function object.
 */
class Utils {
public: 
    /**
     * @brief Count the number of bits set in a given number.
     * 
     * @param number The value to count the number of bits set.
     * @return int The number of bits set.
     */
    static int bitCount(uint64_t number);

    //! @copydoc Utils::bitCount(uint64_t)
    static int bitCount(uint32_t number);

    //! @copydoc Utils::bitCount(uint64_t)
    static int bitCount(uint16_t number);

    //! @copydoc Utils::bitCount(uint64_t)
    static int bitCount(uint8_t number);

    /**
     * @brief Get the current time since unix epoch in microseconds.
     * 
     * @return uint64_t Microseconds since epoch.
     */
    static uint64_t micros();

    /**
     * @brief Return a value with the bit set at the given parameter.
     * 
     * @param n Number to bitshift.
     */
    template<class T>
    static inline T bit(int n) { return static_cast<T>(0x1) << n; }

    template<class T>
    static inline int sign(T val) { return val > 0 ? 1 : val < 0 ? -1 : 0; }

    /**
     * @brief Helper function to extract a field from a json object.
     * 
     * @tparam T The type of the field.
     * @param json The json object.
     * @param key The string key.
     * @param defaultValue A default value.
     * @return T The found object or the default value;
     */
    template<typename T>
    static T getOrDefault(const nlohmann::json& json, const std::string& key, const T& defaultValue) {
        if (json.contains(key) && json[key].is_null() == false) {
            try {
                return json[key].get<T>();
            } catch (const nlohmann::json::exception& e) {
                info("APIMessage: Failed to parse %s, \"%s\"", key.c_str(), e.what());
                return defaultValue;
            }
        }
        return defaultValue;
    }
};

}