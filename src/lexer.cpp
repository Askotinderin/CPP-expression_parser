#include "lexer.hpp"


std::unordered_map<std::string, float> variables;

bool is_number (std::string& str) {
    bool dot = false;
    for (const auto& c : str) {
        if (!(std::isdigit(c))) {
            if (c == '.' && !dot) {
                dot = true;
                continue;
            }
            return false;
        }
    }
    return true;
}

bool is_identifier (std::string& str) {
    for (const auto& c : str) {
        if (c == '_') continue;
        if (std::isalpha(c))
            return true;
        else
            return false;
    }
    return false;
}

bool is_keyword (std::string& str) {
    UNUSED(str);
    return false;
}


void op::push (std::stack<float>& operands, Token* t) {
    if (t->kind == TK::NUMBER) {
        operands.push(t->number);
    }
    else if (t->kind == TK::IDENTIFIER) {
        if (variables.count(t->name) == 1)
            operands.push(variables[t->name]);
        else {
            std::cout << "ERROR: undefined variable " << t->name << std::endl;
            exit(-1);
        }

    }
}
void op::add (std::stack<float>& operands, Token* t) {
    if (operands.empty()) {
        std::cout << "ERROR: stack is empty" << std::endl;
        exit(-1);
    }
    float temp = operands.top();
    operands.pop();
    std::cout << operands.top() << " + " << temp << " = ";
    operands.top() += temp;
    std::cout << operands.top() << std::endl;
    UNUSED(t);
}
void op::mul (std::stack<float>& operands, Token* t) {
    if (operands.empty()) {
        std::cout << "ERROR: stack is empty" << std::endl;
        exit(-1);
    }
    float temp = operands.top();
    operands.pop();
    std::cout << operands.top() << " * " << temp << " = ";
    operands.top() *= temp;
    std::cout << operands.top() << std::endl;
    UNUSED(t);
}
void op::sub (std::stack<float>& operands, Token* t) {
    if (operands.empty()) {
        std::cout << "ERROR: stack is empty" << std::endl;
        exit(-1);
    }
    float temp = operands.top();
    operands.pop();
    std::cout << operands.top() << " - " << temp << " = ";
    operands.top() -= temp;
    std::cout << operands.top() << std::endl;
    UNUSED(t);
}
void op::div (std::stack<float>& operands, Token* t) {
    if (operands.empty()) {
        std::cout << "ERROR: stack is empty" << std::endl;
        exit(-1);
    }
    float temp = operands.top();
    operands.pop();
    std::cout << operands.top() << " / " << temp << " = ";
    operands.top() /= temp;
    std::cout << operands.top() << std::endl;
    UNUSED(t);
}
void op::mod (std::stack<float>& operands, Token* t) {
    if (operands.empty()) {
        std::cout << "ERROR: stack is empty" << std::endl;
        exit(-1);
    }
    int temp = (int)operands.top();
    operands.pop();
    std::cout << operands.top() << " % " << temp << " = ";
    operands.top() =(float)((int)operands.top()%temp);
    std::cout << operands.top() << std::endl;
    UNUSED(t);
}
void op::pow (std::stack<float>& operands, Token* t) {
    if (operands.empty()) {
        std::cout << "ERROR: stack is empty" << std::endl;
        exit(-1);
    }
    float temp = operands.top();
    operands.pop();
    std::cout << operands.top() << " ^ " << temp << " = ";
    operands.top() = std::pow(operands.top(), temp);
    std::cout << operands.top() << std::endl;
    UNUSED(t);
}
void op::min (std::stack<float>& operands, Token* t) {
    if (operands.empty()) {
        std::cout << "ERROR: stack is empty" << std::endl;
        exit(-1);
    }
    float temp = operands.top();
    operands.pop();
    std::cout << operands.top() << " & " << temp << " = ";
    operands.top() = std::fmin(operands.top(), temp);
    std::cout << operands.top() << std::endl;
    UNUSED(t);
}
void op::max (std::stack<float>& operands, Token* t) {
    if (operands.empty()) {
        std::cout << "ERROR: stack is empty" << std::endl;
        exit(-1);
    }
    float temp = operands.top();
    operands.pop();
    std::cout << operands.top() << " | " << temp << " = ";
    operands.top() = std::fmin(operands.top(), temp);
    std::cout << operands.top() << std::endl;
    UNUSED(t);
}


