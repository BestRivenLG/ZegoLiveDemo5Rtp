// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from auto_config.djinni

#pragma once

#include <functional>

namespace zego {

enum class RequestMethod : int {
    /** post */
    POST,
    /** get */
    GET,
};

}  // namespace zego

namespace std {

template <>
struct hash<::zego::RequestMethod> {
    size_t operator()(::zego::RequestMethod type) const {
        return std::hash<int>()(static_cast<int>(type));
    }
};

}  // namespace std
