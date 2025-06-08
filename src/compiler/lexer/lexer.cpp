#include "pebas/lexer/lexer.h"
#include <unordered_map>

namespace pebas {

// Keywords map initialization
const std::map<std::string, TokenType> Lexer::keywords = {
    {"class", TokenType::KEYWORD_CLASS},
    {"interface", TokenType::KEYWORD_INTERFACE},
    {"enum", TokenType::KEYWORD_ENUM},
    {"struct", TokenType::KEYWORD_STRUCT},
    {"function", TokenType::KEYWORD_FUNCTION},
    {"var", TokenType::KEYWORD_VAR},
    {"const", TokenType::KEYWORD_CONST},
    {"if", TokenType::KEYWORD_IF},
    {"else", TokenType::KEYWORD_ELSE},
    {"switch", TokenType::KEYWORD_SWITCH},
    {"case", TokenType::KEYWORD_CASE},
    {"for", TokenType::KEYWORD_FOR},
    {"while", TokenType::KEYWORD_WHILE},
    {"do", TokenType::KEYWORD_DO},
    {"break", TokenType::KEYWORD_BREAK},
    {"continue", TokenType::KEYWORD_CONTINUE},
    {"return", TokenType::KEYWORD_RETURN},
    {"try", TokenType::KEYWORD_TRY},
    {"catch", TokenType::KEYWORD_CATCH},
    {"throw", TokenType::KEYWORD_THROW},
    {"public", TokenType::KEYWORD_PUBLIC},
    {"private", TokenType::KEYWORD_PRIVATE},
    {"protected", TokenType::KEYWORD_PROTECTED},
    {"static", TokenType::KEYWORD_STATIC},
    {"abstract", TokenType::KEYWORD_ABSTRACT},
    {"override", TokenType::KEYWORD_OVERRIDE},
    {"virtual", TokenType::KEYWORD_VIRTUAL},
    {"import", TokenType::KEYWORD_IMPORT},
    {"package", TokenType::KEYWORD_PACKAGE},
    {"new", TokenType::KEYWORD_NEW},
    {"this", TokenType::KEYWORD_THIS},
    {"super", TokenType::KEYWORD_SUPER},
    {"as", TokenType::KEYWORD_AS},
    {"is", TokenType::KEYWORD_IS},
    {"true", TokenType::KEYWORD_TRUE},
    {"false", TokenType::KEYWORD_FALSE},
    {"null", TokenType::KEYWORD_NULL},
    {"print", TokenType::KEYWORD_PRINT}
};

Lexer::Lexer(const std::string& source, const std::string& filename)
    : source(source), filename(filename), start(0), current(0), line(1), column(1) {}

Token Lexer::nextToken() {
    skipWhitespace();

    start = current;

    if (isAtEnd()) return makeToken(TokenType::TOKEN_EOF);

    char c = advance();

    // Check comments 
    if (c == '#') {
        if (peek() == '*') { // multiline comment
            advance(); // consume '*'
            skipComment();
            return nextToken();
        } else { // single line comment 
            while (peek() != '\n' && !isAtEnd()) advance();
            return nextToken();
        }
    }

    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();
    if (c == '"') return string();
    if (c == '\'') return character();

    switch (c) {
        case '(': return makeToken(TokenType::LEFT_PAREN);
        case ')': return makeToken(TokenType::RIGHT_PAREN);
        case '{': return makeToken(TokenType::LEFT_BRACE);
        case '}': return makeToken(TokenType::RIGHT_BRACE);
        case '[': return makeToken(TokenType::LEFT_BRACKET);
        case ']': return makeToken(TokenType::RIGHT_BRACKET);
        case ';': return makeToken(TokenType::SEMICOLON);
        case ':': 
            return match(':') ? makeToken(TokenType::COLON_COLON) : makeToken(TokenType::COLON);
        case ',': return makeToken(TokenType::COMMA);
        case '.': 
            return match('.') ? makeToken(TokenType::DOT_DOT) : makeToken(TokenType::DOT);
        case '~': return makeToken(TokenType::TILDE);
        case '?': return makeToken(TokenType::QUESTION);

        // Operators that can be combined
        case '+': 
            return match('=') ? makeToken(TokenType::PLUS_ASSIGN) : makeToken(TokenType::PLUS);
        case '-':
            if (match('>')) return makeToken(TokenType::ARROW);
            if (match('=')) return makeToken(TokenType::MINUS_ASSIGN);
            return makeToken(TokenType::MINUS);
        case '*': 
            return match('=') ? makeToken(TokenType::STAR_ASSIGN) : makeToken(TokenType::STAR);
        case '/': return makeToken(TokenType::SLASH);
        case '%': 
            return match('=') ? makeToken(TokenType::PERCENT_ASSIGN) : makeToken(TokenType::PERCENT);

        case '=':
            if (match('=')) return makeToken(TokenType::EQUAL_EQUAL);
            if (match('>')) return makeToken(TokenType::DOUBLE_ARROW_RIGHT);
            return makeToken(TokenType::EQUAL);
        
        case '!':
            return match('=') ? makeToken(TokenType::BANG_EQUAL) : makeToken(TokenType::BANG);

        case '<':
            if (match('=')) return makeToken(TokenType::LESS_EQUAL);
            if (match('<')) {
                return match('=') ? makeToken(TokenType::LESS_LESS_ASSIGN) : makeToken(TokenType::ARROW_RIGHT);
            }
            return makeToken(TokenType::LESS);
        
        case '>':
            if (match('=')) return makeToken(TokenType::GREATER_EQUAL);
            if (match('>')) {
                return match('=') ? makeToken(TokenType::GREATER_GREATER_ASSIGN) : makeToken(TokenType::DOUBLE_ARROW_RIGHT_OP);
            }
            return makeToken(TokenType::GREATER);
        
        case '&':
            if (match('&')) return makeToken(TokenType::AND_AND);
            if (match('=')) return makeToken(TokenType::AMPERSAND_ASSIGN);
            return makeToken(TokenType::AMPERSAND);
 
        case '|':
            if (match('|')) return makeToken(TokenType::OR_OR);
            if (match('=')) return makeToken(TokenType::PIPE_ASSIGN);
            return makeToken(TokenType::PIPE);

        case '^':
            return match('=') ? makeToken(TokenType::CARET_ASSIGN) : makeToken(TokenType::CARET);
    }
    
    return errorToken("Unexpected character.");
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        Token token = nextToken();
        tokens.push_back(token);

        if (token.type == TokenType::TOKEN_EOF) {
            break;
        }
    }

