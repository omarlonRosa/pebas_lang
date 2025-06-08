#ifndef PEBAS_PARSER_H
#define PEBAS_PARSER_H

#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <initializer_list>
#include "pebas/lexer/lexer.h"
#include "pebas/ast/ast.h"

namespace pebas {

// Exception for parsing errors
class ParseError : public std::runtime_error {
public:
    SourceLocation location;
    
    ParseError(const std::string& message, const SourceLocation& location)
        : std::runtime_error(message), location(location) {}
};

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    
    std::unique_ptr<Program> parse();

private:
    const std::vector<Token>& tokens;
    size_t current = 0;

    // Utility methods
    Token peek() const;
    Token previous() const;
    bool isAtEnd() const;
    Token advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    bool match(std::initializer_list<TokenType> types);
    Token consume(TokenType type, const std::string& message);
    ParseError error(const Token& token, const std::string& message);
    void synchronize();

    // Grammar rules
    std::unique_ptr<Statement> declaration();
    std::unique_ptr<Statement> varDeclaration();
    std::unique_ptr<Statement> functionDeclaration();
    std::unique_ptr<Statement> statement();
    std::unique_ptr<Statement> expressionStatement();
    std::unique_ptr<Statement> blockStatement();
    std::unique_ptr<Statement> ifStatement();
    std::unique_ptr<Statement> whileStatement();
    std::unique_ptr<Statement> forStatement();
    std::unique_ptr<Statement> returnStatement();
    std::unique_ptr<Statement> breakStatement();
    std::unique_ptr<Statement> continueStatement();

    // Expression parsing
    std::unique_ptr<Expression> expression();
    std::unique_ptr<Expression> assignment();
    std::unique_ptr<Expression> logicalOr();
    std::unique_ptr<Expression> logicalAnd();
    std::unique_ptr<Expression> equality();
    std::unique_ptr<Expression> comparison();
    std::unique_ptr<Expression> term();
    std::unique_ptr<Expression> factor();
    std::unique_ptr<Expression> unary();
    std::unique_ptr<Expression> call();
    std::unique_ptr<Expression> primary();
    
    std::unique_ptr<Expression> finishCall(std::unique_ptr<Expression> callee);
};

} // namespace pebas

#endif // PEBAS_PARSER_H

