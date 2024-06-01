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
    struct list_head *new_head = malloc(sizeof(struct list_head));
    if (new_head != NULL) {
        INIT_LIST_HEAD(new_head);
    }
    return new_head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (head == NULL)
        return;
    for (struct list_head *cur = head->next; cur != head;) {
        struct list_head *tmp = cur->next;
        element_t *elem = container_of(cur, element_t, list);
        free(elem->value);
        free(elem);
        cur = tmp;
    }
    free(head);
}

static element_t *q_insert_new_elem(const char *s)
{
    element_t *new_elem = malloc(sizeof(element_t));
    if (new_elem == NULL) {
        return NULL;
    }

    size_t str_len = strlen(s);
    char *new_s = malloc(str_len + 1);
    if (new_s == NULL) {
        free(new_elem);
        return NULL;
    }

    //! safe than strcpy
    memcpy(new_s, s, str_len);
    new_s[str_len] = 0;
    new_elem->value = new_s;
    INIT_LIST_HEAD(&(new_elem->list));

    return new_elem;
}

/* Insert an element at head of qtueue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (head == NULL)
        return NULL;
    element_t *new_elem = q_insert_new_elem(s);

    if (new_elem == NULL) {
        return false;
    }
    list_add(&(new_elem->list), head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (head == NULL)
        return NULL;
    element_t *new_elem = q_insert_new_elem(s);

    if (new_elem == NULL)
        return false;

    list_add_tail(&(new_elem->list), head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || (head->next == head))
        return NULL;

    struct list_head *next = head->next;

    list_del(next);
    element_t *elem = list_entry(next, element_t, list);
    if (sp == NULL)
        return elem;

    int elem_value_size = strlen(elem->value) + 1;
    if (elem_value_size > bufsize) {
        memcpy(sp, elem->value, bufsize - 1);
        sp[bufsize - 1] = 0;
    } else {
        memcpy(sp, elem->value, elem_value_size);
    }
    return elem;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || (head->prev == head))
        return NULL;

    struct list_head *next = head->prev;

    list_del_init(next);
    element_t *elem = list_entry(next, element_t, list);
    if (sp == NULL)
        return elem;

    int elem_value_size = strlen(elem->value) + 1;
    if (elem_value_size > bufsize) {
        memcpy(sp, elem->value, bufsize - 1);
        sp[bufsize - 1] = 0;
    } else {
        memcpy(sp, elem->value, elem_value_size);
    }

    return elem;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    int len = 0;
    if (head == NULL) {
        return 0;
    }

    struct list_head *cur;

    list_for_each (cur, head) {
        len++;
    }

    return len;
}

struct list_head *find_mid(struct list_head *head)
{
    struct list_head *fast = head, *slow = head;
    while (fast->next != head && fast->next->next != head) {
        fast = fast->next->next;
        slow = slow->next;
    }

    return slow->next;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (head == NULL || (head->next == head)) {
        return false;
    }

    struct list_head *mid = find_mid(head);
    list_del_init(mid);
    // totally free elem
    element_t *elem = container_of(mid, element_t, list);
    free(elem->value);
    free(elem);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (head == NULL)
        return false;
    // TODO: adding ordered check
    struct list_head *node;
    for (node = head->next; node != head;) {
        struct list_head *cur = node->next;
        element_t *node_elem = list_entry(node, element_t, list);
        bool is_dup = false;
        while (cur != head) {
            element_t *cur_elem = list_entry(cur, element_t, list);
            if (strcmp(node_elem->value, cur_elem->value)) {
                break;
            }
            cur = cur->next;

            // free elem
            list_del_init(&cur_elem->list);
            free(cur_elem->value);
            free(cur_elem);
            is_dup = true;
        }

        if (is_dup) {
            list_del_init(node);
            free(node_elem->value);
            free(node_elem);
        }
        node = cur;
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (head == NULL)
        return;
    for (struct list_head *cur = head->next; cur != head; cur = cur->next) {
        struct list_head *next = cur->next;
        if (next == head)
            return;

        list_del(cur);
        list_add(cur, next);
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    const struct list_head *right = head->next;
    struct list_head *left = head;
    while (head->prev != right) {
        struct list_head *tail = head->prev;
        list_del(tail);
        list_add(tail, left);
        left = tail;
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (head == NULL || head->next == head || k <= 1)
        return;
    struct list_head *left = head, *right = head->next;
    while (right != head) {
        // check k elements remaining
        int i = 1;
        for (; (i < k) && (right != head); i++)
            right = right->next;
        if (right == head)
            return;
        right = right->next;

        struct list_head *end = left->next;
        while (right->prev != end) {
            struct list_head *tail = right->prev;
            list_del(tail);
            list_add(tail, left);
            left = tail;
        }
        left = end;
        right = left->next;
    }
}

static void __q_merge(struct list_head *left,
                      struct list_head *right,
                      bool descend)
{
    struct list_head *cur_l = left->next, *cur_r = right->next;
    while (cur_l != left && cur_r != right) {
        // find cur_l whose value is larger than cur_r
        while (cur_l != left) {
            const char *elem_l =
                list_entry(descend ? cur_r : cur_l, element_t, list)->value;
            const char *elem_r =
                list_entry(descend ? cur_l : cur_r, element_t, list)->value;
            if (strcmp(elem_l, elem_r) >= 0)
                break;

            cur_l = cur_l->next;
        }
        if (cur_l == left)
            break;
        struct list_head *tmp = cur_r->next;

        list_del(cur_r);
        cur_l->prev->next = cur_r;
        cur_r->prev = cur_l->prev;
        cur_r->next = cur_l;
        cur_l->prev = cur_r;

        cur_r = tmp;
    }
    if (cur_r == right)
        return;
    list_splice_tail_init(right, left);
}


/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (head == NULL || head->next == head || list_is_singular(head))
        return;
    struct list_head *mid = find_mid(head);
    // only one node, return
    struct list_head right_head;
    INIT_LIST_HEAD(&right_head);

    list_cut_position(&right_head, head, mid->prev);

    q_sort(&right_head, descend);
    q_sort(head, descend);
    __q_merge(head, &right_head, descend);
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    int size = q_size(head);
    if (head == NULL)
        return size;
    struct list_head *left = head;
    while (left->next != head) {
        struct list_head *next = left->next;
        const char *next_value = list_entry(next, element_t, list)->value;
        while (left != head) {
            element_t *left_elem = list_entry(left, element_t, list);
            if (strcmp(left_elem->value, next_value) <= 0)
                break;
            size--;
            struct list_head *tmp = left->prev;

            list_del(left);
            free(left_elem->value);
            free(left_elem);

            left = tmp;
        }
        left = next;
    }
    return size;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    int size = q_size(head);
    if (head == NULL)
        return size;
    struct list_head *left = head;
    while (left->next != head) {
        struct list_head *next = left->next;
        const char *next_value = list_entry(next, element_t, list)->value;
        while (left != head) {
            element_t *left_elem = list_entry(left, element_t, list);
            if (strcmp(left_elem->value, next_value) >= 0)
                break;
            size--;
            struct list_head *tmp = left->prev;

            list_del(left);
            free(left_elem->value);
            free(left_elem);

            left = tmp;
        }
        left = next;
    }
    return size;
}

static void _q_merge(queue_contex_t *left_q,
                     queue_contex_t *right_q,
                     bool descend)
{
    left_q->size += right_q->size;
    right_q->size = 0;
    struct list_head *left = left_q->q, *right = right_q->q;

    __q_merge(left, right, descend);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    // head is head of chain
    int queue_size = q_size(head);
    if (queue_size == 0)
        return 0;
    if (queue_size == 1) {
        queue_contex_t *qc = list_entry(head->next, queue_contex_t, chain);
        return qc->size;
    }

    struct list_head *cur_l, *cur_r = head->prev;
    if (queue_size & 1) {
        queue_contex_t *cur_r_q = list_entry(cur_r, queue_contex_t, chain);
        queue_contex_t *cur_r_q_prev =
            list_entry(cur_r->prev, queue_contex_t, chain);
        _q_merge(cur_r_q_prev, cur_r_q, descend);
        cur_r = cur_r->prev;
    }

    while (cur_r->prev != head) {
        cur_l = head->next;
        while (cur_r->next != cur_l) {
            _q_merge(list_entry(cur_l, queue_contex_t, chain),
                     list_entry(cur_r, queue_contex_t, chain), descend);
            cur_l = cur_l->next;
            cur_r = cur_r->prev;
        }
    }
    return list_entry(head->next, queue_contex_t, chain)->size;
}
