#include "parser/Lexer.h"

std::vector<std::string> Lexer::tokenise()
{
    if (input_.empty())
    {
        return {};
    }

    int start = 0;
    for (int i = 0; i < input_.size(); i++)
    {
        for (; input_[i] != ' ' && input_[i] != '\n';)
        {
            i++;
        }
        tokens_.push_back(input_.substr(start, i - start));
        start = i + 1;
    }

    return {};
}