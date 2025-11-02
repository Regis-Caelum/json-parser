#include "Json/json.h"

#include <iostream>

int main()
{
    Json json;
    json["name"] = "test";

    std::cout << json["name"] << std::endl;
    return 0;
}