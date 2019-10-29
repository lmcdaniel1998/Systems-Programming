typedef struct S_statck {
  int number;
  struct S_stack *next;
} stack;

void push(int number, stack **stk_ptr);
int pop(stack **stk_ptr);
