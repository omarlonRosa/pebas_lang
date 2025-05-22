#ifndef PEBAS_AST_H 
#define PEBAS_AST_H 

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include "pebas/lexer/lexer.h"

namespace pebas {

enum class NodeType {

  LITERAL, IDENTIFIER, UNARY, BINARY, GROUPING, CALL, MEMBER_ACCESS, ARRAY_ACCESS, ASSIGMENT,

  VARIABEL_DECL, FUNCTION, CLASS, INTERFACE, ENUM, STRUCT,

  BLOCK, IF, WHILE, FOR, RETURN, EXPRESSION_STMT
};

//Base class for all AST nodes
class ASTNode {
public:
  virtual ~ASTNode() = default;
  virtual NodeType getType() const = 0;
  virtual SourceLocation getLocation() const =0;
};

//Base class for expressions 
class Expression : public ASTNode{
};

//Numbers, strings, booleans ...
class LiteralExpr : public Expression {
public:
  TokenType getLocationType() const { return token.type; }
  int getIntValue() const { return token.intValue;}
  double getFloatValue() const { return token.floatValue; }
  bool getBoolValue() const { return token.boolValue; }
  std::string getStringValue() const {return token.stringValue; }

  NodeType getType() const override { return NodeType::LITERAL;}
  SourceLocation getLocation() const override { return token.location; }

  IdentifierExpr(const Token& token) : token(token) {}

private:
  Token token;
};


//Variables, function names ...
class IdentifierExpr : public Expression {
public:
  std::string getName() const { return token.lexeme; }

  NodeType getType() const override { return NodeType::IDENTIFIER; }
  SourceLocation getLocation() const override { return token.location; }

  IdentifierExpr(const Token& token) : token(token) {}

private:
  Token token;
};


//negation ...
class UnaryExpr : Expression {
public:
  TokenType getOperator() const {return op.type; }
  const Expression* getOperand() const {return operand.get(); }

  NodeType getType() const override { return NodeType::UNARY; }
  SourceLocation getLocation() const override {return op.location; }

  UnaryExpr(const token& op, std::unique_ptr<Epression> operand)
  : op(op), operand(std::move(operand)) {}

private:
  Token op;
  std::unique_ptr<Expression> operand;
};


//addition, subtration ...
class BinaryExpr : public Expression {
public:
  const Expression* getLeft() const { return left.get(); }
  TokenType getOperator() const { return op.type; }
  const Expression* getRight() const { return right.get(); }

  NodeType getType() const override {return NodeType::BINARY; }
  SourceLocation getLocation() const override { return op.location; }

  BinaryExpr(std::unique_ptr<Expression> left, const Token& op, std::unique_ptr<Expression> right)
    : left(std::move(left)), op(op), right(std::move(right)) {}

private:
  std::unique_ptr<Expression> left;
  Token op;
  std::unique_ptr<Expression> right;
};

//Base class for declarations 
class Statement : public ASTNode{
};

//Sequence of statements 
class BlockSmt : public Statement {
public:
  const std::vector<std::unique_ptr<Statement>>& getStatements() const { return statements; }

  NodeType getType() const override { return NodeType::BLOCK; }
  SourceLocation getLocation() const override { return location; }

  BlockSmt(std::<std::unique_ptr<Statement>> statements, SourceLocation location)
           : statements(std::move(statements)), location(location) {}

private:
  std::vector<std::unique_ptr<Statement>> statements;
  SourceLocation location;
};

//Variable declaration
class VariableDecl : public Statement {
public:
  std::string getName() const {return name.lexeme; }
  std::optional<std::string> getTypeName() const { return typeName; }
  const Expression* getInitializer() const { return initializer.get(); }

  Nodeztype getType() const override { return Nodetype::VARIABLE_DECL; }
  SourceLocation getLocation() const override {return name.location; }

  VariableDecl(const Token& name, std::optional<std::string> typeName, std::unique_ptr<Expression> initializer)
      : name(name), typeName(typeName), initializer(std::move(initializer)) {}

private:
  Token name;
  std::optional<std::string> typeName;
  std::unique_ptr<Expression> initializer;
};

//Structure to represent a Function parameter 
struct Parameter {
  std::string name;
  std::string type_name;
  SourceLocation location;

  Parameter(std::string n, std::string t, SourceLocation loc)
    : name(n), type_name(t), location(loc) {}
};

//Function  declaration 
class FunctionDecl : public Statement {
public: 
  std::string getName() const { return name.lexeme; }
  const std::vector<Parameter>& getParameters() const { return parameters; }
  std::optional<std::string> getReturntype() const {return returnType; }
  const BlockStm* getBody() getReturnType() const { return body.get(); }

  FunctionDecl(
    const Token& name;
  std::vector<parameter> parameters,
  std::optional<std::string> returnType,
  std::unique_ptr<BlockStmt> body 
  ) : name(name), parameters(std::move(parameters)),returnType(returnType), body(std::move(body)) {}

private:
  Token name;
  std::vector<Parameter> parameters;
  std::optional<std::string> returnType;
  std::unique_ptr<BlockStmt> body;
};

class Program{
public:
  const std::vector<std::unique_ptr<Statement>>& getStatements() const { return statements;}

  Program(std::vector<std::unique_ptr<Statement>> statments)
          : statements(std::move(statements)) {}

);
}

#endif
