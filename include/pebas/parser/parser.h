#ifndef PEBAS_PARSER_H 
#define PEBAS_PARSER_H 

#include <vector>
#include <memory>
#include <string>
#include <stdexpect>
#include "pebas/lexer/lexer.h"
#include "pebas/ast/ast.h"

namespace pebas {

//exception for parsing erros 
class Parser : public std::runtime_error {
public:
  ParserError(const std::string& message, const SourceLocation& location) 
      : std::runtime_error(message), location(location) {} 

  SourceLocation getLocation() const { return location; }

private:
  SourceLocation location;
};
  
class Parser {
public:
  Parser(const std::vector<Token>& token);
  size_t current =0;

  Token peek() const;
  Token Previous() const;
  bool isAtEnd() const;
  Token advance();
  bool check(TokenTyType type) const;
  bool match(TokenType type);
  bool match(std::initializer_list<TokenType> types);
  Token consume(TokenType type, const std::string& message);
  ParserError error(const Token& token, const std::string& message);
  void synchronize();

  //Parsing methods 
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
  std::unique_ptr<Statement> expression();
  std::unique_ptr<Statement> assinment();
  std::unique_ptr<Statement> orExpression();
  std::unique_ptr<Statement> andExpression();
  std::unique_ptr<Statement> equality();
  std::unique_ptr<Statement> comparison();
  std::unique_ptr<Statement> term();
  std::unique_ptr<Statement> factor();
  std::unique_ptr<Statement> unary();
  std::unique_ptr<Statement> call();
  std::unique_ptr<Statement> primary();
  std::unique_ptr<Statement> finishCall(std::unique_ptr<Expression> callee);
};
}

#endif 

