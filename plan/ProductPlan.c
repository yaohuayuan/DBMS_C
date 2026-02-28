//
// Created by yaohuayuan on 2024/12/5.
//

#include "ProductPlan.h"
#include "ProductScan.h"
ProductPlan *ProductPlanInit(Plan*plan1,Plan*plan2){
    ProductPlan *productPlan = malloc(sizeof(ProductPlan));
    productPlan->p1 = plan1;
    productPlan->p2 = plan2;
    productPlan->schema = SchemaInit();
    SchemaAddAll(productPlan->schema,productPlan->p1->schema(plan1));
    SchemaAddAll(productPlan->schema,productPlan->p2->schema(plan2));
    return productPlan;
}
Scan* ProductPlanOpen(void *data){
    Plan*plan = (Plan*)data;
    ProductPlan * productPlan = plan->planUnion.productPlan;
    Scan *s1 = productPlan->p1->open(productPlan->p1);
    Scan *s2 = productPlan->p2->open(productPlan->p2);
    Scan *scan = ScanInit(ProductScanInit(s1,s2),SCAN_PRODUCT_CODE);
    return scan;
}
int ProductPlanBlocksAccessed(void *data){
    Plan*plan = (Plan*)data;
    ProductPlan * productPlan = plan->planUnion.productPlan;
    return productPlan->p1->blocksAccessed(productPlan->p1)+productPlan->p1->recordsOutput(productPlan->p1)*productPlan->p2->blocksAccessed(productPlan->p2);
}
int ProductPlanRecordsOutput(void *data){
    Plan*plan = (Plan*)data;
    ProductPlan * productPlan = plan->planUnion.productPlan;
    return productPlan->p1->recordsOutput(productPlan->p1)* productPlan->p2->recordsOutput(productPlan->p2);
}
int ProductPlanDistinctValue(void *data,CString *fldname){
    Plan*plan = (Plan*)data;
    ProductPlan * productPlan = plan->planUnion.productPlan;
    if(SchemaHasField(productPlan->p1->schema(productPlan->p1),fldname)){
        return productPlan->p1->distinctValues(productPlan->p1,fldname);
    }else{
        return productPlan->p2->distinctValues(productPlan->p2,fldname);
    }
}
Schema *ProductPlanSchema(void *data){
    Plan*plan = (Plan*)data;
    ProductPlan * productPlan = plan->planUnion.productPlan;
    return productPlan->schema;
}