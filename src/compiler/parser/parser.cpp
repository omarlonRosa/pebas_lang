#include "pebas/parser/parser.h"

namespace pebas {

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

std::unique_ptr<Program> Parser::parse() {
    std::vector<std::unique_ptr<Statement>> statements;

    while (!isAtEnd()) {
        try {
            statements.push_back(declaration());
        } catch (const ParseError& error) {
            synchronize();
        }
    }

    return std::make_unique<Program>(std::move(statements));
}

// Utility methods
Token Parser::peek() const {
    return tokens[current];
}

Token Parser::previous() const {
    return tokens[current - 1];
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::TOKEN_EOF;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for (auto type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}

ParseError Parser::error(const Token& token, const std::string& message) {
    return ParseError(message, token.location);
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;

        switch (peek().type) {
            case TokenType::KEYWORD_CLASS:
            case TokenType::KEYWORD_FUNCTION:
            case TokenType::KEYWORD_VAR:
            case TokenType::KEYWORD_FOR:
            case TokenType::KEYWORD_IF:
            case TokenType::KEYWORD_WHILE:
            case TokenType::KEYWORD_RETURN:
                return;
            default:
                break;
        }
        advance();
    }
}

// Grammar rules
std::unique_ptr<Statement> Parser::declaration() {
    if (match(TokenType::KEYWORD_VAR)) {
        return varDeclaration();
    }
    if (match(TokenType::KEYWORD_FUNCTION)) {
        return functionDeclaration();
    }

    return statement();
}

std::unique_ptr<Statement> Parser::varDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expected variable name.");

    std::optional<std::string> typeName;
    if (match(TokenType::COLON)) {
        Token type = consume(TokenType::IDENTIFIER, "Expected type after ':'.");
        typeName = type.lexeme;
    }

    std::unique_ptr<Expression> initializer = nullptr;
    if (match(TokenType::EQUAL)) {
        initializer = expression();
    }

    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration.");
    return std::make_unique<VariableDecl>(name, typeName, std::move(initializer));
}

std::unique_ptr<Statement> Parser::functionDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expected function name.");
    consume(TokenType::LEFT_PAREN, "Expected '(' after function name.");

    std::vector<Parameter> parameters;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            Token paramName = consume(TokenType::IDENTIFIER, "Expected parameter name.");
            consume(TokenType::COLON, "Expected ':' after parameter name.");
            Token paramType = consume(TokenType::IDENTIFIER, "Expected parameter type.");

            parameters.emplace_back(paramName.lexeme, paramType.lexeme, paramName.location);
        } while (match(TokenType::COMMA));
    }
    consume(TokenType::RIGHT_PAREN, "Expected ')' after parameters.");

    std::optional<std::string> returnType;
    if (match(TokenType::ARROW)) {
        Token type = consume(TokenType::IDENTIFIER, "Expected return type after '->'.");
        returnType = type.lexeme;
    }

    consume(TokenType::LEFT_BRACE, "Expected '{' before function body.");
    std::unique_ptr<BlockStmt> body = 
        std::unique_ptr<BlockStmt>(static_cast<BlockStmt*>(blockStatement().release()));

    return std::make_unique<FunctionDecl>(name, std::move(parameters), returnType, std::move(body));
}

std::unique_ptr<Statement> Parser::statement() {
    if (match(TokenType::KEYWORD_IF)) {
        return ifStatement();
    }
    if (match(TokenType::KEYWORD_WHILE)) {
        return whileStatement();
    }
    if (match(TokenType::KEYWORD_FOR)) {
        return forStatement();
    }
    if (match(TokenType::KEYWORD_RETURN)) {
        return returnStatement();
    }
    if (match(TokenType::KEYWORD_BREAK)) {
        return breakStatement();
    }
    if (match(TokenType::KEYWORD_CONTINUE)) {
        return continueStatement();
    }
    if (match(TokenType::LEFT_BRACE)) {
        return blockStatement();
    }
    
    return expressionStatement();
}

std::unique_ptr<Statement> Parser::expressionStatement() {
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression.");

    return std::make_unique<ExpressionStmt>(std::move(expr), previous().location);
}

std::unique_ptr<Statement> Parser::blockStatement() {
    std::vector<std::unique_ptr<Statement>> statements;
    SourceLocation location = previous().location;

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expected '}' after block.");
    return std::make_unique<BlockStmt>(std::move(statements), location);
}

std::unique_ptr<Statement> Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'if'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after condition.");

    auto thenBranch = statement();
    std::unique_ptr<Statement> elseBranch = nullptr;

    if (match(TokenType::KEYWORD_ELSE)) {
        elseBranch = statement();
    }

    return std::make_unique<IfStmt>(
        std::move(condition),
        std::move(thenBranch),
        std::move(elseBranch),
        previous().location
    );
}

std::unique_ptr<Statement> Parser::whileStatement() {
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'while'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after condition.");
    auto body = statement();

    return std::make_unique<WhileStmt>(std::move(condition), std::move(body), previous().location);
}

