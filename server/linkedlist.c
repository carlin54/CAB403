//
// Created by carlin on 10/24/16.
//


#include "global.h"

bool is_list_empty(struct LinkedList* pList){
    return pList->pHead == NULL;
}
void list_add_tail(struct LinkedList* pList, void* pData){
    assert(pList != NULL);
    assert(pData != NULL);
    struct Link* pLink = (struct Link*)malloc(sizeof(struct Link));
    pLink->pNext = NULL;
    pLink->pData = pData;
    if(pList->pHead == NULL) {
        assert(pList->pTail == NULL);
        pList->pHead = pLink;
        pList->pTail = pLink;
    } else {
        pList->pTail->pNext = pLink;
        pList->pTail = pLink;
    }
    pList->length++;
}
void list_remove_head(struct LinkedList* pList){
    assert(pList != NULL);
    if(pList->pHead != NULL){
        struct Link* old_head = pList->pHead;
        pList->pHead = pList->pHead->pNext;
        if(pList->pHead == NULL) pList->pTail = NULL;
        free(old_head->pData);
        free(old_head);
        pList->length--;
    }else{
        assert(pList->pTail == NULL);
    }

}