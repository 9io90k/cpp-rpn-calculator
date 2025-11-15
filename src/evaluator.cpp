#include "evaluator.h"
#include <unordered_map>
#include <functional>
#include <stack>
#include <cmath>
#include <stdexcept>

namespace calc
{
    static double applyOp(double a, double b, char op)
    {
        static const std::unordered_map<char, std::function<double(double, double)>> ops = 
        {
            {'+', [] (double x, double y) { return x + y; }},
            {'-', [] (double x, double y) { return x - y; }},
            {'*', [] (double x, double y) { return x * y; }},
            {'/', [] (double x, double y) {if(y == 0) throw std::runtime_error("Division by zero"); return x/y; }},
            {'^', [] (double x, double y) { return std::pow(x, y); }}
        };
        auto it = ops.find(op);
        if(it == ops.end()) throw std::runtime_error(std::string("Unknown operator ") + op);
        return it->second(a,b);
    }

    double evaluate(const std::vector<Token>& rpn)
    {
        std::stack<double> st;
        for(const auto& token : rpn)
        {
            if(token.type == TokenType::Number)
            {
                st.push(token.value);
            }
            else if (token.type == TokenType::Operator)
            {
                if(st.size() < 2) throw std::runtime_error("Not enough operands");
                double b = st.top(); st.pop();
                double a = st.top(); st.pop();
                st.push(applyOp(a, b, token.lexeme[0]));
            }
            else if(token.type == TokenType::UnaryMinus)
            {
                if(st.empty()) throw std::runtime_error("Unary minus missing operand");
                double a = st.top(); st.pop();
                st.push(-a);
            }
        }
        if (st.size() != 1) throw std::runtime_error("Invalid expression");
        return st.top();
    }
}
