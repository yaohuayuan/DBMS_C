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
    CString *lhsName,*rhsName;
    if(ExpressionIsFieldName(term->rhs)&&ExpressionIsFieldName(term->lhs)){
        lhsName = ExpressionAsCStringFieldName(term->lhs);
        rhsName = ExpressionAsCStringFieldName(term->rhs);
        return max(plan->distinctValues(plan,lhsName),
                   plan->distinctValues(plan,rhsName));
    }
    if(ExpressionIsFieldName(term->rhs)){
        rhsName = ExpressionAsCStringFieldName(term->rhs);
        return plan->distinctValues(plan,rhsName);
    }
    if(ExpressionIsFieldName(term->lhs)){
        lhsName = ExpressionAsCStringFieldName(term->lhs);
        return plan->distinctValues(plan,lhsName);
    }
    if(ConstantEquals(ExpressionAsConstant(term->lhs),
                      ExpressionAsConstant(term->rhs))){
        return 1;
    }else
        return INT_MAX;
}
Constant *TermEquatesWithConstant(Term*term,CString *fldname){
    if(ExpressionIsFieldName(term->lhs)&&
        CStringEqual(ExpressionAsCStringFieldName(term->lhs),fldname)&&
        !ExpressionIsFieldName(term->rhs)){
        return ExpressionAsConstant(term->rhs);

    }else if(ExpressionIsFieldName(term->rhs)&& CStringEqual(ExpressionAsCStringFieldName(term->rhs),fldname)&&!ExpressionIsFieldName(term->lhs)){
        return ExpressionAsConstant(term->lhs);
    }else{
        return NULL;
    }
}
CString *TermEquatesWithField(Term*term,CString *fldname){
    if(ExpressionIsFieldName(term->lhs)&&
        CStringEqual(ExpressionAsCStringFieldName(term->lhs),fldname)
        &&ExpressionIsFieldName(term->rhs)){
        return ExpressionAsCStringFieldName(term->rhs);

    }else if(ExpressionIsFieldName(term->rhs)&&
            CStringEqual(ExpressionAsCStringFieldName(term->rhs),fldname)&&
            ExpressionIsFieldName(term->lhs)){
        return ExpressionAsCStringFieldName(term->lhs);
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

bool TermEquals(Term *term1, Term *term2) {
    // 检查两个Term是否都为NULL
    if (term1 == NULL && term2 == NULL) {
        return true;
    }
    if (term1 == NULL || term2 == NULL) {
        return false;
    }
    
    // 检查lhs是否相等
    bool lhsEqual = false;
    bool rhsEqual = false;
    
    // 比较lhs
    if (ExpressionIsFieldName(term1->lhs) && ExpressionIsFieldName(term2->lhs)) {
        lhsEqual = CStringEqual(
            ExpressionAsCStringFieldName(term1->lhs),
            ExpressionAsCStringFieldName(term2->lhs)
        );
    } else if (!ExpressionIsFieldName(term1->lhs) && !ExpressionIsFieldName(term2->lhs)) {
        lhsEqual = ConstantEquals(
            ExpressionAsConstant(term1->lhs),
            ExpressionAsConstant(term2->lhs)
        );
    }
    
    // 比较rhs
    if (ExpressionIsFieldName(term1->rhs) && ExpressionIsFieldName(term2->rhs)) {
        rhsEqual = CStringEqual(
            ExpressionAsCStringFieldName(term1->rhs),
            ExpressionAsCStringFieldName(term2->rhs)
        );
    } else if (!ExpressionIsFieldName(term1->rhs) && !ExpressionIsFieldName(term2->rhs)) {
        rhsEqual = ConstantEquals(
            ExpressionAsConstant(term1->rhs),
            ExpressionAsConstant(term2->rhs)
        );
    }
    
    return lhsEqual && rhsEqual;
}

void TermFree(Term *term) {
    if (term != NULL) {
        // 只释放Term结构体本身，不释放lhs和rhs表达式
        // 按照"谁new谁free"的原则，表达式由创建者负责释放
        free(term);
    }
}