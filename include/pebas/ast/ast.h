#ifndef PEBAS_AST_H
#define PEBAS_AST_H

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include "pebas/lexer/lexer.h"

namespace pebas {

enum class NodeType {
    // Literals
    LITERAL, IDENTIFIER, 
    
    // Expressions
    UNARY, BINARY, GROUPING, CALL, MEMBER_ACCESS, ARRAY_ACCESS, ASSIGNMENT,
    
    // Declarations
    VARIABLE_DECL, FUNCTION, CLASS, INTERFACE, ENUM, STRUCT,
    
    // Statements
    BLOCK, IF, WHILE, FOR, RETURN, EXPRESSION_STMT, BREAK, CONTINUE,
    
    // Program
    PROGRAM
};

// Forward declarations
class ASTVisitor;

// Base class for all AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual NodeType getType() const = 0;
    virtual SourceLocation getLocation() const = 0;
    virtual void accept(ASTVisitor& visitor) = 0;
};

// Base class for expressions
class Expression : public ASTNode {
public:
    virtual ~Expression() = default;
};

// Base class for statements
class Statement : public ASTNode {
public:
    virtual ~Statement() = default;
};

// Literal expressions (numbers, strings, booleans)
class LiteralExpr : public Expression {
public:
    Token token;
    
    LiteralExpr(const Token& token) : token(token) {}
    
    NodeType getType() const override { return NodeType::LITERAL; }
    SourceLocation getLocation() const override { return token.location; }
    void accept(ASTVisitor& visitor) override;
};

// Identifier expressions (variable names, function names)
class IdentifierExpr : public Expression {
public:
    Token token;
    
    IdentifierExpr(const Token& token) : token(token) {}
    
    std::string getName() const { return token.lexeme; }
    NodeType getType() const override { return NodeType::IDENTIFIER; }
    SourceLocation getLocation() const override { return token.location; }
    void accept(ASTVisitor& visitor) override;
};

// Unary expressions (negation, logical not)
class UnaryExpr : public Expression {
public:
    Token operator_;
    std::unique_ptr<Expression> operand;
    
    UnaryExpr(const Token& op, std::unique_ptr<Expression> operand)
        : operator_(op), operand(std::move(operand)) {}
    
    NodeType getType() const override { return NodeType::UNARY; }
    SourceLocation getLocation() const override { return operator_.location; }
    void accept(ASTVisitor& visitor) override;
};

// Binary expressions (arithmetic, comparison, logical)
class BinaryExpr : public Expression {
public:
    std::unique_ptr<Expression> left;
    Token operator_;
    std::unique_ptr<Expression> right;
    
    BinaryExpr(std::unique_ptr<Expression> left, const Token& op, std::unique_ptr<Expression> right)
        : left(std::move(left)), operator_(op), right(std::move(right)) {}
    
    NodeType getType() const override { return NodeType::BINARY; }
    SourceLocation getLocation() const override { return operator_.location; }
    void accept(ASTVisitor& visitor) override;
};

// Grouping expressions (parentheses)
class GroupingExpr : public Expression {
public:
    std::unique_ptr<Expression> expression;
    SourceLocation location;
    
    GroupingExpr(std::unique_ptr<Expression> expr, const SourceLocation& loc)
        : expression(std::move(expr)), location(loc) {}
    
    NodeType getType() const override { return NodeType::GROUPING; }
    SourceLocation getLocation() const override { return location; }
    void accept(ASTVisitor& visitor) override;
};

// Function call expressions
class CallExpr : public Expression {
public:
    std::unique_ptr<Expression> callee;
    std::vector<std::unique_ptr<Expression>> arguments;
    SourceLocation location;
    
    CallExpr(std::unique_ptr<Expression> callee, std::vector<std::unique_ptr<Expression>> args, const SourceLocation& loc)
        : callee(std::move(callee)), arguments(std::move(args)), location(loc) {}
    
    NodeType getType() const override { return NodeType::CALL; }
    SourceLocation getLocation() const override { return location; }
    void accept(ASTVisitor& visitor) override;
};

// Assignment expressions
class AssignmentExpr : public Expression {
public:
    Token name;
    std::unique_ptr<Expression> value;
    
    AssignmentExpr(const Token& name, std::unique_ptr<Expression> value)
        : name(name), value(std::move(value)) {}
    
    NodeType getType() const override { return NodeType::ASSIGNMENT; }
    SourceLocation getLocation() const override { return name.location; }
    void accept(ASTVisitor& visitor) override;
};

// Parameter for function declarations
struct Parameter {
    std::string name;
    std::string type;
    SourceLocation location;
    
    Parameter(const std::string& name, const std::string& type, const SourceLocation& loc)
        : name(name), type(type), location(loc) {}
};

// Variable declarations
class VariableDecl : public Statement {
public:
    Token name;
    std::optional<std::string> typeName;
    std::unique_ptr<Expression> initializer;
    
