#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include "stack.h"
#include "assert.h"

static int __init test_stack(void){
    int res = 0;
    LIST_HEAD(data_stack);
    stack_entry_t *tos = NULL;
    const char *tos_data = NULL;
    const char* test_data[] = { "1", "2", "3", "4" };
    long i = 0;

    pr_alert("Testing basic stack");

    for (i = 0; i != ARRAY_SIZE(test_data); ++i) {
	tos = create_stack_entry((void*)test_data[i]);
	if (!tos){
		res = -ENOMEM;
		break;
	}
        stack_push(&data_stack, tos);
    }

    for (i = ARRAY_SIZE(test_data) - 1; i >= 0; --i) {
        tos = stack_pop(&data_stack);
        tos_data = STACK_ENTRY_DATA(tos, const char*);
        delete_stack_entry(tos);
        printk(KERN_ALERT "%s == %s\n", tos_data, test_data[i]);
        assert(!strcmp(tos_data, test_data[i]));
    }

    assert(stack_empty(&data_stack));

    return res;
}

static int __init print_processes_backwards(void){
    int res = 0;
    struct task_struct *task;
    stack_entry_t *tos = NULL;
    LIST_HEAD(tasks_stack);
    char * taskComm = NULL;

    for_each_process(task){
	taskComm = kmalloc(sizeof(task->comm), GFP_KERNEL);
	if (!taskComm){
		res = -ENOMEM;
		break;
	}
	taskComm = get_task_comm(taskComm, task);
	tos = create_stack_entry((void*)taskComm);
	if (!tos){
		res = -ENOMEM;
		break;
	}
	stack_push(&tasks_stack, tos);
    }
    while(!stack_empty(&tasks_stack)){
	tos = stack_pop(&tasks_stack);
	taskComm = STACK_ENTRY_DATA(tos, char *);
	printk("%s\n",taskComm);
	kfree(taskComm);
	delete_stack_entry(tos);
    }
    return res;
}

static int __init ll_init(void){
    int res = 0;
    printk(KERN_ALERT "Hello, linked_lists\n");
    res = test_stack();
    if (res){
	return res;
    }
    res = print_processes_backwards();
    return res;
}

static void __exit ll_exit(void){
    printk(KERN_ALERT "Goodbye, linked_lists!\n");
}

module_init(ll_init);
module_exit(ll_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linked list exercise module");
MODULE_AUTHOR("Kernel hacker!");
