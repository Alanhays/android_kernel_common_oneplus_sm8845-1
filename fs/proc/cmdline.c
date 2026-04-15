// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/slab.h>
#include "internal.h"

static int cmdline_proc_show(struct seq_file *m, void *v)
{
    char *orig_cmdline = saved_command_line;
    char *new_cmdline;
    char *pos;
    char *end;

    new_cmdline = kstrdup(orig_cmdline, GFP_KERNEL);
    if (!new_cmdline)
        return -ENOMEM;

    pos = strstr(new_cmdline, "androidboot.serialno=");
    if (pos) {
        char *val_start = pos + strlen("androidboot.serialno=");
        
        end = strchr(val_start, ' ');
        
        if (val_start) {
            char *prefix = kstrndup(new_cmdline, val_start - new_cmdline, GFP_KERNEL);
            char *suffix = end ? kstrdup(end, GFP_KERNEL) : kstrdup("", GFP_KERNEL);

            seq_printf(m, "%s3G456U11FH700000%s\n", prefix, suffix);

            kfree(prefix);
            kfree(suffix);
            kfree(new_cmdline);
            return 0;
        }
    }

    seq_printf(m, "%s\n", saved_command_line);
    kfree(new_cmdline);
    return 0;
}

static int __init proc_cmdline_init(void)
{
	struct proc_dir_entry *pde;

	pde = proc_create_single("cmdline", 0, NULL, cmdline_proc_show);
	pde_make_permanent(pde);
	pde->size = saved_command_line_len + 1;
	return 0;
}
fs_initcall(proc_cmdline_init);
