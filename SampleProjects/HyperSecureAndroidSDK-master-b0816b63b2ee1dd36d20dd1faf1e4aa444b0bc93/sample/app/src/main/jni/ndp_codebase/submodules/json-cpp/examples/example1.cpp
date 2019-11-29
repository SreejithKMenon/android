/* Copyright (C) HyperVerge, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <time.h>

#include "json/json.h"

using namespace std;

int main(int argc, char **argv)
{
    string response;

    Json::Reader reader;

    // Default precision : 16
    Json::FastWriter writer;

    // Custom precision : 4 decimal places
    Json::FastWriter writer_custom(4);

    /**** JSON Reading *******/
    Json::Value value;
    reader.parse("[ [\"1\", \"2\"], [\"3\", \"4\", \"5\"] ]", value);
    string created_string = writer.write(value);
    cout << "The created JSON is: " << created_string << endl;

    for(unsigned int i = 0; i < value.size(); i++)
    {
        cout << "Array " << i << endl;
        Json::Value arr = value[i];
        for(unsigned int j = 0; j < arr.size(); j++)
            cout << arr[j].asString() << " ";
        cout << endl;
    }
    cout << endl;

    /****** JSON Writing ******/
    // Writing with a custom JSON
    cout << "Creating array json" << endl;
    Json::Value arrayval(Json::arrayValue);
    float f1 = 0.1234;
    float f2 = 0.2345;
    arrayval.append(Json::Value(f1));
    arrayval.append(Json::Value(f2));

    cout << "The generated JSON with precision of 4 is " << writer_custom.write(arrayval) << endl;
    cout << "The generated JSON with default precision is " << writer.write(arrayval) << endl;

    cout << "Creating object json" << endl;
    Json::Value val(Json::objectValue);
    val["int-var1"] = 123;
    val["str-var2"] = "value2";
    cout << "The generated JSON is " << writer.write(val) << endl;

    return 0;
}
