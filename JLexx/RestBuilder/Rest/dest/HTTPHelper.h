//
// Created by joshua on 7/20/16.
// Copyright (c) 2016 joshua. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>


// THIS NEEDS TO BE DEFINED
const static NSString * baseURL = @"localhost";

@protocol NSURLResponseHandler
// I follow this style throughout the whole Rest package
// (althought don't actually use it because the methods are class methods)
// but it will pass YES and the data on success or NO,nil on fail!
- (void)acceptURL:(NSURLResponse *)response withData:(NSData *)data
        withBlock:(void (^)(BOOL, id))onCompletion;
@end

@interface DictionaryResponseHandler: NSObject<NSURLResponseHandler>
@end

@interface ArrayResponseHandler: NSObject<NSURLResponseHandler>
@end

@interface HTTPHelper : NSObject
+ (NSArray *)URLToArray:(NSURLResponse *)resp
        andData:(NSData *)data;
+ (NSDictionary *)URLToDict:(NSURLResponse *)resp
        andData:(NSData *)data;


// only use with DictionaryResponseHandler or ArraResponseHandler
#define basicHandler(type) \
type * basicHandler = [[type  alloc]init];\
if(error){\
    [basicHandler acceptURL:nil withData:nil withBlock:onCompletion];\
}else{\
[   basicHandler acceptURL:response withData:data withBlock:onCompletion];\
}





#define getValue(dict,key) if([dict objectForKey:key]){return dict[key];}else{return nil;}

+ (NSString *)dateToString:(NSDate *)date;
+ (NSDate *)stringToDate:(id)o;
@end