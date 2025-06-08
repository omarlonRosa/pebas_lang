#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "pebas/lexer/lexer.h"
#include "pebas/parser/parser.h"
#include "pebas/ast/ast.h"

// Simple AST printer for debugging
class ASTPrinter : public pebas::ASTVisitor {
private:
    int indentLevel = 0;
    
    void indent() {
        for (int i = 0; i < indentLevel; ++i) {
            std::cout << "  ";
        }
    }
    
public:
    void visitLiteralExpr(pebas::LiteralExpr& expr) override {
        indent();
        std::cout << "Literal: " << expr.token.lexeme << std::endl;
    }
    
    void visitIdentifierExpr(pebas::IdentifierExpr& expr) override {
        indent();
        std::cout << "Identifier: " << expr.getName() << std::endl;
    }
    
    void visitUnaryExpr(pebas::UnaryExpr& expr) override {
        indent();
        std::cout << "Unary: " << expr.operator_.lexeme << std::endl;
        indentLevel++;
        expr.operand->accept(*this);
        indentLevel--;
    }
    
    void visitBinaryExpr(pebas::BinaryExpr& expr) override {
        indent();
        std::cout << "Binary: " << expr.operator_.lexeme << std::endl;
        indentLevel++;
        expr.left->accept(*this);
        expr.right->accept(*this);
        indentLevel--;
    }
    
    void visitGroupingExpr(pebas::GroupingExpr& expr) override {
        indent();
        std::cout << "Grouping:" << std::endl;
        indentLevel++;
        expr.expression->accept(*this);
        indentLevel--;
    }
    
    void visitCallExpr(pebas::CallExpr& expr) override {
        indent();
        std::cout << "Call:" << std::endl;
        indentLevel++;
        expr.callee->accept(*this);
        for (auto& arg : expr.arguments) {
            arg->accept(*this);
        }
        indentLevel--;
    }
    
    void visitAssignmentExpr(pebas::AssignmentExpr& expr) override {
        indent();
        std::cout << "Assignment: " << expr.name.lexeme << std::endl;
        indentLevel++;
        expr.value->accept(*this);
        indentLevel--;
    }
    
    void visitVariableDecl(pebas::VariableDecl& stmt) override {
        indent();
        std::cout << "VarDecl: " << stmt.name.lexeme;
        if (stmt.typeName) {
            std::cout << " : " << *stmt.typeName;
        }
        std::cout << std::endl;
        if (stmt.initializer) {
            indentLevel++;
            stmt.initializer->accept(*this);
            indentLevel--;
        }
    }
    
    void visitFunctionDecl(pebas::FunctionDecl& stmt) override {
        indent();
        std::cout << "FunctionDecl: " << stmt.name.lexeme << "(";
        for (size_t i = 0; i < stmt.parameters.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << stmt.parameters[i].name << ": " << stmt.parameters[i].type;
        }
        std::cout << ")";
        if (stmt.returnType) {
            std::cout << " -> " << *stmt.returnType;
        }
        std::cout << std::endl;
        indentLevel++;
        stmt.body->accept(*this);
        indentLevel--;
    }
    
    void visitBlockStmt(pebas::BlockStmt& stmt) override {
        indent();
        std::cout << "Block:" << std::endl;
        indentLevel++;
        for (auto& statement : stmt.statements) {
            statement->accept(*this);
        }
        indentLevel--;
    }
    
    void visitIfStmt(pebas::IfStmt& stmt) override {
        indent();
        std::cout << "If:" << std::endl;
        indentLevel++;
        stmt.condition->accept(*this);
        stmt.thenBranch->accept(*this);
        if (stmt.elseBranch) {
            stmt.elseBranch->accept(*this);
        }
        indentLevel--;
    }
    
    void visitWhileStmt(pebas::WhileStmt& stmt) override {
        indent();
        std::cout << "While:" << std::endl;
        indentLevel++;
        stmt.condition->accept(*this);
        stmt.body->accept(*this);
        indentLevel--;
    }
    
    void visitReturnStmt(pebas::ReturnStmt& stmt) override {
        indent();
        std::cout << "Return:" << std::endl;
        if (stmt.value) {
            indentLevel++;
            stmt.value->accept(*this);
            indentLevel--;
        }
    }
    
    void visitExpressionStmt(pebas::ExpressionStmt& stmt) override {
        indent();
        std::cout << "ExpressionStmt:" << std::endl;
        if (stmt.expression) {
            indentLevel++;
            stmt.expression->accept(*this);
            indentLevel--;
        }
    }
    
    void visitProgram(pebas::Program& program) override {
        std::cout << "Program:" << std::endl;
        indentLevel++;
        for (auto& statement : program.statements) {
            statement->accept(*this);
        }
        indentLevel--;
    }
};

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options] <source_file>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h, --help     Show this help message" << std::endl;
    std::cout << "  -t, --tokens   Print tokens only" << std::endl;
    std::cout << "  -a, --ast      Print AST only" << std::endl;
    std::cout << "  -v, --verbose  Verbose output" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    bool printTokens = false;
    bool printAST = false;
    bool verbose = false;
    std::string sourceFile;
    
    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-t" || arg == "--tokens") {
            printTokens = true;
        } else if (arg == "-a" || arg == "--ast") {
            printAST = true;
        } else if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        } else if (arg[0] != '-') {
            sourceFile = arg;
        } else {
            std::cerr << "Unknown option: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    
    if (sourceFile.empty()) {
        std::cerr << "Error: No source file specified" << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    try {
        // Read source file
        if (verbose) {
            std::cout << "Reading file: " << sourceFile << std::endl;
        }
        std::string source = readFile(sourceFile);
        
        // Lexical analysis
        if (verbose) {
            std::cout << "Starting lexical analysis..." << std::endl;
        }
        pebas::Lexer lexer(source, sourceFile);
        std::vector<pebas::Token> tokens = lexer.tokenize();
        
        if (printTokens || verbose) {
            std::cout << "\n=== TOKENS ===" << std::endl;
            for (const auto& token : tokens) {
                std::cout << "Token: " << token.lexeme 
                         << " (Line: " << token.location.line 
                         << ", Col: " << token.location.column << ")" << std::endl;
            }
        }
        
        // Syntax analysis
        if (verbose) {
            std::cout << "\nStarting syntax analysis..." << std::endl;
        }
        pebas::Parser parser(tokens);
        auto program = parser.parse();
        
        if (printAST || verbose) {
            std::cout << "\n=== AST ===" << std::endl;
            ASTPrinter printer;
            program->accept(printer);
        }
        
        if (verbose) {
            std::cout << "\nCompilation completed successfully!" << std::endl;
        }
        
    } catch (const pebas::ParseError& e) {
        std::cerr << "Parse error at line " << e.location.line 
                  << ", column " << e.location.column 
                  << ": " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

