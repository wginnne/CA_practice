#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

/* 创建一个新的队列，分配 Queue 结构体和 data 数组的空间。
 * 如果 malloc 失败返回 NULL，避免后续空指针解引用。
 * 如果 data 分配失败，需要先释放已分配的 queue 再返回。 */
Queue *queue_create(void)
{
  Queue *queue = malloc(sizeof(Queue));
  if (queue == NULL)
  {
    return NULL;
  }
  queue->size = 0;
  queue->capacity = QUEUE_INITIAL_CAPACITY;
  queue->data = malloc(sizeof(double) * queue->capacity);
  if (queue->data == NULL)
  {
    free(queue);
    return NULL;
  }
  return queue;
}

/* 将元素放入队列队尾。队列满时容量翻倍。
 * realloc 失败时直接返回，不覆盖原有 data，避免数据丢失。 */
void push(Queue *queue, double element)
{
  if (queue == NULL)         /* 空指针检查：防止对 NULL 解引用 */
  {
    return;
  }

  if (queue->size == queue->capacity)
  {
    int capacity = queue->capacity * 2;

    /* 使用临时指针接收 realloc 结果，失败时不覆盖原 data */
    double *new_data = realloc(queue->data, sizeof(double) * capacity);
    if (new_data == NULL)    
    {
      return;
    }
    queue->data = new_data;
    queue->capacity = capacity;
  }

  int insert_index = queue->size % queue->capacity;
  queue->data[insert_index] = element;
  queue->size++;
}

/* 返回队尾元素。如果队列为空或指针为 NULL，返回 0。 */
double back(Queue *queue)
{
  if (queue == NULL || queue->size == 0)  /* 空指针 + 空队列检查 */
  {
    return 0;
  }
  return queue->data[queue->size - 1];
}

/* 释放队列占用的内存。指针本身不变（调用者手中仍是悬空指针）。 */
void queue_free(Queue *queue)
{
  if (queue == NULL)         /* 空指针检查：防止重复释放 */
  {
    return;
  }

  free(queue->data);
  free(queue);
}

/* 释放队列内存并将指针置为 NULL，防止悬空指针和二次释放。
 * 使用二级指针，可以修改调用者手中的指针值，比 queue_free 更安全。 */
void another_queue_free(Queue **queue)
{
  if (queue == NULL || *queue == NULL)  /* 双重重检查：既检查指针本身也检查指向内容 */
  {
    return;
  }

  free((*queue)->data);
  free(*queue);
  *queue = NULL;   /* 把调用者的指针设为 NULL，防止悬空指针 */
}

