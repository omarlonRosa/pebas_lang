#include <iostream>
#include <string>
#include <vector>
#include <regex>

// Definição de Tokens
enum TokenType {
    INT, FLOAT, DOUBLE, STRING, IDENTIFIER, KEYWORD,
    OPERATOR, SEPARATOR, NUMBER, TEXT, COMMENT, UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType type, const std::string &value) : type(type), value(value) {}
};

class Lexer {
private:
    std::string source;
    std::vector<Token> tokens;

    void tokenize() {
        std::regex pattern(R"((int|float|double|string)|([a-zA-Z_][a-zA-Z0-9_]*)|(\d+\.\d+|\d+)|(\"[^\"]*\")|([+\-*/=<>])|(\/\/.*?$)|(\/\/ <\/.*?<\/))");
        auto words_begin = std::sregex_iterator(source.begin(), source.end(), pattern);
        auto words_end = std::sregex_iterator();

        for (auto it = words_begin; it != words_end; ++it) {
            std::string match = it->str();
            if (match == "int" || match == "float" || match == "double" || match == "string")
                tokens.emplace_back(KEYWORD, match);
            else if (std::regex_match(match, std::regex("[a-zA-Z_][a-zA-Z0-9_]*")))
                tokens.emplace_back(IDENTIFIER, match);
            else if (std::regex_match(match, std::regex("\d+\.\d+|\d+")))
                tokens.emplace_back(NUMBER, match);
            else if (std::regex_match(match, std::regex("\"[^\"]*\"")))
                tokens.emplace_back(TEXT, match);
            else if (std::regex_match(match, std::regex("\/\/ <\/.*?<\/")))
                tokens.emplace_back(COMMENT, "Comentário de bloco: " + match);
            else if (std::regex_match(match, std::regex("\/\/.*?$")))
                tokens.emplace_back(COMMENT, "Comentário de uma linha: " + match);
            else
                tokens.emplace_back(OPERATOR, match);
        }
    }

public:
    Lexer(const std::string &source) : source(source) { tokenize(); }

    void displayTokens() const {
        for (const auto &token : tokens) {
            std::cout << "Token: " << token.value << " Type: " << token.type << std::endl;
        }
    }
};

int main() {
    std::string source = "int x = 10; // </ Este é um comentário de bloco\n// Este é um comentário de uma linha\nstring nome = \"Pebas Lang\";";
    Lexer lexer(source);
    lexer.displayTokens();

    return 0;
}

