
//-------GENERATED FILE-------
#import Post.h
@implementation Post : NSObject
const NSString * baseURL = http://localhost:3000;
+(NSURLSessionTask * ) getBasicTaskWithStringURL:(NSString *) stringURL
        completionHandler:(void (^)(NSData * __nullable data, NSURLResponse * __nullable response, NSError * __nullable error))completionHandler{
    NSURL* url = [NSURL URLWithString:[stringURL stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLQueryAllowedCharacterSet]]];
    NSMutableURLRequest *urlRequest = [NSMutableURLRequest requestWithURL:url];
    [urlRequest setHTTPMethod:@"POST"];


    NSURLSessionTask *task = [[NSURLSession sharedSession] dataTaskWithRequest:urlRequest completionHandler:completionHandler];

    return task;
}

+(void) posthelloPostusername:(NSString *)username  andpass:(NSString *)pass andcool:(NSData *)cool andpic:(NSData *)pic andfileName:(NSString *)fileName andtheDict:(NSDictionary*)theDict andtheArray:(NSArray *)theArraywithComp:(void (^)(BOOL, id))onCompletion{
	NSString * route = [NSString stringWithFormat:@"http://localhost:3000/helloPost/%@",username];
	NSSDictionary * dict = 
		@{
			{@"cool"                 : cool           },
			{@"pic"                  : pic            },
			{@"fileName"             : fileName       },
			{@"theDict"              : theDict        },
			{@"theArray"             : theArray       }
		};


	[Post postHTTP:route andParams:dict andCompletion:onCompletion];


}
+(void)postHTTP:(NSString *)actualURL
      andParams:(NSDictionary *) params
  andCompletion:(void(^)(BOOL,id)) onCompletion{
    
    
    // the boundary string : a random string, that will not repeat in post data, to separate post data fields.
    NSString *BoundaryConstant = @"----------V2ymHFg03ehbqgZCaKO6jy";
    
    // string constant for the post parameter contentFile
   // NSString* FileParamConstant = @"contentFile";
    
    // the server url to which the image (or the media) is uploaded. Use your server url here
    NSURL* requestURL = [NSURL URLWithString:[actualURL stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLQueryAllowedCharacterSet]]];
    
    // create request
    NSMutableURLRequest *request = [[NSMutableURLRequest alloc] init];
    [request setCachePolicy:NSURLRequestReloadIgnoringLocalCacheData];
    [request setHTTPShouldHandleCookies:NO];
    [request setTimeoutInterval:30];
    [request setHTTPMethod:@"POST"];
    
    // set Content-Type in HTTP header
    NSString *contentType = [NSString stringWithFormat:@"multipart/form-data; boundary=%@", BoundaryConstant];
    [request setValue:contentType forHTTPHeaderField: @"Content-Type"];
    
    // post body
    NSMutableData *body = [NSMutableData data];
    
    for (NSString *param in params) {
        [body appendData:[[NSString stringWithFormat:@"--%@\r\n", BoundaryConstant] dataUsingEncoding:NSUTF8StringEncoding]];
        [body appendData:[[NSString stringWithFormat:@"Content-Disposition: form-data; name=\"%@\"\r\n\r\n", param] dataUsingEncoding:NSUTF8StringEncoding]];
        // convert to json
        NSError * err= nil;
        NSData * data = [NSJSONSerialization dataWithJSONObject:[params objectForKey:param] options:0 error:&err];
        NSString * toAppend = [[NSString alloc] initWithBytes:[data bytes] length:[data length] encoding:NSUTF8StringEncoding];
        if(err){
            NSLog(@"Error converting into json");
            // sending empty data.. whatvs
            [body appendData:[[NSString stringWithFormat:@"\r\n"] dataUsingEncoding:NSUTF8StringEncoding]];
            
        }else{
            [body appendData:[[NSString stringWithFormat:@"%@\r\n", toAppend] dataUsingEncoding:NSUTF8StringEncoding]];
        }
    }
    
    [body appendData:[[NSString stringWithFormat:@"--%@--\r\n", BoundaryConstant] dataUsingEncoding:NSUTF8StringEncoding]];

    
    // setting the body of the post to the reqeust
    [request setHTTPBody:body];
    
    
    // set the content-length
    NSString *postLength = [NSString stringWithFormat:@"%d", ( int)[body length]];
    [request setValue:postLength forHTTPHeaderField:@"Content-Length"];
    
    // set URL
    [request setURL:requestURL];
    
    NSURLSessionTask *task = [[NSURLSession sharedSession] dataTaskWithRequest:request
                                                             completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
                                                                 // check error and/or handle response here
                                                                 BasicHandler  * handler = [[BasicHandler  alloc]init];
                                                                 if (error) {
                                                                     [handler acceptURL:nil
                                                                               withData:nil
                                                                              withBlock:onCompletion];
                                                                 } else {
                                                                     [handler acceptURL:response
                                                                               withData:data
                                                                              withBlock:onCompletion];
                                                                 }
                                                             }];
    
    [task resume];
    
    
    
}

@end // Post.m
