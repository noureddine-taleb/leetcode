struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode *sorted_list_insert(struct ListNode *head, struct ListNode *el)
{
    struct ListNode *prev;
    struct ListNode *list = head;
   
    if (el->val <= head->val) {
        el->next = head;
        return el;
    }

    while (head && el->val > head->val)
        prev = head, head = head->next;

    el->next = prev->next;
    prev->next = el;
    
    return list;
}

struct ListNode *mergeTwoLists(struct ListNode *l1, struct ListNode *l2){
    struct ListNode *cur;
    if (!l1 && !l2)
        return NULL;

    if (!l1)
        return l2;    
    
    if (!l2)
        return l1;    
    

    for (cur = l2, l2 = l2->next; cur; cur = l2, l2 && (l2 = l2->next)) {
        l1 = sorted_list_insert(l1, cur);
        if (cur == l2)
                break;
    }
    

    return l1;
}


struct ListNode* mergeKLists(struct ListNode** lists, int listsSize){
    struct ListNode *head = NULL;
    for (int i=0; i < listsSize; i++) {
        head = mergeTwoLists(head, lists[i]);
    }
    
    return head;
}