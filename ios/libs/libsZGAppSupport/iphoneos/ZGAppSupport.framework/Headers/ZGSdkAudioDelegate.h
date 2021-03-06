// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from auto_config.djinni

#import <Foundation/Foundation.h>


@protocol ZGSdkAudioDelegate

/** 设置音频码率 */
- (void)onSetAudioBitrate:(int32_t)bitrate;

/** 设置音频模式 */
- (void)onSetAudioDeviceMode:(int32_t)mode;

/** 回声消除开关 */
- (void)onEnableAec:(BOOL)enable;

/** 耳机插入状态下是否使用回声消除 */
- (void)onEnableAecwhenHeadsetDetected:(BOOL)enable;

/** 是否开启离散音频包发送 */
- (void)onEnableDtx:(BOOL)enable;

/** 音频采集自动增益开关 */
- (void)onEnableAgc:(BOOL)enable;

/** 设置音频通道数 */
- (void)onSetAudioChannelCount:(int32_t)channelCount;

@end
