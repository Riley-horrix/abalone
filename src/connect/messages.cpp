/**
 * @file messages.cpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief API messages implementation.
 * @version 0.1
 * @date 2025-07-12
 * 
 * Copyright (c) Riley Horrix 2025
 */
#include <sstream>
#include <string>
#include <format>

#include "connect/messages.hpp"
#include "common/utils.hpp"

#include "lib/json.hpp"

using namespace Abalone;

APIVersion::APIVersion(const std::string& version) {
    std::stringstream ss(version);
    char dot1, dot2;

    if (!(ss >> major >> dot1 >> minor >> dot2 >> patch && dot1 == '.' && dot2 == '.')) {
        major = ABALONE_API_VERSION_INVALID;
        minor = ABALONE_API_VERSION_INVALID;
        patch = ABALONE_API_VERSION_INVALID;
    }
}

std::string APIVersion::toString(void) {
    return std::format("{}.{}.{}", major, minor, patch);
}

bool APIVersion::compatible(void) {
    if (major != ABALONE_API_VERSION_MAJOR) { return false; }
    if (minor != ABALONE_API_VERSION_MINOR) {
        info("APIVersion: Compatable but different API version being used.");
    }

    return true;
}

// Static message id definitions
const int APIMessage::ID;
const int APIConnect::ID;

std::unique_ptr<APIMessage> APIMessage::fromString(const std::string& message) {
    nlohmann::json j;

    const auto json = j.parse(message);
    const int id = Utils::getOrDefault(json, "id", -1);

    if (id <= 0) { return nullptr; }

    switch (id) {
        case APIConnect::ID:
            return APIConnect::fromJSON(json);
    }

    return nullptr;
}

APIMessage::APIMessage() {}
APIMessage::~APIMessage() {}

APIConnect::APIConnect(const std::string& name): 
    version(APIMessage::getCurrentVersion()), magic(ABALONE_API_CONNECT_MAGIC), name(name) {}

APIConnect::APIConnect(const std::string& name, const APIVersion& version): 
    version(version), magic(ABALONE_API_CONNECT_MAGIC), name(name) {}

std::string APIConnect::toString() {
    nlohmann::json j;
    toJSON(j);
    return j.dump();
}

std::unique_ptr<APIMessage> APIConnect::fromJSON(const nlohmann::json& j) {
    if (!j.contains("content") && j["content"].is_object()) { return nullptr; }
    
    std::string name;
    if ((name = Utils::getOrDefault<std::string>(j, "name", "")) == "") { return nullptr; }
    std::string version;
    if ((version = Utils::getOrDefault<std::string>(j, "version", "")) == "") { return nullptr; }
    std::string magic;
    if ((magic = Utils::getOrDefault<std::string>(j, "magic", "")) == "") { return nullptr; }

    // Check magic
    if (magic != ABALONE_API_CONNECT_MAGIC) { return nullptr; }
    // Check version compatible
    APIVersion apiVersion(version);
    if (!apiVersion.compatible()) { return nullptr; }

    return std::make_unique<APIConnect>(name, apiVersion);
}

void APIConnect::toJSON(nlohmann::json& j) {
    j["id"] = APIConnect::ID;
    j["content"] = {
        {"version", version.toString()},
        {"magic", magic},
        {"name", name}
    };
}