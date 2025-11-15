#include <parser.h>
#include <tokenizer.h>
#include <stdexcept>
#include <stack>
#include <vector>
namespace calc
{
    constexpr int precedence(char op)
    {
        switch (op)
        {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^' : return 3;
        default: return 0;
        }
    }

    constexpr bool isRightAssociative(char op) { return op == '^';}

    std::vector<Token> shunting_yard(const std::vector<Token>& tokens)
    {
        std::stack<Token> ops;
        std::vector<Token> out;

        for(const auto& token : tokens)
        {
            if(token.type == TokenType::Number)
            {
                out.push_back(token);
                if(!ops.empty() && ops.top().type == TokenType::UnaryMinus)
                {
                    out.push_back(ops.top());
                    ops.pop();
                }
            }
            else if(token.type == TokenType::UnaryMinus)
            {
                ops.push(token);
            }
            else if(token.type == TokenType::Operator)
            {
                while (!ops.empty() &&
                    ops.top().type == TokenType::Operator &&
                    (
                        (!isRightAssociative(token.lexeme[0]) &&
                            precedence(token.lexeme[0]) <= precedence(ops.top().lexeme[0])) ||
                        (isRightAssociative(token.lexeme[0]) &&
                            precedence(token.lexeme[0]) < precedence(ops.top().lexeme[0]))
                    ))
                {
                    out.push_back(ops.top());
                    ops.pop();
                }
                ops.push(token);
            }
            else if(token.type == TokenType::LParen)
            {
                ops.push(token);
            }
            else if(token.type == TokenType::RParen)
            {
                while(!ops.empty() && ops.top().type != TokenType::LParen)
                {
                    out.push_back(ops.top());
                    ops.pop();
                }
                if(ops.empty()) { throw std::runtime_error ("Mismatched parentheses"); }
                ops.pop();
                if(!ops.empty() && ops.top().type == TokenType::UnaryMinus)
                {
                    out.push_back(ops.top());
                    ops.pop();
                }
            }
        }
        while (!ops.empty()) 
        {
            if (ops.top().type == TokenType::LParen || ops.top().type == TokenType::RParen) {
                throw std::runtime_error("Mismatched parentheses");
            }
            out.push_back(ops.top());
            ops.pop();
        }
        return out;
    }
} 
