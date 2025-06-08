#ifndef PEBAS_LEXER_H
#define PEBAS_LEXER_H

#include <string>
#include <vector>
#include <map>
#include <variant>

namespace pebas {

// Single-Character tokens
enum class TokenType {
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    LEFT_BRACKET, RIGHT_BRACKET, COMMA, DOT, MINUS, PLUS,
    SEMICOLON, SLASH, STAR, PERCENT, TILDE,

    // One or two character tokens
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    ARROW, // ->
    DOUBLE_ARROW_RIGHT, // =>
    COLON_COLON, // ::
    DOT_DOT, // .. (Range operator)

    // Bitwise shifts
    ARROW_RIGHT, // << (Left shift)
    DOUBLE_ARROW_RIGHT_OP, // >> (Right shift)

    // Assignment operators
    PLUS_ASSIGN, MINUS_ASSIGN, STAR_ASSIGN, PERCENT_ASSIGN,
    AMPERSAND_ASSIGN, PIPE_ASSIGN, CARET_ASSIGN,
    LESS_LESS_ASSIGN, GREATER_GREATER_ASSIGN,

    // Increment/Decrement
    AND_AND, OR_OR,

    // Literals
    IDENTIFIER, STRING, INTEGER_LITERAL, FLOAT_LITERAL, CHAR_LITERAL,

    // Keywords
    KEYWORD_CLASS, KEYWORD_INTERFACE, KEYWORD_ENUM,
    KEYWORD_STRUCT, KEYWORD_FUNCTION, KEYWORD_VAR,
    KEYWORD_CONST, KEYWORD_PUBLIC, KEYWORD_PRIVATE,
    KEYWORD_PROTECTED, KEYWORD_STATIC, KEYWORD_ABSTRACT,
    KEYWORD_OVERRIDE, KEYWORD_VIRTUAL, KEYWORD_IMPORT,
    KEYWORD_PACKAGE, KEYWORD_NEW, KEYWORD_THIS,
    KEYWORD_SUPER, KEYWORD_AS, KEYWORD_IS,

    // Flow control
    KEYWORD_IF, KEYWORD_ELSE, KEYWORD_SWITCH, KEYWORD_CASE,
    KEYWORD_FOR, KEYWORD_WHILE, KEYWORD_DO, KEYWORD_BREAK,
    KEYWORD_CONTINUE, KEYWORD_RETURN,

    // Error Handling
    KEYWORD_TRY, KEYWORD_CATCH, KEYWORD_THROW,

    // Literals as keywords
    KEYWORD_NULL, KEYWORD_TRUE, KEYWORD_FALSE,

    KEYWORD_PRINT,

    TOKEN_ERROR, TOKEN_EOF,

    // Additional operators
    COLON, QUESTION, AMPERSAND, PIPE, CARET
};

struct SourceLocation {
    std::string filename;
    int line;
    int column;

    SourceLocation(const std::string& file, int l, int c)
        : filename(file), line(l), column(c) {}
};

struct Token {
    TokenType type;
    std::string lexeme;
    SourceLocation location;
    
    // Union-like storage for literal values
    std::variant<std::monostate, int, double, std::string, char> value;

    Token(TokenType t, const std::string& lex, const SourceLocation& loc)
        : type(t), lexeme(lex), location(loc) {}
        
    // Convenience constructors for literals
    Token(TokenType t, const std::string& lex, const SourceLocation& loc, int intVal)
        : type(t), lexeme(lex), location(loc), value(intVal) {}
        
    Token(TokenType t, const std::string& lex, const SourceLocation& loc, double floatVal)
        : type(t), lexeme(lex), location(loc), value(floatVal) {}
        
    Token(TokenType t, const std::string& lex, const SourceLocation& loc, const std::string& stringVal)
        : type(t), lexeme(lex), location(loc), value(stringVal) {}
        
    Token(TokenType t, const std::string& lex, const SourceLocation& loc, char charVal)
        : type(t), lexeme(lex), location(loc), value(charVal) {}
};

class Lexer {
public:
    Lexer(const std::string& source, const std::string& filename);
    
    Token nextToken();
    std::vector<Token> tokenize();

private:
    const std::string& source;
    const std::string filename;
    size_t start;
    size_t current;
    int line;
    int column;

    // Auxiliary methods
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
    
    // Keywords map
    static const std::map<std::string, TokenType> keywords;
};

} // namespace pebas

#endif // PEBAS_LEXER_H

