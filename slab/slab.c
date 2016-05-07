#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>

typedef struct slabtest {
    int data;
}slabtest;

void init_slabtest(void *data)
{
    slabtest *p = (slabtest*)data;
    p->data = 999;
}

static int slab_init_module(void)
{
    struct kmem_cache *p_slabtest = kmem_cache_create("slabtest",
                                             sizeof(slabtest),0,
                                             (SLAB_RECLAIM_ACCOUNT|SLAB_PANIC|SLAB_MEM_SPREAD),
                                             init_slabtest);
    slabtest *ret = kmem_cache_alloc(p_slabtest,GFP_KERNEL);
    printk(KERN_)
    return 0;

}

static void slab_cleanup_module(void)
{

}

module_init(slab_init_module);
module_exit(slab_cleanup_module);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("slab test");
