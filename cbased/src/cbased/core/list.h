#pragma once

#include "cbased/core/base.h"
#include "cbased/core/list.h"

typedef struct DLL_Node     DLL_Node;
typedef struct DLL_List     DLL_List;
typedef struct DLL_Iterator DLL_Iterator;

struct DLL_Node {
  DLL_Node *prev, *next;
};

struct DLL_List {
  DLL_Node *head, *tail;
};

struct DLL_Iterator {
  DLL_Node *curr;
  u64       offset;
};

INLINE void
DLL_push_front(DLL_List *list, DLL_Node *node)
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
DLL_push_back(DLL_List *list, DLL_Node *node)
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
DLL_remove(DLL_List *list, DLL_Node *node)
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
DLL_is_empty(DLL_List *list)
{
  return list->head == NULL;
}

INLINE DLL_Node *
DLL_pop_front(DLL_List *list)
{
  DLL_Node *link = list->head;
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

INLINE DLL_Node *
DLL_pop_back(DLL_List *list)
{
  DLL_Node *link = list->tail;
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
DLL_iterate_next(DLL_Iterator *it)
{
  DLL_Node *node = it->curr;
  if (node == NULL) {
    return NULL;
  }
  it->curr = node->next;
  return (void *)((u64)node - it->offset);
}

INLINE void *
DLL_iterate_prev(DLL_Iterator *it)
{
  DLL_Node *node = it->curr;
  if (node == NULL) {
    return NULL;
  }
  it->curr = node->prev;
  return (void *)((u64)node - it->offset);
}

#define _DLL_iterate_next(it, type) (type *)DLL_iterate_next(it)

#define DLL_iterator_head(list, typeid, field_name)                           \
  (DLL_Iterator) { list.head, OFFSET_OF_BY_STRING(typeid, field_name) }

#define DLL_iterator_tail(list, typeid, field_name)                           \
  (DLL_Iterator) { list.tail, OFFSET_OF_BY_STRING(typeid, field_name) }
