#include "parser/Lexer.h"

#include <iostream>

int main()
{
    Lexer lexer("{ 1, 2, 3 }");

    for (auto &&i : lexer.getTokens())
    {
        std::cout<< i << std::endl;
    }
    
    return 0;
}