#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/hash.h>

struct simple_hash
{
    int data;
    struct hlist_node node;
};

struct hlist_head * phash = NULL;

static int __init initialization(void)
{
    int i,k;
    struct hlist_head * phead;
    struct hlist_node * pnode;
    struct simple_hash * p;

    printk(KERN_INFO " init simple start\n");

    phash = (struct hlist_head*)kmalloc(sizeof(struct hlist_head) * 0xFF, GFP_KERNEL);
    for (i = 0; i < 0xFF; i++) {
        INIT_HLIST_HEAD(&phash[i]);
    }
    for (i = 0; i < 10; i++) {
        p = (struct simple_hash*)kmalloc(sizeof(struct simple_hash), GFP_KERNEL);
        k = i * 13;

        p->data = k;
        INIT_HLIST_NODE(&p->node);

        printk(KERN_INFO "insert %d\n", k);
        phead = &phash[hash_32(k, 8)];
        hlist_add_head(&p->node, phead);
    }
    k = 3 * 13;
    phead = &phash[hash_32(k, 8)];
    printk(KERN_INFO "search %d\n", k);
    hlist_for_each_entry(p, pnode, phead, node) {
        if (p->data == k) {
            printk(KERN_INFO " find it\n");
        }
    }

    printk(KERN_INFO "init simple end\n");
    return 0;
}

static void __exit cleanup(void)
{
    int i;
    struct hlist_head * phead = NULL;
    struct simple_hash * p = NULL;
    printk(KERN_INFO "cleanup simple\n");

    if (phash == NULL) {
        return;
    }

    for (i = 0; i < 0xFF; i++) {
        phead = &phash[i];
        while (!hlist_empty(phead)) {
            p = hlist_entry(phead->first, struct simple_hash, node);
            printk(KERN_INFO "delete %d", p->data);
            hlist_del(&p->node);
            kfree(p);
        }
    }
    kfree(phead);
}

module_init(initialization);
module_exit(cleanup);

MODULE_AUTHOR("cppbreak cppbreak@gmail.com");
MODULE_DESCRIPTION("A simple linux kernel module");
MODULE_VERSION("V0.1");
MODULE_LICENSE("Dual BSD/GPL");