    VariableDecl(const Token& name, const std::optional<std::string>& type, std::unique_ptr<Expression> init)
        : name(name), typeName(type), initializer(std::move(init)) {}
    
    NodeType getType() const override { return NodeType::VARIABLE_DECL; }
    SourceLocation getLocation() const override { return name.location; }
    void accept(ASTVisitor& visitor) override;
};

// Block statements
class BlockStmt : public Statement {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    SourceLocation location;
    
    BlockStmt(std::vector<std::unique_ptr<Statement>> stmts, const SourceLocation& loc)
        : statements(std::move(stmts)), location(loc) {}
    
    NodeType getType() const override { return NodeType::BLOCK; }
    SourceLocation getLocation() const override { return location; }
    void accept(ASTVisitor& visitor) override;
};

// Function declarations
class FunctionDecl : public Statement {
public:
    Token name;
    std::vector<Parameter> parameters;
    std::optional<std::string> returnType;
    std::unique_ptr<BlockStmt> body;
    
    FunctionDecl(const Token& name, std::vector<Parameter> params, 
                 const std::optional<std::string>& returnType, std::unique_ptr<BlockStmt> body)
        : name(name), parameters(std::move(params)), returnType(returnType), body(std::move(body)) {}
    
    NodeType getType() const override { return NodeType::FUNCTION; }
    SourceLocation getLocation() const override { return name.location; }
    void accept(ASTVisitor& visitor) override;
};

// If statements
class IfStmt : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> thenBranch;
    std::unique_ptr<Statement> elseBranch;
    SourceLocation location;
    
    IfStmt(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> thenBranch,
           std::unique_ptr<Statement> elseBranch, const SourceLocation& loc)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)), 
          elseBranch(std::move(elseBranch)), location(loc) {}
    
    NodeType getType() const override { return NodeType::IF; }
    SourceLocation getLocation() const override { return location; }
    void accept(ASTVisitor& visitor) override;
};

// While statements
class WhileStmt : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> body;
    SourceLocation location;
    
    WhileStmt(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> body, const SourceLocation& loc)
        : condition(std::move(condition)), body(std::move(body)), location(loc) {}
    
    NodeType getType() const override { return NodeType::WHILE; }
    SourceLocation getLocation() const override { return location; }
    void accept(ASTVisitor& visitor) override;
};

// Return statements
class ReturnStmt : public Statement {
public:
    Token keyword;
    std::unique_ptr<Expression> value;
    
    ReturnStmt(const Token& keyword, std::unique_ptr<Expression> value)
        : keyword(keyword), value(std::move(value)) {}
    
    NodeType getType() const override { return NodeType::RETURN; }
    SourceLocation getLocation() const override { return keyword.location; }
    void accept(ASTVisitor& visitor) override;
};

// Expression statements
class ExpressionStmt : public Statement {
public:
    std::unique_ptr<Expression> expression;
    SourceLocation location;
    
    ExpressionStmt(std::unique_ptr<Expression> expr, const SourceLocation& loc)
        : expression(std::move(expr)), location(loc) {}
    
    NodeType getType() const override { return NodeType::EXPRESSION_STMT; }
    SourceLocation getLocation() const override { return location; }
    void accept(ASTVisitor& visitor) override;
};

// Program (root of AST)
class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    SourceLocation location;
    
    Program(std::vector<std::unique_ptr<Statement>> stmts)
        : statements(std::move(stmts)), location("", 1, 1) {}
    
    NodeType getType() const override { return NodeType::PROGRAM; }
    SourceLocation getLocation() const override { return location; }
    void accept(ASTVisitor& visitor) override;
};

// Visitor pattern interface
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    virtual void visitLiteralExpr(LiteralExpr& expr) = 0;
    virtual void visitIdentifierExpr(IdentifierExpr& expr) = 0;
    virtual void visitUnaryExpr(UnaryExpr& expr) = 0;
    virtual void visitBinaryExpr(BinaryExpr& expr) = 0;
    virtual void visitGroupingExpr(GroupingExpr& expr) = 0;
    virtual void visitCallExpr(CallExpr& expr) = 0;
    virtual void visitAssignmentExpr(AssignmentExpr& expr) = 0;
    
    virtual void visitVariableDecl(VariableDecl& stmt) = 0;
    virtual void visitFunctionDecl(FunctionDecl& stmt) = 0;
    virtual void visitBlockStmt(BlockStmt& stmt) = 0;
    virtual void visitIfStmt(IfStmt& stmt) = 0;
    virtual void visitWhileStmt(WhileStmt& stmt) = 0;
    virtual void visitReturnStmt(ReturnStmt& stmt) = 0;
    virtual void visitExpressionStmt(ExpressionStmt& stmt) = 0;
    virtual void visitProgram(Program& program) = 0;
};

} // namespace pebas

#endif // PEBAS_AST_H

