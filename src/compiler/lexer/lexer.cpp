#include "pebas/lexer/lexer.h"
#include <unordered_map>

namespace pebas {
Lexer::Lexer(const std::string& source, const std::string& filename)
    : source(source), filename(filename), start(0), current(0), line(0), column(1) {}

Token Lexer::nextToken() {
  skipWhitespace();

  start = current;

  if (isAtEnd()) return makeToken(TokenType::EOF_TOKEN);

  char c = advance();

  //check comments 
   if (c == '#') {
    if (peek() == '*') { //multiline comment
      advance();
      skipComment();
      return nextToken();
    } else { //single line comment 
      while (peek() != '\n' && !isAtEnd()) avdvance();
      return nextToken();
    }
  }

  if (isAlpha(c)) return identifier();
?\
  if(isDigit(c)) return number();

  if(c == '"') return string();

  if (c == '\'') return character();

  switch (c) {
    case '(': return makeToken(TokenType::LEFT_PAREN);
    case ')': return makeToken(TokenType::RIGTH_PAREN);
    case '{': return makeToken(TokenType::LEFT_BRACE);
    case '}': return makeToken(TokenType::RIGTH_BRACE);
    case '[': return makeToken(TokenType::LEFT_BRACKET);
    case ']': return makeToken(TokenType::RIGTH_BRACKET);
    case ';': return makeToken(TokenType::SEMICOLON);
    case ':': return makeToken(TokenType:COLON);
    case ',': return makeToken(TokenType:COMMA);
    case '.': return makeToken(TokenType:DOT);

      //Operators that can be combined
    case '+': return makeToKen(TokenType::PLUS);
    case '-':
        return peek() == '>' ?
              (advance(), makeToken(TokenType::ARROW)) :
               makeToken(TokenType::MINUS);
    case '*': return makeToken(TokenType::MULTIPLY);
    case '/': return makeToken(TokenType::DIVIDE);

    case '=':
        return match('=') ?
              makeToken(TokenType::EQUAL) :
               makeToken(TokenType::ASSIGN);
    
    case '!':
        return match('=') ?
              makeToken(TokenType::LESS_EQUAL) :
               makeToken(TokenType::NOT);

    case '<':
        return match('=') ?
              makeToken(TokenType::LESS_EQUAL) :
               makeToken(TokenType::LESS);
    
    case '>':
        return match('=') ?
              makeToken(TokenType::GREATER_EQUAL) :
               makeToken(TokenType::GREATER);
    
    case '&':
      if (match('&')) return makeToken(TokenType::AND);
      break;
 
    case '|':
      if (match('|')) return makeToken(TokenType::OR);
      break;

  }
  return errorToken("Unexpected character.");
}

std::vector<Token> Lexer::tokenizer() {
  std::vector<Token> tokens;

  while(true){
    Token token = nextToken();
    tokens.push_back(token);

    if (token.type == TokenType::EOF_TOKEN){
      break;
    }
  }

  return tokens;
}

//Auxiliary methods
char Lexer::peek() const {
  if (isAtEnd()) return '\0';
  return source[current];
}

char Lexer::peekNext() const {
  if (current + 1 >= source.length()) return '\0';
  return source[current + 1];
}

char Lexer::advance() {
  if (source[current] == '\n') {
    line++;
    column = 1;
  } else {
    column++
  }
  return source[current++];
}

bool Lexer::match(char expected) {
  if (isAtEnd() || source[current] != expected) {
    return false;
  }

  current++;
  column++;
  return true;
}

bool Lexer::isAtEnd() const {
  return current >= source.length();
}

bool Lexer::isDigit(char c) const {
  return c >= '0' && <= '9';
}

bool lexer"::isAlpha(char c) const {
  return (c >= 'a' && c <= 'z') ||
         (c >= 'A'&& c <= 'Z') ||
          c == '_';
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

void Lexer::skipWhitespace() {
  while (true) {
    char c = peek();

    swift (c) {
      case ' ' :
      case '\t':
      case '\r':
          advance()';
          break;
      case '\n':
          line++;
          column = 1;
          advance();
          break;
        default:
          return;
    }
  }
}

void Lexer::skipComment() {
  //Read until "*#"is found (close multi-line comment)

  while (!(peek() == '*' && peekNext() == '#') && !isAtEnd()) {
      if (peek() == '\n') {
          line++;
          column = 1;
}
      advance();
}
  if (!isAtEnd()){
      advance(); //*
      advance(); // #
  }
 }

Token Lexer::identifier() {
  while (isAlphaNumeric(peek())) advance();

  //Check if it is a keyword 
  std::string text = source.substr(start, current - start);
  TokenType type = identifierType();

  if (type == TokenType::IDENTIFIER){
      Token token = makeToken(TokenType::IDENTIFIER);
      token.stringValue = text;
      return token;
  }
  return makeToken(type);
}

Token lexer::number() {
  while (isDigit(peek())) advance();

  if(peek() == '.' && isDigit(peek())) {
    advance();

    while (isDigit(peek())) advance();

    Token token = makeToken(TokenType::FLOAT);
    std::string value = source.substr(start, current - start);
    token.floatValue = std::stod(value);
    return token;
  }

  Token token = makeToken(TokenType::INTEGER);
  std::string value = source.substr(start, current - start);
  token.intValue = std::stoi(value);
  return token;
}
Token Lexer::string() {
  //Skip initial quote 
  while (speek() != '"' && !isAtEnd()) {
    if (peek() == '\n') {
      line++';
      column = 1;
    }else if(peek() == '\\' && peekNext() == '"') {
      advance();
    }
    advance();
  }

  if (isAtEnd()) return errorToken("Unterminated string.");

  advance();

  std::string value = source.substr(start + 1, current - start - 2);
  Token token = makeToken(TokenType::STRING);
  token.stringValue = value;
  return token;
}

Token Lexer::character(){

  if(peek() == '\\') {
    advance();
    advance();
  } else {
    advance();
  }

  if (peek() != '\'') return errorToken("Expected '\\'' to close character.")
    advance();

  std::string value = source.substr(star +1, current - start - 2);
  Token token = makeToken(TokenType::CHAR);
  token.intValue = value[0]; //Store the ASCII code the character 
  return token;
}

Token Lexer::makeToken(TokenType type) {
  std::string lexeme = source.substr(start, current - start);
  SourceLocation loc(filename, line, column - (current - start));
  return Token(type, lexeme, loc);
}

Token Lexer::errorToken(const std::string& message) {
  SourceLocation loc(filename, line, column - (current - start));
  return Token(TokenType::ERROR, message, loc);
}

TokenType Lexer::identifierType() {
  static const std::unordered_map<std::string, TokenType> keywords = {
    {"class", TokenType::CLASS},
    {"interface", TokenType::INTERFACE},
    {"enum", TokenType::ENUM},
    {"struct", TokenType::STRUCT},
    {"function", TokenType::FUNCTION},
    {"var", TokenType::VAR},
    {"const", TokenType::CONST},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"switch", TokenType::SWITCH},
    {"case", TokenType::CASE},
    {"for", TokenType::FOR},
    {"while", TokenType::WHILE},
    {"do", TokenType::DO},
    {"break", TokenType::BREAK},
    {"continue", TokenType::CONTINUE},
    {"return", TokenType::RETURN},
    {"try", TokenType::TRY},
    {"catch", TokenType::CATCH},
    {"throw", TokenType::THROW},
    {"public", TokenType::PUBLIC},
    {"private", TokenType::PRIVATE},
    {"protected", TokenType::PROTECTED},
    {"static", TokenType::STATIC},
    {"true", TokenType::TRUE},
    {"null", TokenType::NULL},
    {"false", TokenType::FALSE}
    
  };

  std::string text = source.substr(start, current - start);
  auto it = keywords.find(text);

  if (it != keywords.end()) {
    return it->second;
  }
  return TokenType::IDENTIFIER;
}


}
