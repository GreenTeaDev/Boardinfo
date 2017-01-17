#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

#include <linux/fs.h>		// for basic filesystem
#include <linux/proc_fs.h>	// for the proc filesystem
#include <linux/seq_file.h>	// for sequence files

#define BOARDINFO_PROC_NAME "boardinfo"

static struct proc_dir_entry* jif_file;

static int boardinfo_show(struct seq_file *m, void *v)
{
	seq_printf(m, "Just a dummy string\n");
	return 0;
}

static int boardinfo_open(struct inode *inode, struct file *file)
{
	return single_open(file, boardinfo_show, NULL);
}

static const struct file_operations boardinfo_fops = {
	.owner	= THIS_MODULE,
	.open	= boardinfo_open,
	.read	= seq_read,
	.llseek	= seq_lseek,
	.release= single_release,
};

static int __init boardinfo_init(void)
{
	printk(KERN_INFO "Loading my demo module\n");
	jif_file = proc_create(BOARDINFO_PROC_NAME, 0, NULL, &boardinfo_fops);

	if (!jif_file) {
		return -ENOMEM;
	}

	return 0;
}

static void __exit boardinfo_exit(void)
{
	remove_proc_entry(BOARDINFO_PROC_NAME, NULL);
	printk(KERN_INFO "Unload my demo module\n");
}

module_init(boardinfo_init);
module_exit(boardinfo_exit);

MODULE_LICENSE("GPL"); 
