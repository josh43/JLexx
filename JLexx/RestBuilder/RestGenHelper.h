//
// Created by joshua on 8/25/16.
//

#ifndef JLEXX_RESTGENHELPER_H
#define JLEXX_RESTGENHELPER_H


string getObjCType(std::string text){
    static map<std::string,std::string> theMap =
            {
                    {"file"        , "NSData *"},
                    {"string"      , "NSString *"},
                    {"dictionary"  , "NSDictionary*"},
                    {"array"       , "NSArray *"},


            };
    if(theMap.find(text) == theMap.end()){
        printf("Error could not find %s when trying to getObjcType\n",text.c_str());
        throw std::invalid_argument("");
    }

    return theMap[text];
}
/*
 * I am generating random bytes here we g ooooo 234235-90
 * 'a;lsdlkflldksfl;dsfl oipwefpo oiasdpo kAP;LSD ;LSDLKJFASDF
 * SALDF;KSA;LDFKASL;DF SDF9;FWKELFK WEF WK;LK2398SAF ASL;KDLASD
 * ASDLK; ASD;LKASD
 *
 *
 *
 *
 *
 *
 *
 *
 * \R\N
 * asld;l;aksdlklaksdl;k
 * as;ldk;lasd
 * l340923849852650-98
 * `
 * `
 * `asdk;lk
 *
 *
 */
void setFiles(string verb,unsigned int & jsFileNo,unsigned int &objFileNo){
    switch(verb[0]){
        case'g':{
            jsFileNo = 0;
            objFileNo = 4;
            break;
        }case 'p':{
            if(verb[1] == 'o'){
                jsFileNo = 2;
                objFileNo = 6;
            }else{
                //1
                jsFileNo = 1;
                objFileNo =5;
            }
            break;
        }case 'd':{
            jsFileNo = 3;
            objFileNo = 7;
            break;
        }

        default:
            throw std::invalid_argument("Unrecgonized verb(aka not a get put post or delete\n");
    }
}


const string basicGet =
        "+(NSURLSessionTask * ) getBasicTaskWithStringURL:(NSString *) stringURL\n"
                "        completionHandler:(void (^)(NSData * __nullable data, NSURLResponse * __nullable response, NSError * __nullable error))completionHandler{\n"
                "    NSURL* url = [NSURL URLWithString:[stringURL stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLQueryAllowedCharacterSet]]];\n"
                "    NSMutableURLRequest *urlRequest = [NSMutableURLRequest requestWithURL:url];\n"
                "    [urlRequest setHTTPMethod:@\"GET\"];\n"
                "\n"
                "\n"
                "    NSURLSessionTask *task = [[NSURLSession sharedSession] dataTaskWithRequest:urlRequest completionHandler:completionHandler];\n"
                "\n"
                "    return task;\n"
                "}";
// Below is an example of really bad lazy coding

const string basicPut =
        "+(NSURLSessionTask * ) getBasicTaskWithStringURL:(NSString *) stringURL\n"
                "        completionHandler:(void (^)(NSData * __nullable data, NSURLResponse * __nullable response, NSError * __nullable error))completionHandler{\n"
                "    NSURL* url = [NSURL URLWithString:[stringURL stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLQueryAllowedCharacterSet]]];\n"
                "    NSMutableURLRequest *urlRequest = [NSMutableURLRequest requestWithURL:url];\n"
                "    [urlRequest setHTTPMethod:@\"PUT\"];\n"
                "\n"
                "\n"
                "    NSURLSessionTask *task = [[NSURLSession sharedSession] dataTaskWithRequest:urlRequest completionHandler:completionHandler];\n"
                "\n"
                "    return task;\n"
                "}";
const string basicPost =
        "+(NSURLSessionTask * ) getBasicTaskWithStringURL:(NSString *) stringURL\n"
                "        completionHandler:(void (^)(NSData * __nullable data, NSURLResponse * __nullable response, NSError * __nullable error))completionHandler{\n"
                "    NSURL* url = [NSURL URLWithString:[stringURL stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLQueryAllowedCharacterSet]]];\n"
                "    NSMutableURLRequest *urlRequest = [NSMutableURLRequest requestWithURL:url];\n"
                "    [urlRequest setHTTPMethod:@\"POST\"];\n"
                "\n"
                "\n"
                "    NSURLSessionTask *task = [[NSURLSession sharedSession] dataTaskWithRequest:urlRequest completionHandler:completionHandler];\n"
                "\n"
                "    return task;\n"
                "}";
const string basicDelete =
        "+(NSURLSessionTask * ) getBasicTaskWithStringURL:(NSString *) stringURL\n"
                "        completionHandler:(void (^)(NSData * __nullable data, NSURLResponse * __nullable response, NSError * __nullable error))completionHandler{\n"
                "    NSURL* url = [NSURL URLWithString:[stringURL stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLQueryAllowedCharacterSet]]];\n"
                "    NSMutableURLRequest *urlRequest = [NSMutableURLRequest requestWithURL:url];\n"
                "    [urlRequest setHTTPMethod:@\"DELETE\"];\n"
                "\n"
                "\n"
                "    NSURLSessionTask *task = [[NSURLSession sharedSession] dataTaskWithRequest:urlRequest completionHandler:completionHandler];\n"
                "\n"
                "    return task;\n"
                "}";

