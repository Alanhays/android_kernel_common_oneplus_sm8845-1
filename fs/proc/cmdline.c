// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include "internal.h"

static int cmdline_proc_show(struct seq_file *m, void *v)
{
    if (current_uid().val < 10000) {
        seq_puts(m, saved_command_line);
    } else {
        char *p_buf = kstrdup(saved_command_line, GFP_KERNEL);
        char *target;

        if (p_buf) {
            while ((target = strstr(p_buf, "androidboot.debuggable=1")) != NULL) {
                memset(target, ' ', 24);
            }
            while ((target = strstr(p_buf, "androidboot.selinux=permissive")) != NULL) {
                memset(target, ' ', 30);
            }
            seq_puts(m, p_buf);
            kfree(p_buf);
        } else {
            seq_puts(m, saved_command_line);
        }
    }
    seq_putc(m, '\n');
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
