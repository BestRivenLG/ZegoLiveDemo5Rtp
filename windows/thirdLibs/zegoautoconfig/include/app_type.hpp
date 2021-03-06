// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from auto_config.djinni

#pragma once

#include <functional>

namespace zego {

enum class AppType : int {
    /** 实时视频 */
    VIDEO_TALK,
    /** 实时语音 */
    AUDIO_LIVE,
    /** 在线直播 */
    LIVE_DEMO5,
};

}  // namespace zego

namespace std {

template <>
struct hash<::zego::AppType> {
    size_t operator()(::zego::AppType type) const {
        return std::hash<int>()(static_cast<int>(type));
    }
};

}  // namespace std