const string advancedPost =
        "+(void)postHTTP:(NSString *)actualURL\n"
        "      andParams:(NSDictionary *) params\n"
        "  andCompletion:(void(^)(BOOL,id)) onCompletion{\n"
        "    \n"
        "    \n"
        "    // the boundary string : a random string, that will not repeat in post data, to separate post data fields.\n"
        "    NSString *BoundaryConstant = @\"----------V2ymHFg03ehbqgZCaKO6jy\";\n"
        "    \n"
        "    // string constant for the post parameter contentFile\n"
        "   // NSString* FileParamConstant = @\"contentFile\";\n"
        "    \n"
        "    // the server url to which the image (or the media) is uploaded. Use your server url here\n"
        "    NSURL* requestURL = [NSURL URLWithString:[actualURL stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLQueryAllowedCharacterSet]]];\n"
        "    \n"
        "    // create request\n"
        "    NSMutableURLRequest *request = [[NSMutableURLRequest alloc] init];\n"
        "    [request setCachePolicy:NSURLRequestReloadIgnoringLocalCacheData];\n"
        "    [request setHTTPShouldHandleCookies:NO];\n"
        "    [request setTimeoutInterval:30];\n"
        "    [request setHTTPMethod:@\"POST\"];\n"
        "    \n"
        "    // set Content-Type in HTTP header\n"
        "    NSString *contentType = [NSString stringWithFormat:@\"multipart/form-data; boundary=%@\", BoundaryConstant];\n"
        "    [request setValue:contentType forHTTPHeaderField: @\"Content-Type\"];\n"
        "    \n"
        "    // post body\n"
        "    NSMutableData *body = [NSMutableData data];\n"
        "    \n"
        "    for (NSString *param in params) {\n"
        "        [body appendData:[[NSString stringWithFormat:@\"--%@\\r\\n\", BoundaryConstant] dataUsingEncoding:NSUTF8StringEncoding]];\n"
        "        [body appendData:[[NSString stringWithFormat:@\"Content-Disposition: form-data; name=\\\"%@\\\"\\r\\n\\r\\n\", param] dataUsingEncoding:NSUTF8StringEncoding]];\n"
        "        // convert to json\n"
        "        NSError * err= nil;\n"
        "        NSData * data = [NSJSONSerialization dataWithJSONObject:[params objectForKey:param] options:0 error:&err];\n"
        "        NSString * toAppend = [[NSString alloc] initWithBytes:[data bytes] length:[data length] encoding:NSUTF8StringEncoding];\n"
        "        if(err){\n"
        "            NSLog(@\"Error converting into json\");\n"
        "            // sending empty data.. whatvs\n"
        "            [body appendData:[[NSString stringWithFormat:@\"\\r\\n\"] dataUsingEncoding:NSUTF8StringEncoding]];\n"
        "            \n"
        "        }else{\n"
        "            [body appendData:[[NSString stringWithFormat:@\"%@\\r\\n\", toAppend] dataUsingEncoding:NSUTF8StringEncoding]];\n"
        "        }\n"
        "    }\n"
        "    \n"
        "    [body appendData:[[NSString stringWithFormat:@\"--%@--\\r\\n\", BoundaryConstant] dataUsingEncoding:NSUTF8StringEncoding]];\n"
        "\n"
        "    \n"
        "    // setting the body of the post to the reqeust\n"
        "    [request setHTTPBody:body];\n"
        "    \n"
        "    \n"
        "    // set the content-length\n"
        "    NSString *postLength = [NSString stringWithFormat:@\"%d\", ( int)[body length]];\n"
        "    [request setValue:postLength forHTTPHeaderField:@\"Content-Length\"];\n"
        "    \n"
        "    // set URL\n"
        "    [request setURL:requestURL];\n"
        "    \n"
        "    NSURLSessionTask *task = [[NSURLSession sharedSession] dataTaskWithRequest:request\n"
        "                                                             completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {\n"
        "                                                                 // check error and/or handle response here\n"
        "                                                                 DictionaryResponseHandler  * handler = [[DictionaryResponseHandler  alloc]init];\n"
        "                                                                 if (error) {\n"
        "                                                                     [handler acceptURL:nil\n"
        "                                                                               withData:nil\n"
        "                                                                              withBlock:onCompletion];\n"
        "                                                                 } else {\n"
        "                                                                     [handler acceptURL:response\n"
        "                                                                               withData:data\n"
        "                                                                              withBlock:onCompletion];\n"
        "                                                                 }\n"
        "                                                             }];\n"
        "    \n"
        "    [task resume];\n"
        "    \n"
        "    \n"
        "    \n"
        "}";
#endif //JLEXX_RESTGENHELPER_H
