// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from auto_config.djinni

#import "ZGRoomInfo.h"
#import <Foundation/Foundation.h>


@protocol ZGRoomListUpdateListener

- (void)onUpdateRoomList:(nonnull NSArray<ZGRoomInfo *> *)roomList;

- (void)onUpdateRoomListError;

@end
