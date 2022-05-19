#ifndef _AST_H
#define _AST_H

#include <iostream>
#include <memory>
#include <list>
#include <unordered_map>

class Expr;
class Stmt;

typedef Expr *ExprPtr;
typedef Stmt *StmtPtr;

typedef std::list<Expr *> ArgumentList;
typedef std::list<Stmt *> StmtList;

class Context {
public:
    explicit Context() {}
    float value;
    std::unordered_map<std::string, float> vars;
    std::unordered_map<std::string, std::list<Stmt *>> method;
};

class AstNode {
public:
    AstNode() {}
    virtual ~AstNode() {}
};

class Expr: public AstNode {
public:
    Expr() {}
    virtual ~Expr() {}
    virtual float eval(Context &ctx) = 0;
};

class IntExpr : public Expr {
public:
    IntExpr(float value) : value(value) {}
    
    float value;
    float eval(Context &ctx) override;
};

class IdentExpr : public Expr {
public:
    IdentExpr(std::string id) : id(id) {}
    
    std::string id;
    float eval(Context &ctx) override;
};

class FloatExpr : public Expr {
public:
    FloatExpr(float value) : value(value) {}
    
    float value;
    float eval(Context &ctx) override;
};

class MethodInvoc : public Expr {
public:
    MethodInvoc(std::string id, ArgumentList *vars) : id(id), vars(vars) {}
    std::string id;
    ArgumentList *vars;
    float eval(Context &ctx) override;
};

#define BINARY_EXPR(name) \
class name##Expr: public Expr { \
public: \
    name##Expr(ExprPtr leftExpr, ExprPtr rightExpr) : leftExpr(std::move(leftExpr)), rightExpr(std::move(rightExpr)) {} \
    ExprPtr leftExpr, rightExpr; \
    float eval(Context &ctx) override; \
}; \

BINARY_EXPR(Add);
BINARY_EXPR(Sub);
BINARY_EXPR(Mult);
BINARY_EXPR(Div);
BINARY_EXPR(Gt);
BINARY_EXPR(Lt);
BINARY_EXPR(Equals);

class Stmt : public AstNode {
public:
    Stmt() {}
    virtual ~Stmt() {}
    virtual float exec(Context &ctx) = 0;
    bool isTrue;
};

class BlockStmt : public Stmt {
public:
    BlockStmt(StmtList *stmtlist) : stmtlist(stmtlist) { }
    StmtList *stmtlist;
    float exec(Context &ctx) override;
    bool isTrue;
};

class AssignStmt : public Stmt {
public:
    AssignStmt(std::string id, ExprPtr rightExpr) : id(id), rightExpr(rightExpr) {}
    std::string id;
    ExprPtr rightExpr;
    float exec(Context &ctx) override;
};

class WriteStmt : public Stmt {
public:
    WriteStmt(ExprPtr expr) : expr(expr) {}
    ExprPtr expr;
    float exec(Context &ctx) override;
};

class ReturnStmt : public Stmt {
public:
    ReturnStmt(ExprPtr expr) : expr(expr) {}
    ExprPtr expr;
    float exec(Context &ctx) override;
};

class MethodDecl : public Stmt {
public:
    MethodDecl(std::string id, ArgumentList *param, StmtList *stmtList) : id(id), stmtList(stmtList), param(param) {}
    StmtList *stmtList;
    ArgumentList *param;
    std::string id;
    float exec(Context &ctx) override;
};

class IfStmt : public Stmt {
public:
    IfStmt(ExprPtr conditionExpr, StmtList *stmtlist) : conditionExpr(conditionExpr), stmtlist(stmtlist) {}
    ExprPtr conditionExpr;
    StmtList *stmtlist;
    float exec(Context &ctx) override;
};

#endif