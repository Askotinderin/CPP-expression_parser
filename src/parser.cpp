#include "parser.hpp"


Parser::Parser () {
    this->t_stack = std::stack<Token>();
    this->f_stack = std::stack<float>();
    t_stack.push(Token(TK::EOE, "$"));
}

void Parser::operator() (std::queue<Token>& tokens) {
    while (true) {
        switch (operator_precedence[t_stack.top().table_id][tokens.front().table_id]) {
            case '<' :
                t_stack.push(tokens.front());
                tokens.pop();
                continue;
            case '>' :
                t_stack.top().reduce(f_stack);
                t_stack.pop();
                continue;
            case 'x' :
                std::cerr << "Invalid sequence" << std::endl;
                exit(-1);
                break;
            case '=' :
                t_stack.pop();
                tokens.pop();
                continue;
            case 'a' :
                std::cout << "result: "<< f_stack.top() << std::endl;
                break;
            default :
                std::cerr << "This should never happen" << std::endl;
                exit(-1);
        }
        break;
    }
}