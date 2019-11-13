#include <linux/module.h>
#include <linux/mm.h>
#include <linux/slab.h>

static unsigned char *ptr;
static unsigned int pa;

int init_module(void)
{
	/* size of struct page  */
	pr_info("Size of struct page = %d\n",sizeof(struct page));
	ptr = kmalloc(4096, GFP_KERNEL);
	pr_info("Value of ptr (Linear Address) = %p\n",ptr);

	/* print no of nodes in page list */
	pr_info("\nNumber of physical pages %ld", num_physpages);

	/* get physical address mapped from page frame table */
	pa = __pa(ptr);
	pr_info("Value of pa (Physical Address) = %08x\n",pa);
	strcpy(ptr,"data written to page\n");
	
	return 0;
}

void cleanup_module(void)
{
	kfree(ptr);
	pr_info("Unloading\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("techveda.org");
