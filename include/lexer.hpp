#pragma once

#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <array>
#include <sstream>
#include <functional>
#include <cmath>


#define OP_SIZE 14
#define UNUSED(expr) do { (void)(expr); } while (0)


extern std::unordered_map<std::string, float> variables;

bool is_number (std::string& str);
bool is_identifier (std::string& str);
bool is_keyword (std::string& str);

const std::unordered_map<std::string, int> op_to_id = {
    {"+", 2},
    {"*", 3},
    {"-", 4},
    {"/", 5},
    {"%", 6},
    {"(", 7},
    {")", 8},
    {"^", 9},
    {"&", 10},
    {"|", 11},
    {"=", 12},
    {";", 13},
};

typedef enum class TK : unsigned char{
    EOE,
    OPERATOR,
    NUMBER,
    KEYWORD,
    IDENTIFIER
}TokenKind;

const std::unordered_map<TokenKind, std::string> kind_to_str = {
    {TK::EOE,       "End of expression"},
    {TK::OPERATOR,  "Operator: "},
    {TK::NUMBER,    "Number: "},

};

struct Token {
    TokenKind kind;
    int table_id;
    float number;
    std::string name;

    Token(TokenKind kind, std::string content);
    Token(int id, std::string content);

    void reduce (std::stack<float>& operands);

};

namespace op {
    void push (std::stack<float>& operands,     Token* t);
    void add (std::stack<float>& operands,      Token* t);
    void mul (std::stack<float>& operands,      Token* t);
    void sub (std::stack<float>& operands,      Token* t);
    void div (std::stack<float>& operands,      Token* t);
    void mod (std::stack<float>& operands,      Token* t);
    void pow (std::stack<float>& operands,      Token* t);
    void min (std::stack<float>& operands,      Token* t);
    void max (std::stack<float>& operands,      Token* t);
}

const std::array<std::function<void(std::stack<float>&, Token*)>, OP_SIZE> functions  = {
    nullptr,
    op::push,
    op::add,
    op::mul,
    op::sub,
    op::div,
    op::mod,
    nullptr,
    nullptr,
    op::pow,
    op::min,
    op::max,
    nullptr,
    nullptr
};

class Lexer {
private:
    std::queue<Token> tokens;
    char c;
    std::string content;

    void lex_symbol(std::stringstream& file);
    void lex_number(std::stringstream& file);
    bool match_sequence (std::stringstream& file, std::string str);
public:
    Lexer ();
    std::queue<Token> operator() (std::stringstream& file);
};

std::ostream& operator<< (std::ostream& stream, const Token& t);