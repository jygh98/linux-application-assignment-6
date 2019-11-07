#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>

long long last_saved_time;

struct my_node {
	struct list_head list;
	int data;
};

long long current_timestamp(void) {
    struct timespec ts; 
    getnstimeofday(&ts); // get current time
    long long milliseconds = ts.tv_sec*1000LL + ts.tv_nsec/1000000; // calculate milliseconds
    return milliseconds;
}

void ll_insert_search_delete(void){
	int num_of_elems[3] = {1000, 10000, 100000};
	int i;
	for (i=0; i < 3; i++){
		// initialize linked list
		struct list_head my_list;
		INIT_LIST_HEAD(&my_list);
		
		// add items into linked list
		last_saved_time = current_timestamp();
		int j;
		for (j=0; j < num_of_elems[i]; j++){
			struct my_node *new = kmalloc(sizeof(struct my_node), GFP_KERNEL);
			new->data = j;
			list_add(&new->list, &my_list);
		}
		printk("time in ms to insert %d elements: %lld\n", num_of_elems[i], current_timestamp()-last_saved_time);
		
		// search items in linked list
		last_saved_time = current_timestamp();
		int k;
		for (k=0; k < num_of_elems[i]; k++){
			struct my_node *current_node;
			list_for_each_entry(current_node, &my_list, list){
				if(current_node->data == k){
					break;
				}
			}
		}
		printk("time in ms to search %d elements: %lld\n", num_of_elems[i], current_timestamp()-last_saved_time);

		// delete items from linked list
		last_saved_time = current_timestamp();
		struct my_node *current_node;
		struct my_node *tmp;
		list_for_each_entry_safe(current_node, tmp, &my_list, list){
			list_del(&current_node->list);
			kfree(current_node);
		}
		printk("time in ms to delete %d elements: %lld\n", num_of_elems[i], current_timestamp()-last_saved_time);
	}
}

int __init ll_module_init(void){
	printk("linked list module init\n");
	ll_insert_search_delete();
	return 0;
}


void __exit ll_module_cleanup(void){
	printk("linked list module exit\n");
}
module_init(ll_module_init);
module_exit(ll_module_cleanup);
MODULE_LICENSE("GPL");
