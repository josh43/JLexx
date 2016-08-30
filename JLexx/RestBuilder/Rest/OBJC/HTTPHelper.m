//
// Created by joshua on 7/20/16.
// Copyright (c) 2016 joshua. All rights reserved.
//

#import "HTTPHelper.h"

#define debugging 1
#define printHTTPMsg(x) if(debugging){NSLog(x);}


@implementation DictionaryResponseHandler
-(void)acceptURL:(NSURLResponse *)response
        withData:(NSData *)data withBlock:(void (^)(BOOL, id))onCompletion{
    if(response == nil){
        if(debugging) {
            NSLog(@"respone was nil %d %s\n", __LINE__, __FILE__);
        }
        onCompletion(NO,nil);
    }else if(data == nil){
        if(debugging) {
            NSLog(@"data was nil %d %s\n", __LINE__, __FILE__);
        }
        onCompletion(NO,nil);
    }else{
        // It's not always NSDictionary data and this could pass back an NSArray although it casts it to a dictionary
        NSDictionary * jsonData = [HTTPHelper URLToDict:response andData:data];
        if(debugging){NSLog(@"jsonData : %@",jsonData);}
        if(!jsonData){
            onCompletion(NO, nil);
        }
        else if([jsonData isKindOfClass:[NSDictionary class]]){
            if(((NSDictionary *)jsonData)[@"Error"]){
                NSLog(@"error accepting the data %@",jsonData);
                onCompletion(NO,nil);
            }else {
                onCompletion(YES, jsonData);
            }
        }else{
            onCompletion(NO, nil);

        }


    }
}
@end

@implementation ArrayResponseHandler
-(void)acceptURL:(NSURLResponse *)response
        withData:(NSData *)data withBlock:(void (^)(BOOL, id))onCompletion{
    if(response == nil){
        if(debugging) {
            NSLog(@"respone was nil %d %s\n", __LINE__, __FILE__);
        }
        onCompletion(NO,nil);
    }else if(data == nil){
        if(debugging) {
            NSLog(@"data was nil %d %s\n", __LINE__, __FILE__);
        }
        onCompletion(NO,nil);
    }else{
        // It's not always NSDictionary data and this could pass back an NSArray although it casts it to a dictionary
        NSDictionary * jsonData = [HTTPHelper URLToDict:response andData:data];
        if(debugging){NSLog(@"jsonData : %@",jsonData);}
        if(!jsonData){
            onCompletion(NO, nil);
        }
        else if([jsonData isKindOfClass:[NSDictionary class]]){
            if(((NSDictionary *)jsonData)[@"Error"]){
                NSLog(@"error accepting the data %@",jsonData);
                onCompletion(NO,nil);
            }else {
                NSArray * toPass = jsonData[@"Success"];
                if(toPass == nil || ![toPass isKindOfClass:[NSArray class]]){
                    onCompletion(NO,nil);
                }else{
                    onCompletion(YES, toPass);
                }
            }
        }else{
            onCompletion(NO, nil);

        }


    }
}
@end

@implementation HTTPHelper





+(NSDictionary *)URLToDict:(NSURLResponse *)resp
                   andData:(NSData *)data {

    NSError *err = nil;
    id toReturn;
    if (resp == nil) {
        return nil;
    } else {
        toReturn = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&err];
        if (err) {
            return nil;
        } else {
            return toReturn;
        }
    }
}
+ (NSArray *)URLToArray:(NSURLResponse *)resp
                andData:(NSData *)data {
    NSError *err = nil;
    NSArray *arr;
    if (resp == nil) {
        return nil;
    } else {
        arr = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&err];
        if (err) {
            // say thiers an err
            return nil;
        } else {
            return arr;
        }
    }
}


+ (NSDate *)stringToDate:(NSString *)str {

    if([str isEqualToString:@""]){
        return nil;
    }else{
        //ZZZZ is time offset


        NSDateFormatter  * formatterThree = [[NSDateFormatter alloc]init];
        formatterThree.dateFormat = @"yyyy-MM-dd'T'HH:mm:ss.SSSZ";

        NSDate *  d3 = [formatterThree dateFromString:str];
        return d3;
    }

}


+ (NSString *)dateToString:(NSDate *)date {
    //ZZZZ is time offset
    NSDateFormatter  * formatterThree = [[NSDateFormatter alloc]init];
    formatterThree.dateFormat = @"yyyy-MM-dd'T'HH:mm:ss.SSSZ";
    NSString  *  d3 = [formatterThree stringFromDate:date];
    return d3;


}
@end