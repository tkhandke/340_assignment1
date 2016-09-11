#include <linux/module.h>   //needed for all kernel modules
#include <linux/init.h>     //used for _init and _exit macros
#include <linux/kernel.h>   //used for Kern-Info
#include <linux/proc_fs.h>  //used for writing to proc files
#include <linux/seq_file.h> //used for using to seq files
#include <linux/mm.h>       //used for vm stats.h


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tanay");
MODULE_DESCRIPTION("Number of pagefaults");


//a place to store info about vm stats events
static unsigned long counter_buffer[NR_VM_EVENT_ITEMS]; 

//print total pagefaults
static int show_num_pagefaults(struct seq_file *m,void *v)
{
	all_vm_events(counter_buffer);
	seq_printf(m,"Number of pagefaults=\t");
	seq_printf(m,"%lu\n",counter_buffer[PGFAULT]);
	return 0;
}

//when opening proc files
static int open_num_pagefaults(struct inode *inode,struct file *file)
{
	return single_open(file,show_num_pagefaults,NULL);

}


//function for proc file operations
static const struct file_operations numpagefaults_fops = 
{
	.owner      =  THIS_MODULE,
	.open	    =  open_num_pagefaults,
	.read	    =  seq_read,
	.release    =  single_release,

};

//when kernel module is initialized
static int __init init_num_pagefault(void)	
{
	//creates proc file entry
	proc_create("num_pagefaults",0,NULL,&numpagefaults_fops);
	return 0;
}

//when kernel module is removed
static void __exit exit_num_pagefault(void)
{
	remove_proc_entry("num_pagefaults", NULL); //removes proc file entry
	printk(KERN_INFO "Cleaned up module.\n");

}


module_init(init_num_pagefault);

module_exit(exit_num_pagefault);



MODULE_LICENSE("GPL");







