#include "parser.hpp"


Parser::Parser () {
    this->t_stack = std::stack<Token>();
    this->f_stack = std::stack<float>();
    t_stack.push(Token(TK::EOE, "$"));
}

void Parser::operator() (std::queue<Token>& tokens) {
    print = true;
    int i = 0;
    while (true) {
        switch (operator_precedence[t_stack.top().table_id][tokens.front().table_id]) {
            case '<' :
                t_stack.push(tokens.front());
                last_var = tokens.front().name;
                tokens.pop();
                i++;
                continue;
            case '>' :
                t_stack.top().reduce(f_stack);
                t_stack.pop();
                i++;
                continue;
            case 'x' :
                std::cerr << "Invalid sequence" << std::endl;
                exit(-1);
                break;
            case '=' :
                t_stack.pop();
                tokens.pop();
                continue;
            case 's' :
                t_stack.pop();
                if (print)
                    std::cout << t_stack.top().name << " = " << f_stack.top() << std::endl;
                print = true;
                variables.insert_or_assign(t_stack.top().name, f_stack.top());
                f_stack.pop();
                t_stack.pop();
                continue;
            case 'a' :
                if (!f_stack.empty()) {
                    if (i == 2) {
                        if (print)
                            std::cout << last_var << " = " << f_stack.top() << std::endl;
                        print = true;
                        f_stack.pop();
                    }
                    else {
                        if (print)
                            std::cout << "ans = " << f_stack.top() << std::endl;
                        print = true;
                        variables.insert_or_assign("ans", f_stack.top());
                        f_stack.pop();
                    }
                }
                i = 0;
                tokens.pop();
                std::cout << std::endl;
                if (tokens.empty())
                    break;
                continue;
            case 'p' :
                print = false;
                tokens.pop();
                continue;
            default :
                std::cerr << "This should never happen" << std::endl;
                exit(-1);
        }
        break;
    }
}