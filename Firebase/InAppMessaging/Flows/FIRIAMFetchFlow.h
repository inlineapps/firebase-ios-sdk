/*
 * Copyright 2017 Google
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import <Foundation/Foundation.h>

#import "FIRIAMActivityLogger.h"
#import "FIRIAMBookKeeper.h"
#import "FIRIAMMessageClientCache.h"
#import "FIRIAMSDKModeManager.h"
#import "FIRIAMTimeFetcher.h"

@protocol FIRIAMAnalyticsEventLogger;

NS_ASSUME_NONNULL_BEGIN
@interface FIRIAMFetchSetting : NSObject
@property(nonatomic) NSTimeInterval fetchMinIntervalInMinutes;
@end

typedef void (^FIRIAMFetchMessageCompletionHandler)(
    NSArray<FIRIAMMessageDefinition *> *_Nullable messages,
    NSNumber *_Nullable nextFetchWaitTime,
    NSInteger discardedMessageCount,
    NSError *_Nullable error);

@protocol FIRIAMMessageFetcher
- (void)fetchMessagesWithImpressionList:(NSArray<FIRIAMImpressionRecord *> *)impressonList
                         withCompletion:(FIRIAMFetchMessageCompletionHandler)completion;
@end

// Parent class for supporting different fetching flows. Subclass is supposed to trigger
// checkAndFetch at appropriate moments based on its fetch strategy
@interface FIRIAMFetchFlow : NSObject
- (instancetype)initWithSetting:(FIRIAMFetchSetting *)setting
                   messageCache:(FIRIAMMessageClientCache *)cache
                 messageFetcher:(id<FIRIAMMessageFetcher>)messageFetcher
                    timeFetcher:(id<FIRIAMTimeFetcher>)timeFetcher
                     bookKeeper:(id<FIRIAMBookKeeper>)displayBookKeeper
                 activityLogger:(FIRIAMActivityLogger *)activityLogger
           analyticsEventLogger:(id<FIRIAMAnalyticsEventLogger>)analyticsEventLogger
           FIRIAMSDKModeManager:(FIRIAMSDKModeManager *)sdkModeManager;

// Triggers a potential fetch of in-app messaging from the source. It would check and respect the
// the fetchMinIntervalInMinutes defined in setting
- (void)checkAndFetch;
@end
NS_ASSUME_NONNULL_END
