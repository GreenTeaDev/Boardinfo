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

static u_char    mac[6];
static long int  wps;
static u_char    ddr1[16];
static u_char    ddr2[16];

static int boardinfo_show(struct seq_file *m, void *v)
{
	u_int8_t i;

	seq_printf(m, "mac address : %pM\n", mac);
	seq_printf(m, "wps pin     : %ld\n", wps);

	seq_printf(m, "ddr settings: ");
	for (i = 0; i < 16; i++)
	{
		if (i > 0) seq_printf(m, " ");
		seq_printf(m, "%02X", ddr1[i]);
	}
	seq_printf(m, "\n");

	seq_printf(m, "ddr settings: ");
        for (i = 0; i < 16; i++)
        {
                if (i > 0) seq_printf(m, " ");
                seq_printf(m, "%02X", ddr2[i]);
        }
        seq_printf(m, "\n");

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

	printk(KERN_INFO "Loading Boardinfo module\n");
	jif_file = proc_create(BOARDINFO_PROC_NAME, 0, NULL, &boardinfo_fops);

	if (!jif_file) {
		return -ENOMEM;
	}

	mtd_info = get_mtd_device_nm(BOARDCONFIG_MTD_NAME);

	if(!IS_ERR(mtd_info)) {
		size_t retlen;
		u_char wps_raw[4];
		u_char wps_hex[9] = { '\0' };

		mtd_read(mtd_info,  0xf100, 6, &retlen, mac);

		mtd_read(mtd_info,  0xf200, 4, &retlen, wps_raw);
		bin2hex(wps_hex, wps_raw, 4);
		kstrtol(wps_hex, 16, &wps);

		mtd_read(mtd_info, 0x10000, 16, &retlen, ddr1);
		mtd_read(mtd_info, 0x10010, 16, &retlen, ddr2);
	}

	mtd_info = NULL;

	return 0;
}

static void __exit boardinfo_exit(void)
{
	remove_proc_entry(BOARDINFO_PROC_NAME, NULL);
	printk(KERN_INFO "Unload Boardinfo module\n");
}

module_init(boardinfo_init);
module_exit(boardinfo_exit);

MODULE_LICENSE("GPL"); 
