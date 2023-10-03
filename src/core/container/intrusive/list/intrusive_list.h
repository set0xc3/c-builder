#pragma once

#include "core/base.h"
#include "core/intrinsics.h"

typedef struct IL_Node     IL_Node;
typedef struct IL_List     IL_List;
typedef struct IL_Iterator IL_Iterator;

struct IL_Node {
  IL_Node *prev, *next;
};

struct IL_List {
  IL_Node *head, *tail;
};

struct IL_Iterator {
  IL_Node *curr;
  u64      offset;
};

INLINE void
IL_push_front(IL_List *list, IL_Node *node)
{
  if (list->head != NULL) {
    list->head->prev = node;
    node->prev       = NULL;
    node->next       = NULL;
    list->head       = node;
  } else {
    list->head = node;
    list->tail = node;
    node->prev = NULL;
    node->next = NULL;
  }
}

INLINE void
IL_push_back(IL_List *list, IL_Node *node)
{
  if (list->tail != NULL) {
    list->tail->next = node;
    node->prev       = list->tail;
    node->next       = NULL;
    list->tail       = node;
  } else {
    list->head = node;
    list->tail = node;
    node->prev = NULL;
    node->next = NULL;
  }
}

INLINE void
IL_remove(IL_List *list, IL_Node *node)
{
  if (node != NULL) {
    node->next->prev = node->prev;
  }
  if (node->prev != NULL) {
    node->prev->next = node->next;
  }
  if (list->head == node) {
    list->head = node->next;
  }
  if (list->tail == node) {
    list->tail = node->prev;
  }
}

INLINE b32
IL_is_empty(IL_List *list)
{
  return list->head == NULL;
}

INLINE IL_Node *
IL_pop_front(IL_List *list)
{
  IL_Node *link = list->head;
  if (link == NULL) {
    return NULL;
  }
  if (link->next != NULL) {
    link->next->prev = link->prev;
  }
  if (link->prev != NULL) {
    link->prev->next = link->next;
  }
  if (link == list->head) {
    list->head = link->next;
  }
  if (link == list->tail) {
    list->tail = link->prev;
  }
  return link;
}

INLINE IL_Node *
IL_pop_back(IL_List *list)
{
  IL_Node *link = list->tail;
  if (link == NULL) {
    return NULL;
  }
  if (link->next != NULL) {
    link->next->prev = link->prev;
  }
  if (link->prev != NULL) {
    link->prev->next = link->next;
  }
  if (link == list->head) {
    list->head = link->next;
  }
  if (link == list->tail) {
    list->tail = link->prev;
  }
  return link;
}

INLINE void *
IL_iterate_next(IL_Iterator *it)
{
  IL_Node *node = it->curr;
  if (node == NULL) {
    return NULL;
  }
  it->curr = node->next;
  return (void *)((u64)node - it->offset);
}

INLINE void *
IL_iterate_prev(IL_Iterator *it)
{
  IL_Node *node = it->curr;
  if (node == NULL) {
    return NULL;
  }
  it->curr = node->prev;
  return (void *)((u64)node - it->offset);
}

#define _IL_iterate_next(it, type) (type *)IL_iterate_next(it)

#define IL_iterator_head(list, typeid, field_name)                            \
  (IL_Iterator) { list.head, offset_of_by_string(typeid, field_name) }

#define IL_iterator_tail(list, typeid, field_name)                            \
  (IL_Iterator) { list.tail, offset_of_by_string(typeid, field_name) }
