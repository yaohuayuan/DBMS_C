//
// Created by yaohuayuan on 2024/11/28.
//

#include "Term.h"
#include "Plan.h"
#include "math.h"
#define max(a, b) ((a) > (b) ? (a) : (b))

Term *TermInit(Expression*lhs,Expression*rhs){
    Term *term = malloc(sizeof (Term));
    term->lhs = lhs;
    term->rhs = rhs;
    return term;
}
bool TermIsSatisfied(Term *term,Scan *s){
    Constant *lhsval = ExpressionEvaluate(term->lhs,s);
    Constant *rhsval = ExpressionEvaluate(term->rhs,s);
    return ConstantEquals(rhsval,lhsval);
}
int TermReductionFactor(Term*term,Plan*plan){
    char *lhsName,*rhsName;
    if(ExpressionIsFieldName(term->rhs)&&ExpressionIsFieldName(term->lhs)){
        lhsName = strdup(ExpressionAsFieldName(term->lhs));
        rhsName = strdup(ExpressionAsFieldName(term->rhs));
        return max(plan->distinctValues(plan,lhsName),
                   plan->distinctValues(plan,rhsName));
    }
    if(ExpressionIsFieldName(term->rhs)){
        rhsName = strdup(ExpressionAsFieldName(term->rhs));
        return plan->distinctValues(plan,rhsName);
    }
    if(ExpressionIsFieldName(term->lhs)){
        lhsName = strdup(ExpressionAsFieldName(term->lhs));
        return plan->distinctValues(plan,lhsName);
    }
    if(ConstantEquals(ExpressionAsConstant(term->lhs),
                      ExpressionAsConstant(term->rhs))){
        return 1;
    }else
        return INT_MAX;
}
Constant *TermEquatesWithConstant(Term*term,char *fldname){
    if(ExpressionIsFieldName(term->lhs)&&
        strcmp(ExpressionAsFieldName(term->lhs),fldname)==0&&
        !ExpressionIsFieldName(term->rhs)){
        return ExpressionAsConstant(term->rhs);

    }else if(ExpressionIsFieldName(term->rhs)&& strcmp(ExpressionAsFieldName(term->rhs),fldname)==0&&!ExpressionIsFieldName(term->lhs)){
        return ExpressionAsConstant(term->lhs);
    }else{
        return NULL;
    }
}
char *TermEquatesWithField(Term*term,char *fldname){
    if(ExpressionIsFieldName(term->lhs)&&
        strcmp(ExpressionAsFieldName(term->lhs),fldname)==0
        &&ExpressionIsFieldName(term->rhs)){
        return ExpressionAsFieldName(term->rhs);

    }else if(ExpressionIsFieldName(term->rhs)&&
            strcmp(ExpressionAsFieldName(term->rhs),fldname)==0&&
            ExpressionIsFieldName(term->lhs)){
        return ExpressionAsFieldName(term->lhs);
    }else{
        return NULL;
    }
}
bool TermAppliesTo(Term*term,Schema*schema){
    return ExpressionAppliesTo(term->lhs,schema)&&
            ExpressionAppliesTo(term->rhs,schema);
}
char *TermToString(Term*term){
    char str[512];
    sprintf(str,"%s=%s",ExpressionToString(term->lhs),ExpressionToString(term->rhs));
    return strdup(str);
}