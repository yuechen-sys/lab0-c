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

static element_t *q_insert_new_elem(const struct list_head *head, const char *s)
{
    if (head == NULL)
        return NULL;

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

    // safe
    memcpy(new_s, s, str_len + 1);
    new_elem->value = new_s;
    INIT_LIST_HEAD(&(new_elem->list));

    return new_elem;
}

/* Insert an element at head of qtueue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *new_elem = q_insert_new_elem(head, s);
    if (new_elem == NULL) {
        return false;
    }
    list_add(&(new_elem->list), head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *new_elem = q_insert_new_elem(head, s);

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

    struct list_head *elem_p = head->next;

    list_del_init(elem_p);
    element_t *elem = container_of(elem_p, element_t, list);
    return elem;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || (head->prev == head))
        return NULL;

    struct list_head *elem_p = head->prev;

    list_del_init(elem_p);
    element_t *elem = container_of(elem_p, element_t, list);
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

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