Token::Token(TokenKind kind, std::string content) {
    this->name = content;
    this->kind = kind;
    if (kind == TK::NUMBER) {
        this->number = std::stof(content);
        this->table_id = 1;
    }
    else if (kind == TK::IDENTIFIER) {
        this->number = 0;
        this->table_id = 1;
    }
    else if (kind == TK::OPERATOR) {
        this->table_id = op_to_id.at(content);
        this->number = 0;
    }
    else {
        this->table_id = 0;
        this->number = 0;
    }
}

Token::Token(int id, std::string content) {
    this->name = content;
    this->kind = TK::OPERATOR;
    this->table_id = id;
    this->number = 0;
}

void Token::reduce (std::stack<float>& operands) {
    functions[table_id](operands, this);
}


Lexer::Lexer () {
    this->tokens = std::queue<Token>();
    this->content = "";
}


void Lexer::lex_number(std::stringstream& file) {
    do {
        content += c;
        file >> std::noskipws >> c;
    }
    while (!file.eof() && (std::isalnum(c) || c == '_' || c == '.'));

    if (is_number(content))
        tokens.push(Token(TK::NUMBER, content));
    else {
        std::cerr << "invalid number" << std::endl;
        exit(-1);
    }
    content = "";
}

void Lexer::lex_symbol(std::stringstream& file) {
    do {
        content += c;
        file >> std::noskipws >> c;
    }
    while (!file.eof() && (std::isalnum(c) || c == '_'));

    if (is_keyword(content))
        tokens.push(Token(TK::KEYWORD, content));
    else if (is_identifier(content))
        tokens.push(Token(TK::IDENTIFIER, content));
    else {
        std::cerr << "invalid identifier" << std::endl;
        exit(-1);
    }
    content = "";
}

bool Lexer::match_sequence (std::stringstream& file, std::string str) {
    char savec = c;
    std::streampos save = file.tellg();
    content =+ c;
    while (content.length() < str.length() && c != '\n' && !file.eof()) {
        file >> std::noskipws >> c;
        content += c;
    }
    if (content == str) {
        file >> std::noskipws >> c;
        return true;
    }
    else {
        c = savec;
        file.seekg(save);
        return false;
    }
}

std::queue<Token> Lexer::operator() (std::stringstream& file) {
    file >> std::noskipws >> c;
    while (!file.eof()) {
        if (c == ' ') {
            file >> std::noskipws >> c;
            continue;
        }
        if (c == '\n') {
            tokens.push(Token(TK::EOE, "$"));
            file >> std::noskipws >> c;
            continue;
        }

        if (std::isalpha(c) || c == '_') {
            lex_symbol(file);
            continue;
        }
        if (std::isdigit(c)) {
            lex_number(file);
            continue;
        }

        bool found = false;
        for (const auto& [key, value] : op_to_id) {
            if (match_sequence(file, key)) {
                tokens.push(Token(value, content));
                content = "";
                found = true;
                break;
            }
        }
        if (found) continue;

        std::cerr << "invalid operator" << std::endl;
        exit(-1);
    }
    tokens.push(Token(TK::EOE, "$"));
    return tokens;
}

std::ostream& operator<< (std::ostream& stream, const Token& t) {
    for (const auto& [key, value] : kind_to_str) {
        if (t.kind == key) {
            stream << t.name << " (" << t.table_id << ")";
            return stream;
        }
    }
    return stream;
}
