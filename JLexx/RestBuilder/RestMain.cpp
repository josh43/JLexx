//
// Created by joshua on 8/26/16.
//




#include "RestBuilder.h"

int main(){


    Rest::RestGenerator rg;
    Rest::RestBuilder b(rg,"/Users/josh/ClionProjects/JLexx/JLexx/RestBuilder/restEx.rest");

    rg.generateCode();
}