//
// Created by yaohuayuan on 2024/11/30.
//

#ifndef DBMS_C_PREDICATE_H
#define DBMS_C_PREDICATE_H
#include "List.h"
#include "Term.h"
typedef struct Term Term;
typedef struct Plan Plan;
typedef struct Predicate{
    List *terms;
}Predicate;
Predicate *PredicateInit(Term* term);
void PredicateConjoinWith(Predicate*predicate,Predicate*predicate1);
bool PredicateIsSatisfied(Predicate*predicate,Scan*scan);
int PredicateReductionFactor(Predicate*predicate,Plan*plan);
Predicate *PredicateSelectSubPred(Predicate*predicate,Schema*schema);
Predicate* PredicateJoinSubPred(Predicate*predicate,Schema *schema1,Schema *schema2);
Constant *PredicateEquatesWithConstant(Predicate*predicate,char *fldnam);
char *PredicateEquatesWithField(Predicate*predicate,char *fldnam);
char *PredicateToString(Predicate*predicate);
#endif //DBMS_C_PREDICATE_H
