#include "main.hpp"


int main (int argc, char** argv) {

    for (int i = 0; i < argc; i++)
        std::cout << argv[i] << std::endl;
    std::cout << std::endl;

    std::string expr =  "2.5/3 + 7*5 -3/4 + 5%2 + 6*(4+2)^2^2 & 65 | 5\n"
                        "a = 5+ans\n"
                        "b = a+34;\n"
                        "a";
    std::cout << expr << std::endl;

    std::stringstream file(expr);

    Lexer lex;
    std::queue<Token> tokens = lex(file);
    Parser parse;
    parse(tokens);

    std::cout << "Program finished successfuly" << std::endl;

    return 0;
}