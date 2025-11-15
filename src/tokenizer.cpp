#include <tokenizer.h>
#include <cctype>
#include <stdexcept>

namespace calc
{
    static bool isSpace(char c) { return std::isspace(static_cast<unsigned char>(c)); }
    static bool isDigit(char c) { return std::isdigit(static_cast<unsigned char>(c)); }
    static bool isOperator(char c) { return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';}
    static bool isLeftParen(char c) { return c == '('; }
    static bool isRightParen(char c) { return c == ')'; }

    Token readNumber(const std::string& input, std::size_t& start)
    {
        size_t i = start;
        bool seenDot = false;
        while(i < input.size())
        {
            if(isDigit(input[i])) { ++i; continue;}
            if(input[i] == '.')
            {
                if(seenDot) { throw std::runtime_error ("Invalid number: extra dot");}
                seenDot = true;
                if(i > start && i + 1 < input.size() && isDigit(input[i + 1])) { ++i; continue; }
                else { throw std::runtime_error("Invalid number: dot without digit"); }
            }
            break;
        }
        std::string lexeme = input.substr(start, i - start);
        double value = std::stod(lexeme);
        Token t{TokenType::Number, lexeme, value, start};
        start = i;
        return t;
    }

    std::vector<Token> tokenize(const std::string& input)
    {
        std::vector<Token> tokens;
        for(std::size_t i = 0; i < input.size(); )
        {
            if(isSpace(input[i])) { ++i; continue; }
            if(isDigit(input[i])) 
            { 
                tokens.push_back(readNumber(input, i)); 
                continue;
            }
            if(isOperator(input[i]))
            {
                char c = input[i];
                bool unary = (c == '-') && (tokens.empty() 
                    || tokens.back().type == TokenType::Operator
                    || tokens.back().type == TokenType::LParen);
                if(unary)
                {
                    tokens.push_back(Token{TokenType::UnaryMinus, "-", 0.0, i});
                    ++i; continue;;
                }

                std::string lexeme(1, c);
                tokens.push_back(Token{TokenType::Operator, lexeme, 0.0, i});
                ++i; continue;
            }
            if(isLeftParen(input[i]))
            {
                tokens.push_back(Token{TokenType::LParen, "(", 0.0, i});
                ++i; continue;
            }
            if(isRightParen(input[i]))
            {
                tokens.push_back(Token{TokenType::RParen, ")", 0.0, i});
                ++i; continue;
            }
            throw std::runtime_error("Unknown symbol at pos" + std::to_string(i));
        }
        return tokens;
    }
}   
