#ifndef PEBAS_LEXER_H
#define PEBAS_LEXER_H 

#include <string>
#include <vector>
#include <map>
#include <variant>


namespace pebas {

enum class TokenType {
  //Single-Character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, 
  LEFT_BRACKET, RIGHT_BRACKET, COMMA, DOT, MINUS, PLUS, 
  SEMICOLON, SLASH, STAR, COLON, QUESTION, PERCENT, TILDE,
  AMPERSAND, PIPE, CARET, 

  //One or two character tokens.
  BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,

  //Assingment operators 
  PLUS_ASSIGN, MINUS_ASSIGN, STAR_ASSIGN, PERCENT_ASSIGN,
  AMPERSAND_ASSIGN, PIPE_ASSIGN, CARET_ASSIGN,
  LESS_LESS_ASSIGN, GREATER_GREATER_ASSIGN,

  //Increment/Decrement 
  AND_AND, OR_OR, 

  //Bitwise shifts 
  ARROW_RIGHT, // ->
  DOUBLE_ARROW_RIGHT, // =>
  COLON_COLON, // :: (Scope resolution)
  DOT_DOT, // .. (Range operator)

  //Literals.
  IDENTIFIER, STRING, INTERGER_LITERAL, FLOAT_LITERAL, CHAR_LITERAL, 

  //keywords 
  KEYWORD_CLASS, KEYWORD_INTERFACE, KEYWORD_ENUM, 
  KEYWORD_STRUCT, KEYWORD_FUNCTION, KEYWORD_VAR, 
  KEYWORD_CONST, KEYWORD_PUBLIC, KEYWORD_PRIVATE,
  KEYWORD_PROTECTED, KEYWORD_STATIC, KEYWORD_ABSTRACT,
  KEYWORD_OVERRIDE, KEYWORD_VIRTUAL, KEYWORD_IMPORT,
  KEYWORD_PACKAGE, KEYWORD_NEW, KEYWORD_THIS, 
  KEYWORD_SUPER, KEYWORD_AS, KEYWORD_IS,

  //Flow control 
  KEYWORD_IF, KEYWORD_ELSE, KEYWORD_SWITCH, KEYWORD_CASE,
  KEYWORD_FOR, KEYWORD_WHILE, KEYWORD_DO, KEYWORD_BREAK,
  KEYWORD_CONTINUE, KEYWORD_RETURN,

  //Error Handling 
  KEYWORD_TRY, KEYWORD_CATCH, KEYWORD_THROW,

  //Literals as keywords 
  KEYWORD_NULL, KEYWORD_TRUE, KEYWORD_FALSE,

  KEYWORD_PRINT,

  TOKEN_ERROR, TOKEN_EOF,

}; 

struct Token {
  TokenType type;
  std::string lexeme;
  int line;
  int col;

  TokenType(TokenType type, std::string lexeme, int line, int col);

  std::string to_string() const;
};

class Lexer {
public:
  Lexer(std::string source);
  std::vector<Token> scan_Tokens();

private: 
  std::string m_source;
  std::vector<Token> m_tokens;
  std::map<std::string, TokenType> m_keywords;

  int m_start = 0;
  int m_current = 0;
  int m_line = 1;
  int m_column = 1;

  void scan_token_internal();
  void add_token(TokenType type);


  char advance();
  char peek() const;
  char peekNext() const;
  bool match(char expected);

  void string_literal(char quote_type = '"');
  void number_literal();
  void identifier();
  void block_comment();

  bool is_at_end() const;
  bool is_digit(char c) const;
  bool is_alpha(char c) const;
  bool is_alpha_numeric(char c) const;
};
} 

#endif 
 