    return tokens;
}

// Auxiliary methods
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
        column++;
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
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

void Lexer::skipWhitespace() {
    while (true) {
        char c = peek();

        switch (c) {
            case ' ':
            case '\t':
            case '\r':
                advance();
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
    // Read until "*#" is found (close multi-line comment)
    while (!(peek() == '*' && peekNext() == '#') && !isAtEnd()) {
        if (peek() == '\n') {
            line++;
            column = 1;
        }
        advance();
    }
    
    if (!isAtEnd()) {
        advance(); // *
        advance(); // #
    }
}

Token Lexer::identifier() {
    while (isAlphaNumeric(peek())) advance();

    // Check if it is a keyword 
    std::string text = source.substr(start, current - start);
    TokenType type = identifierType();

    if (type == TokenType::IDENTIFIER) {
        return Token(TokenType::IDENTIFIER, text, SourceLocation(filename, line, column - (current - start)), text);
    }
    
    return makeToken(type);
}

Token Lexer::number() {
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())) {
        advance(); // consume '.'

        while (isDigit(peek())) advance();

        std::string value = source.substr(start, current - start);
        double floatValue = std::stod(value);
        return Token(TokenType::FLOAT_LITERAL, value, SourceLocation(filename, line, column - (current - start)), floatValue);
    }

    std::string value = source.substr(start, current - start);
    int intValue = std::stoi(value);
    return Token(TokenType::INTEGER_LITERAL, value, SourceLocation(filename, line, column - (current - start)), intValue);
}

Token Lexer::string() {
    // Skip initial quote 
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            line++;
            column = 1;
        } else if (peek() == '\\' && peekNext() == '"') {
            advance(); // consume backslash
        }
        advance();
    }

    if (isAtEnd()) return errorToken("Unterminated string.");

    advance(); // consume closing quote

    std::string value = source.substr(start + 1, current - start - 2);
    return Token(TokenType::STRING, value, SourceLocation(filename, line, column - (current - start)), value);
}

Token Lexer::character() {
    if (peek() == '\\') {
        advance(); // consume backslash
        advance(); // consume escaped character
    } else {
        advance(); // consume character
    }

    if (peek() != '\'') return errorToken("Expected '\\'' to close character.");
    advance(); // consume closing quote

    std::string value = source.substr(start + 1, current - start - 2);
    char charValue = value.empty() ? '\0' : value[0]; // Handle escaped characters properly in a real implementation
    return Token(TokenType::CHAR_LITERAL, value, SourceLocation(filename, line, column - (current - start)), charValue);
}

Token Lexer::makeToken(TokenType type) {
    std::string lexeme = source.substr(start, current - start);
    SourceLocation loc(filename, line, column - (current - start));
    return Token(type, lexeme, loc);
}

Token Lexer::errorToken(const std::string& message) {
    SourceLocation loc(filename, line, column - (current - start));
    return Token(TokenType::TOKEN_ERROR, message, loc);
}

TokenType Lexer::identifierType() {
    std::string text = source.substr(start, current - start);
    auto it = keywords.find(text);

    if (it != keywords.end()) {
        return it->second;
    }
    
    return TokenType::IDENTIFIER;
}

} // namespace pebas