std::unique_ptr<Statement> Parser::forStatement() {
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'for'.");
    
    // For now, implement a simple for loop
    // TODO: Implement for-in loops and range-based loops
    std::unique_ptr<Statement> initializer;
    if (match(TokenType::SEMICOLON)) {
        initializer = nullptr;
    } else if (match(TokenType::KEYWORD_VAR)) {
        initializer = varDeclaration();
    } else {
        initializer = expressionStatement();
    }

    std::unique_ptr<Expression> condition = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expected ';' after loop condition.");

    std::unique_ptr<Expression> increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN)) {
        increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expected ')' after for clauses.");

    auto body = statement();

    // Desugar for loop into while loop
    if (increment != nullptr) {
        std::vector<std::unique_ptr<Statement>> bodyStatements;
        bodyStatements.push_back(std::move(body));
        bodyStatements.push_back(std::make_unique<ExpressionStmt>(std::move(increment), previous().location));
        body = std::make_unique<BlockStmt>(std::move(bodyStatements), previous().location);
    }

    if (condition == nullptr) {
        condition = std::make_unique<LiteralExpr>(Token(TokenType::KEYWORD_TRUE, "true", previous().location));
    }
    body = std::make_unique<WhileStmt>(std::move(condition), std::move(body), previous().location);

    if (initializer != nullptr) {
        std::vector<std::unique_ptr<Statement>> statements;
        statements.push_back(std::move(initializer));
        statements.push_back(std::move(body));
        body = std::make_unique<BlockStmt>(std::move(statements), previous().location);
    }

    return body;
}

std::unique_ptr<Statement> Parser::returnStatement() {
    Token keyword = previous();
    std::unique_ptr<Expression> value = nullptr;
    
    if (!check(TokenType::SEMICOLON)) {
        value = expression();
    }

    consume(TokenType::SEMICOLON, "Expected ';' after return value.");
    return std::make_unique<ReturnStmt>(keyword, std::move(value));
}

std::unique_ptr<Statement> Parser::breakStatement() {
    Token keyword = previous();
    consume(TokenType::SEMICOLON, "Expected ';' after 'break'.");
    // TODO: Implement break statement AST node
    return std::make_unique<ExpressionStmt>(nullptr, keyword.location);
}

std::unique_ptr<Statement> Parser::continueStatement() {
    Token keyword = previous();
    consume(TokenType::SEMICOLON, "Expected ';' after 'continue'.");
    // TODO: Implement continue statement AST node
    return std::make_unique<ExpressionStmt>(nullptr, keyword.location);
}

// Expression parsing (precedence climbing)
std::unique_ptr<Expression> Parser::expression() {
    return assignment();
}

std::unique_ptr<Expression> Parser::assignment() {
    auto expr = logicalOr();

    if (match(TokenType::EQUAL)) {
        Token equals = previous();
        auto value = assignment();

        if (auto identifier = dynamic_cast<IdentifierExpr*>(expr.get())) {
            Token name = identifier->token;
            expr.release(); // Release ownership before creating AssignmentExpr
            return std::make_unique<AssignmentExpr>(name, std::move(value));
        }

        error(equals, "Invalid assignment target.");
    }

    return expr;
}

std::unique_ptr<Expression> Parser::logicalOr() {
    auto expr = logicalAnd();

    while (match(TokenType::OR_OR)) {
        Token operator_ = previous();
        auto right = logicalAnd();
        expr = std::make_unique<BinaryExpr>(std::move(expr), operator_, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expression> Parser::logicalAnd() {
    auto expr = equality();

    while (match(TokenType::AND_AND)) {
        Token operator_ = previous();
        auto right = equality();
        expr = std::make_unique<BinaryExpr>(std::move(expr), operator_, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expression> Parser::equality() {
    auto expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token operator_ = previous();
        auto right = comparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), operator_, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expression> Parser::comparison() {
    auto expr = term();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token operator_ = previous();
        auto right = term();
        expr = std::make_unique<BinaryExpr>(std::move(expr), operator_, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expression> Parser::term() {
    auto expr = factor();

    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token operator_ = previous();
        auto right = factor();
        expr = std::make_unique<BinaryExpr>(std::move(expr), operator_, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expression> Parser::factor() {
    auto expr = unary();

    while (match({TokenType::SLASH, TokenType::STAR, TokenType::PERCENT})) {
        Token operator_ = previous();
        auto right = unary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), operator_, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expression> Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token operator_ = previous();
        auto right = unary();
        return std::make_unique<UnaryExpr>(operator_, std::move(right));
    }

    return call();
}

std::unique_ptr<Expression> Parser::call() {
    auto expr = primary();

    while (true) {
        if (match(TokenType::LEFT_PAREN)) {
            expr = finishCall(std::move(expr));
        } else {
            break;
        }
    }

    return expr;
}

std::unique_ptr<Expression> Parser::finishCall(std::unique_ptr<Expression> callee) {
    std::vector<std::unique_ptr<Expression>> arguments;

    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            arguments.push_back(expression());
        } while (match(TokenType::COMMA));
    }

    Token paren = consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments.");
    return std::make_unique<CallExpr>(std::move(callee), std::move(arguments), paren.location);
}

std::unique_ptr<Expression> Parser::primary() {
    if (match(TokenType::KEYWORD_TRUE)) {
        return std::make_unique<LiteralExpr>(previous());
    }

    if (match(TokenType::KEYWORD_FALSE)) {
        return std::make_unique<LiteralExpr>(previous());
    }

    if (match(TokenType::KEYWORD_NULL)) {
        return std::make_unique<LiteralExpr>(previous());
    }

    if (match({TokenType::INTEGER_LITERAL, TokenType::FLOAT_LITERAL, TokenType::STRING, TokenType::CHAR_LITERAL})) {
        return std::make_unique<LiteralExpr>(previous());
    }

    if (match(TokenType::IDENTIFIER)) {
        return std::make_unique<IdentifierExpr>(previous());
    }

    if (match(TokenType::LEFT_PAREN)) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
        return std::make_unique<GroupingExpr>(std::move(expr), previous().location);
    }

    throw error(peek(), "Expected expression.");
}

} // namespace pebas

