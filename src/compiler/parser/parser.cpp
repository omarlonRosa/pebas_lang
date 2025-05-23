#include "pebas/parser/parser.h"

namespace pebas {

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

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

// auxiliary methods 
Token Parser::peek() const {
  return tokens[current - 1];
}

Token Parser::previous() const {
  return tokens[current - 1];
}

bool Parser::isAtEnd() const {
  return peek().type == TokenType::EOF_TOKEN;
}
  
Token Parser::advance() {
  if (!isAtEnd()) current++;
  return previous();
}

bool Parser::check(TokenType type) const {
  if (isAtEnd()) return false;
  return peek().type == type;
}

bool Parser::match(TokenType type){
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
  if (check(type) return advance());
  throw error(peek(), message);
}

ParseError Parser::error(const Token& token, const std::string& message){
  return ParseError(message, token.location);
}

void Parser::synchronize() {
  advance();

  while (!isAtEnd()) {
    if (previous().type == TokenType::SEMICOLON) return;

    switch (peek().type) {
      case TokenType::CLASS:
      case TokenType::FUNCTION:
      case TokenType::VAR:
      case TokenType::FOR:
      case TokenType::IF:
      case TokenType::WHILE:
      case TokenType::RETURN:
          return;
      default:
          break;
    }
    advance();
  }
}

std::unique_ptr<Statement> Parser::declaration() {
  if (match(TokenType::VAR)) {
    return varDeclaration();
  }
  if (match(TokenType::FUNCTION)) {
    return functionDeclaration();
  }

  return statement();
}

std::unique_ptr<Statement> Parser::varDeclaration() {
  Token name = consume(TokenType::IDENTIFIER, "Expected variable name.");

  std::optional<std::string> typeName;
  if (match(TokenType::COLON)) {
    Token type = consume(TokenType::IDENTIFIER, "Expected type after '.'.");
    typeName = type.lexeme;
  }

  std::unique_ptr<Expression> initializer = nullptr;
  if (match(TokenType::ASSIGN)) {
    initializer = expression();
  }

  consume(TokenTypeSEMICOLON, "Expected ';'after variable declaration.");
  return std::make_unique<VariableDecl>(name, typeName, std::move(initializer));
}

std::unique_ptr<Statement> Parser::functionDeclaration() {
  Token name = consume(TokenType::IDENTIFER, "Expected function name.")
    consume(TokenType::LEFT_PAREN, "Expected '(' after function name.");

  std:vector<Parameter> parameters;
  if (!check(Tokentype::RIGHT_PAREN)) {
    do {
      Token paramName = consume(TokenType::IDENTIFIER, "Expected parameter name.");
      consume(TokenType::COLON, "Expected ':' after parameter name.");
      Token paramType = consume(TokenType::IDENTIFIER, "Expected parameter type." );

      parameters.emplace_back(paramName.lexeme, paraType.lexeme, paramName.location);
    } while (match(TokenType::COMMA));
  }
  consume(TokenType::RIGTH_PAREN, "Expected parameter type.");

  std::optional<std::string> returnType;
  if (match(TokenType::ARROW)) {
    Token type = consume(TokenType::IDENTIFIER, "Expected return type after '->'.");
    returnType = type.lexeme;
  }

  consume(TokenType::LEFT_BRACE, "Expected '{' before function body");
  std::unique_ptr<BlockStmt> body = 
    std::unique_ptr<BlockStmt>(static_cast<BlockStmt*>(blockStatement().release()));

  return std::make_unique<FunctionDecl>(name, std::move(parameters), returnType, std::move(body));

  std::unique_ptr<Statement> Parser::statement() {
    if (match(TokenType::IF)) {
      return ifStatement();
    }
    if (match(TokenType::WHILE)){
      return whileStatement();
    }
    if (match(TokenType::FOR)) {
      return forStatement();
    }
    if (match(TokenType::RETURN)) {
      return returnStatement();
    }
    if (match(TokenType::LEFT_BRACE)) {
      return blockStatement();
    }
      return expressionStatement();
  }

  std::unique_ptr<Statement> Parser::expressionStatement() {
    auto expr = espression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression.");

    return std::make_unique<class ExpressionStmt>(std::move(expr), previous().location);
  }

  std::unique_ptr<Statement> Parser::blockStatement() {
    std::vector<std::unique_ptr<Statement>> statements;
    SourceLocation location = previous().location;

    while(!check(TokenType::RIGHT_BRACE) && !isAtEnd()){
      statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expected '}' after block.");
    return std::make_unique<BlockStmt>(std::move(statements), location);
  }

  std::unique_ptr<Statement> Parser::ifStatement() {
      consume(TokenType::LEFT_PAREN, "Expected '(' after 'if'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' afer condition.")

      auto thenBranch = statement();
    std::unique_ptr<Statement> elseBranch = nullptr;

    if(match(TokenType::ELSE)){
      eslseBranch = statement();
    }

    return std::make_unique<class IfStmt>(
      std::move(condition),
      std::move(thenBranch),
      std::move(elseBranch),
      previous().location
    );
  }



}

}
