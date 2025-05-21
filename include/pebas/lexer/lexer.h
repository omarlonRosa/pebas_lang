#ifndef PEBAS_LEXER_H
#define PEBAS_LEXER_H 

#include <string>
#include <vector>

namespace pebas {

enum class TokensType {
    CLASS, INTERFACE, ENUM, STRUCT, FUNCTION, VAR, CONST,
    IF, ELSE, SWITCH, CASE, FOR, WHILE, DO, BREAK, CONTINUE, RETURN,
  TRY, CATCH, THROW, PUBLIC, PRIVATE, PROTECTED, STATIC,

  PLUS, MINUS, MULTIPLY, DIVIDE, ASSIGN, EQUAL, NOT_EQUAL,
  LESS, GREATER, LESS_EQUAL, GREATER_EQUAL, AND, OR, NOT,

  INDENTIFIER, INTEGER, FLOAT, STRING, CHAR, TRUE, FALSE, NULL_LITERAL,

  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET,
  SEMICOLON, COLON, COMMA, DOT, ARROW,

  COMMENT, EOF_TOKEN, ERROR
}; 

struct SourceLocation {
  std::string filename;
  int line;
  int column;

  SourceLocation() : line(1), column(1) {}
  SourceLocation(std::string fn, int l, int c) : filename(fn), line(l), column(c) {}
};

class Token {
public:
  TokenType type;
  std::string lexeme;
  SourceLocation location; 

  union {
    int intValue;
    double floatValue;
    bool boolValue;
  };
  std::string stringValue;

  Token(TokenType type, const std::string& lexeme, const SourceLocation& loc)
    : type(type),lexeme(lexeme), location(loc) {}
};

class Lexer {
public:
  Lexer(const std::string& source, const std::string& filename = "<stdin>"); 
  Token nextToken();
  std::vector<Token> tokenizer();

private: 
  std::string source;
  std::string filename;
  int start;
  int current;
  int line;
  int column;

  char peek() const;
  char peekNext() const;
  char advance();
  bool match(char expected);
  bool isAtEnd() const;
  bool isDigit(char c) const;
  bool isAlpha(char c) const;
  bool isAlphaNumeric(char c) const;

  void skipWhitespace();
  void skipComment();

  Token identifier();
  Token number();
  Token string();
  Token character();

  Token makeToken(TokenType type);
  Token errorToken(const std::string& message);

  TokenType identifierType();
};
} 

#endif 
