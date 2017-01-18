#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

#include <linux/fs.h>		// for basic filesystem
#include <linux/proc_fs.h>	// for the proc filesystem
#include <linux/seq_file.h>	// for sequence files

#include <linux/mtd/mtd.h>
#include <linux/err.h>

#define BOARDINFO_PROC_NAME "boardinfo"
#define BOARDCONFIG_MTD_NAME "boardconfig"

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
	struct mtd_info* mtd_info;
	size_t retlen;

	printk(KERN_INFO "Loading my demo module\n");
	jif_file = proc_create(BOARDINFO_PROC_NAME, 0, NULL, &boardinfo_fops);

	if (!jif_file) {
		return -ENOMEM;
	}

	mtd_info = get_mtd_device_nm(BOARDCONFIG_MTD_NAME);

	if(!IS_ERR(mtd_info)) {
		u_char buf[64];
		u_char strg[64];

		/* read something from last sector */
		mtd_read(mtd_info, 0xf100, 6, &retlen, buf);
		bin2hex(strg, buf, 6);
		printk("Mac: %s\n", strg);
	}

	mtd_info = NULL;

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
