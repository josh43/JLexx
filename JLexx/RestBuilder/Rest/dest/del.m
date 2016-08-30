
//-------GENERATED FILE-------
#import Del.h
@implementation Del : NSObject
const NSString * baseURL = http://localhost:3000;
+(NSURLSessionTask * ) getBasicTaskWithStringURL:(NSString *) stringURL
        completionHandler:(void (^)(NSData * __nullable data, NSURLResponse * __nullable response, NSError * __nullable error))completionHandler{
    NSURL* url = [NSURL URLWithString:[stringURL stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLQueryAllowedCharacterSet]]];
    NSMutableURLRequest *urlRequest = [NSMutableURLRequest requestWithURL:url];
    [urlRequest setHTTPMethod:@"DELETE"];


    NSURLSessionTask *task = [[NSURLSession sharedSession] dataTaskWithRequest:urlRequest completionHandler:completionHandler];

    return task;
}

@end // Del.m
