#pragma once
#include "tokenizer.h"

namespace calc
{
    constexpr int precedence(char op);
    constexpr bool isRightAssociative(char op);
    std::vector<Token> shunting_yard(const std::vector<Token>& tokens);
} 
