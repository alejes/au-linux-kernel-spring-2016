#include "stack.h"
#include "linux/slab.h"
#include "linux/gfp.h"

stack_entry_t* create_stack_entry(void *data){
	stack_entry_t * ptr = kmalloc(sizeof(stack_entry_t), GFP_KERNEL);
	if (!ptr){
		printk(KERN_ALERT "Cannot allocate memory\n");
		return NULL;
	}
	
	ptr->data = data;
	INIT_LIST_HEAD(&(ptr->lh));
	
	return ptr;
}

void delete_stack_entry(stack_entry_t *entry){
	kfree(entry);
}
	
void stack_push(struct list_head *stack, stack_entry_t *entry){
	list_add(&(entry->lh), stack);
}

stack_entry_t* stack_pop(struct list_head *stack){
	struct list_head * prev = stack->next;
	stack_entry_t * ptr = list_entry(prev, stack_entry_t, lh);
	list_del(prev);
	return ptr;
}
