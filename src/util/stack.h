#ifndef __STACK_H_
#define __STACK_H_

typedef struct stack_s stack;
stack* stack_create();
void stack_push(stack* stk, void* val);
void* stack_pop(stack* stk);
void stack_destroy(stack* stk);
char stack_is_empty(stack* stk);
stack* stack_reverse(stack* stk);
void stack_foreach(stack* stk, void (*cb)(void*, void*), void* data);
#endif