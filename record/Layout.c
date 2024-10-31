//
// Created by yaohuayuan on 2024/10/31.
//

#include "Layout.h"
int LayoutLengthInBytes(Schema *schema, char*FldName){
    int FldType = SchemaType(schema,FldName);
    if(FldType == FILE_INFO_CODE_INTEGER){
        return sizeof(int);
    }else{
        return PageMaxLength(SchemaLength(schema,FldName));
    }
}
Layout * LayoutInit(Schema*schema,map_int_t* mapInt,int SloSize ){
    Layout *layout = malloc(sizeof(Layout));
    if(mapInt!=NULL){
        layout->schema = schema;
        layout->offsets = mapInt;
        layout->SlotSize = SloSize;
    }else{
        layout->schema = schema;
        layout->offsets = malloc(sizeof(map_int_t));
        map_init(layout->offsets);
        int pos = sizeof(int);
        FieldNode * fieldNode= schema->fields;
        while(fieldNode!=NULL){
            map_set(layout->offsets,fieldNode->fileName,pos);
            pos += LayoutLengthInBytes(schema,fieldNode->fileName);
            int *value = map_get(layout->offsets,fieldNode->fileName);
            printf("%d\n",*value);
            fieldNode=fieldNode->next;
        }

    }

    return layout;
}
int LayoutOffset(Layout*layout ,char*FldName){
    int *val = map_get(layout->offsets,FldName);
    return *val;
}