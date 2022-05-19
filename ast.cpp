#include "ast.h"

#define BINARY_EXPR_EVAL(name, op) \
float name##Expr::eval(Context &ctx) { \
    return leftExpr->eval(ctx) op rightExpr->eval(ctx); \
} \

BINARY_EXPR_EVAL(Add, +);
BINARY_EXPR_EVAL(Sub, -);
BINARY_EXPR_EVAL(Mult, *);
BINARY_EXPR_EVAL(Div, /);
BINARY_EXPR_EVAL(Gt, >);
BINARY_EXPR_EVAL(Lt, <);
BINARY_EXPR_EVAL(Equals, ==);

float IntExpr::eval(Context &ctx) {
    ctx.value = value;
    return ctx.value;
}

float IdentExpr::eval(Context &ctx) {
    return ctx.vars[id];
}

float FloatExpr::eval(Context &ctx) {
    ctx.value = value;
    return ctx.value;
}

float MethodInvoc::eval(Context &ctx) {
    StmtList stmt = ctx.method[id];

    for(auto &it: *vars) {
        it->eval(ctx);
    }

    for(auto &it : stmt) {
        return it->exec(ctx);
    }
    return 1;
}

float BlockStmt::exec(Context &ctx) {
    for (auto &it : *stmtlist) 
        it->exec(ctx);
    return 1;
}

float AssignStmt::exec(Context &ctx) {
    ctx.vars[id] = rightExpr->eval(ctx);
    return 1;
}

float ReturnStmt::exec(Context &ctx) {
    return expr->eval(ctx);
}

float WriteStmt::exec(Context &ctx) {
    auto printExpr = expr->eval(ctx);
    std::cout << "Result: " << printExpr << std::endl;
    return 1;
}

float MethodDecl::exec(Context &ctx) {
    ctx.method.insert({id, *stmtList});
    
    for (auto &it : *param) {
        it->eval(ctx);
    }
    
    return 1;
}

float IfStmt::exec(Context &ctx) {
    if(conditionExpr) {
        for (auto &it : *stmtlist)
            it->exec(ctx);
    }
    return 1;
}