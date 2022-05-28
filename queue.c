#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return 0;
    INIT_LIST_HEAD(head);
    return head;
}


/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;

    if (list_empty(l)) {
        free(l);
        return;
    }

    element_t *ele, *tmp;

    list_for_each_entry_safe (ele, tmp, l, list) {
        q_release_element(ele);
    }

    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *ele = malloc(sizeof(element_t));

    if (!ele)
        return false;

    ele->value = malloc(strlen(s) + 1);
    memcpy(ele->value, s, strlen(s) + 1);
    list_add(&ele->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *ele = malloc(sizeof(element_t));

    if (!ele)
        return false;

    ele->value = malloc(strlen(s) + 1);
    memcpy(ele->value, s, strlen(s) + 1);
    list_add_tail(&ele->list, head);

    return true;
}


/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    struct list_head *node = head->next;
    list_del(node);

    element_t *ele = list_entry(node, element_t, list);
    strncpy(sp, ele->value, bufsize - 1);


    return ele;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    struct list_head *node = head->prev;
    list_del(node);

    element_t *ele = list_entry(node, element_t, list);
    strncpy(sp, ele->value, bufsize - 1);

    return ele;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;
    int len = 0;
    struct list_head *li;
    list_for_each (li, head)
        len++;

    return len;
}


struct list_head *get_mid(struct list_head *head)
{
    struct list_head *slow, *fast;
    slow = fast = head->next;
    while (fast != head && fast->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;
    struct list_head *node;
    node = get_mid(head);
    list_del(node);

    return true;
}

static inline char *get_value(struct list_head *node)
{
    return list_entry(node, element_t, list)->value;
}


/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    char *s1, *s2;
    s1 = get_value(head->next);
    s2 = get_value(head->next);
    if (strcmp(s1, s2))
        return false;
    return true;
}

static inline void list_replace(struct list_head *old, struct list_head *new)
{
    new->next = old->next;
    new->next->prev = new;
    new->prev = old->prev;
    new->prev->next = new;
}

static inline void list_swap(struct list_head *entry1, struct list_head *entry2)
{
    struct list_head *pos = entry2->prev;
    list_del(entry2);
    list_replace(entry1, entry2);
    if (pos == entry1)
        pos = entry2;
    list_add(entry1, pos);
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (list_is_singular(head))
        return;

    struct list_head *pos;
    for (pos = head->next; pos->next != head && pos != head; pos = pos->next) {
        list_swap(pos, pos->next);
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    struct list_head *pos, *n;

    list_for_each_safe (pos, n, head) {
        pos->next = pos->prev;
        pos->prev = n;
    }
    head->next = pos->prev;
    head->prev = n;
}



/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}
