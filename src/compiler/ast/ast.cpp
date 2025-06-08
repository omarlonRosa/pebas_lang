#include "pebas/ast/ast.h"

namespace pebas {

// Implementação dos métodos accept() para o padrão Visitor

void LiteralExpr::accept(ASTVisitor& visitor) {
    visitor.visitLiteralExpr(*this);
}

void IdentifierExpr::accept(ASTVisitor& visitor) {
    visitor.visitIdentifierExpr(*this);
}

void UnaryExpr::accept(ASTVisitor& visitor) {
    visitor.visitUnaryExpr(*this);
}

void BinaryExpr::accept(ASTVisitor& visitor) {
    visitor.visitBinaryExpr(*this);
}

void GroupingExpr::accept(ASTVisitor& visitor) {
    visitor.visitGroupingExpr(*this);
}

void CallExpr::accept(ASTVisitor& visitor) {
    visitor.visitCallExpr(*this);
}

void AssignmentExpr::accept(ASTVisitor& visitor) {
    visitor.visitAssignmentExpr(*this);
}

void VariableDecl::accept(ASTVisitor& visitor) {
    visitor.visitVariableDecl(*this);
}

void FunctionDecl::accept(ASTVisitor& visitor) {
    visitor.visitFunctionDecl(*this);
}

void BlockStmt::accept(ASTVisitor& visitor) {
    visitor.visitBlockStmt(*this);
}

void IfStmt::accept(ASTVisitor& visitor) {
    visitor.visitIfStmt(*this);
}

void WhileStmt::accept(ASTVisitor& visitor) {
    visitor.visitWhileStmt(*this);
}

void ReturnStmt::accept(ASTVisitor& visitor) {
    visitor.visitReturnStmt(*this);
}

void ExpressionStmt::accept(ASTVisitor& visitor) {
    visitor.visitExpressionStmt(*this);
}

void Program::accept(ASTVisitor& visitor) {
    visitor.visitProgram(*this);
}

} // namespace pebas

