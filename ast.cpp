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
    std::vector<std::string> variables;

    std::string var;
    for(auto &paramTable: *vars) {
        for (auto &varTable : ctx.vars) {
                var = varTable.first;
                ctx.vars[var] = paramTable->eval(ctx);
        }
    }

    std::cout << "Keys keep reassigning its value upon passing through parameter, atleast ir works." << std::endl;
    for (auto &it : ctx.vars) {
        std::cout << "Variable: " << it.first << ", Value: " << it.second << std::endl;
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
    std::cout << "Print Result: " << printExpr << std::endl;
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