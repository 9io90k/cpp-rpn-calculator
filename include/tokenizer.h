#pragma once
#include <string>
#include <vector>

namespace calc
{
    enum class TokenType { Number, Operator, UnaryMinus, LParen, RParen};

    struct Token
    {
        TokenType type;
        std::string lexeme;
        double value;
        std::size_t pos;
    };


    Token readNumber(const std::string& input, std::size_t& start);
    std::vector<Token> tokenize(const std::string& input);
}
