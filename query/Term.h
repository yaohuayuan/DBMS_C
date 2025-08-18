//
// Created by yaohuayuan on 2024/11/28.
//

#ifndef DBMS_C_TERM_H
#define DBMS_C_TERM_H
#include "Schema.h"
#include "Scan.h"
#include "Expression.h"
typedef struct Expression Expression;
typedef struct Plan Plan;
typedef struct Term{
    Expression *lhs,*rhs;
}Term;
Term *TermInit(Expression*lhs,Expression*rhs);
bool TermIsSatisfied(Term *term,Scan *s);
int TermReductionFactor(Term*term,Plan*plan);
Constant *TermEquatesWithConstant(Term*term,char *fldname);
char *TermEquatesWithField(Term*term,char *fldname);
bool TermAppliesTo(Term*term,Schema*schema);
char *TermToString(Term*term);
#endif //DBMS_C_TERM_H
