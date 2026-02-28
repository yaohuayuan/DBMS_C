//
// Created by yaohuayuan on 2024/11/30.
//

#include "Predicate.h"
#include "Term.h"
Predicate *PredicateInit(Term* term){
    Predicate *predicate = malloc(sizeof (Predicate));
    if(term==NULL){
        predicate->terms = ListInit(LIST_TYPE_TERM, NULL, NULL, NULL);
    }else{
        predicate->terms = ListInit(LIST_TYPE_TERM, NULL, NULL, NULL);
        ListAppend(predicate->terms, term);
    }
    return predicate;
}
void PredicateConjoinWith(Predicate*predicate,Predicate*predicate1){
    ListAddAll(predicate->terms,predicate1->terms);
}
bool PredicateIsSatisfied(Predicate*predicate,Scan*scan){
    ListNode *p = predicate->terms->head;
    while(p){
        if(!TermIsSatisfied(p->value.termData,scan)){
            return false;
        }
        p=p->next;
    }
    return true;
}
int PredicateReductionFactor(Predicate*predicate,Plan*plan){
    int factor = 1;
    ListNode *p = predicate->terms->head;
    while(p){
        factor*= TermReductionFactor(p->value.termData,plan);
        p=p->next;
    }
    return factor;
}
Predicate *PredicateSelectSubPred(Predicate*predicate,Schema*schema){
    Predicate *result = PredicateInit(NULL);
    ListNode *p = predicate->terms->head;
    while(p){
        if(TermAppliesTo(p->value.termData,schema)){
            ListAppend(result->terms,p->value.termData);
        }
        p=p->next;
    }
    if(result->terms->size==0)
        return NULL;
    else
        return result;
}
Predicate* PredicateJoinSubPred(Predicate*predicate,Schema *schema1,Schema *schema2){
    Predicate *result = PredicateInit(NULL);
    Schema *newSch = SchemaInit();
    SchemaAddAll(newSch,schema1);
    SchemaAddAll(newSch,schema2);
    ListNode *p = predicate->terms->head;
    while(p){
        if(!TermAppliesTo(p->value.termData,schema1)&&
            !TermAppliesTo(p->value.termData,schema2) &&
            TermAppliesTo(p->value.termData,newSch)){
            ListAppend(result->terms,p->value.termData);
        }
        p=p->next;
    }
    if(result->terms->size==0)
        return NULL;
    else
        return result;
}
Constant *PredicateEquatesWithConstant(Predicate*predicate,CString *fldname){
    ListNode *p = predicate->terms->head;
    while(p){
        Constant *c = TermEquatesWithConstant(p->value.termData,fldname);
        if(c!=NULL){
            return c;
        }
        p=p->next;
    }
    return NULL;
}
CString *PredicateEquatesWithField(Predicate*predicate,CString *fldname){
    ListNode *p = predicate->terms->head;
    while(p){
        CString *c = TermEquatesWithField(p->value.termData,fldname);
        if(c!=NULL){
            return c;
        }
        p=p->next;
    }
    return NULL;
}
char *PredicateToString(Predicate*predicate){
    ListNode *p = predicate->terms->head;
    if(p==NULL)
        return strdup("");
    size_t totalLen = 1;
    while (p) {
        char *termStr = TermToString(p->value.termData);
        totalLen += strlen(termStr);
        free(termStr);  // 释放临时字符串
        p = p->next;
    }

    // 分配足够的内存
    char *result = (char *)malloc(totalLen);
    if (!result) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    result[0] = '\0';  // 初始化为空字符串

    // 拼接字符串
    p = predicate->terms->head;
    while (p) {
        char *termStr = TermToString(p->value.termData);
        strcat(result, termStr);
        free(termStr);  // 释放临时字符串
        p = p->next;
    }

    return result;  // 返回拼接后的字符串
}

// 释放Predicate资源
void PredicateFree(Predicate *predicate) {
    if (!predicate) return;
    
    // 释放terms列表
    if (predicate->terms) {
        ListFree(predicate->terms);
    }
    
    free(predicate);
}