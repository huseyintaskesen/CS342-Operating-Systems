#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/rcupdate.h>
#include <linux/fdtable.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/stat.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <asm/pgtable.h>
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");
/* Module Parameter */
static int pid;
static unsigned long virtaddr = 0;
module_param(pid, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(pid, "Process ID, integer");
module_param(virtaddr, ulong,0);
MODULE_PARM_DESC(virtaddr, "Virtual address, unsigned long");
char* long_to_binary(unsigned long k);

char* long_to_binary(unsigned long k)
{
        static char c[65];
	unsigned long val;
        c[0] = '\0';


       for (val = 1UL << (sizeof(unsigned long)*8-1); val > 0; val >>= 1) 
        {   
            strcat(c, ((k & val) == val) ? "1" : "0");
        }
        return c;
}

int init_module(void)	
{
	printk(KERN_INFO "MODULE LOADED\n");
	struct task_struct *task;
	int pidFound = 0;
	unsigned long v_addr;


	for_each_process(task)
	{
	   if(task->pid == pid){
		printk(KERN_INFO "Name: %s PID: [%d]\n", task->comm, task->pid);
		pidFound = 1;
		break;
	   }
	}
	if(pidFound == 0){
		printk( "No process with given pid\n");
		return 0;	
	}
	printk( KERN_INFO "--Memory Management Information--\n" );

	struct mm_struct* mm = task->mm;
   	
	printk( KERN_INFO "[CODE START]\t\t[CODE END]\t[CODE SIZE]\n");
	printk( KERN_INFO "%lx\t\t%lx\t%lu\n", mm->start_code, 
		mm->end_code, mm->end_code - mm->start_code );
			
	printk( KERN_INFO "[DATA START]\t\t[DATA END]\t[DATA SIZE]\n");
	printk( KERN_INFO "%lx\t\t%lx\t%lu\n", mm->start_data, 
		mm->end_data, mm->end_data - mm->start_data );
	
	printk( KERN_INFO "[ARG START]\t\t[ARG END]\t[ARG SIZE]\n");
	printk( KERN_INFO "%lx\t\t%lx\t%lu\n", mm->arg_start, 
		mm->arg_end, mm->arg_end - mm->arg_start );
		
	printk( KERN_INFO "[ENV START]\t\t[ENV END]\t[ENV SIZE]\n");
	printk( KERN_INFO "%lx\t\t%lx\t%lu\n", mm->env_start, 
		mm->env_end, mm->env_end - mm->env_start );

	printk( KERN_INFO "[HEAP START]\t\t[HEAP END]\t[HEAP SIZE]\n");
	printk( KERN_INFO "%lx\t\t%lx\t%lu\n", mm->start_brk, 
		mm->brk, mm->brk - mm->start_brk );
		
	printk( KERN_INFO "Total VM area = %lu\n", mm->total_vm);
	printk( KERN_INFO "Number of frames = %lu\n", get_mm_rss( mm) );


	struct vm_area_struct *mmap = mm->mmap;
	unsigned long virtual_addr;
   	virtual_addr = mmap -> vm_start;

	int virtaddr_valid = 0;
	if ((virtaddr >= (mmap->vm_start)) && (virtaddr <= (mmap->vm_end)))
		{
			virtaddr_valid = 1;
		}
	/* Checking is entered virtual address in the scope or not and printing */
	if (virtaddr_valid == 1)
	{	
		pr_info("VMU: Entered virtual memory = 0x%lx is in the range\n", virtaddr);
	}
	else
	{
		pr_info("VMU: Entered virtual memory = 0x%lx is not in the range\n", virtaddr);
	}
	
	unsigned long stack_start,stack_end,stack_size;
	printk( KERN_INFO "--Virtual Memory Information--\n" );
	printk( KERN_INFO "[VM START]\t\t[VM_END]\t[VM_SIZE]");
	while( mmap != NULL )
	{
		if( (mmap -> vm_start <= mm->start_stack) &&  (mmap -> vm_end >=mm->start_stack) ) {

			stack_start=mmap ->vm_start;
			stack_end=mmap ->vm_end;
			stack_size=mmap -> vm_end - mmap -> vm_start;
		}
		printk( KERN_INFO "%lx\t\t%lx\t%lu\n", mmap -> vm_start, 
			mmap -> vm_end, mmap -> vm_end - mmap -> vm_start );
		mmap = mmap -> vm_next;
	}
	printk( KERN_INFO "The stack information of the process:\n");
	printk( KERN_INFO "[STACK START]\t[STACK END]\t[STACK SIZE]\n" );
	printk( KERN_INFO "%lx\t\t%lx\t%lu\n", stack_start, 
		stack_end, stack_size );
	
	pgd_t *pgd;
	p4d_t *p4d;
        pud_t *pud;
        pmd_t *pmd;
        pte_t *pte;

	pgd=mm->pgd;
	int i;
	printk( KERN_INFO "[PGD ADDRESSES]" );
	for(i=0; i<512; i++){
		printk("Top Level Page Table Entry: %s\n",long_to_binary(pgd));
		printk("P:  %c",long_to_binary(pgd)[63]);
		printk("R/W:  %c",long_to_binary(pgd)[62]);
		printk("U/S:  %c",long_to_binary(pgd)[61]);
		printk("PWT:  %c",long_to_binary(pgd)[60]);
		printk("PCD:  %c",long_to_binary(pgd)[59]);
		printk("A:  %c",long_to_binary(pgd)[58]);
		pgd=pgd+1;
	}

	p4d = p4d_offset(pgd, virtual_addr);
	pud = pud_offset(p4d, virtual_addr);
	

	v_addr = pud_val(*pud);

	printk( KERN_INFO "[Level 2 ADDRESSES]" );
	for(i=0; i<512; i++){
		printk("Second Level Page Table Entry: %s\n",long_to_binary(v_addr));
		printk("P:  %c",long_to_binary(v_addr)[63]);
		printk("R/W:  %c",long_to_binary(v_addr)[62]);
		printk("U/S:  %c",long_to_binary(v_addr)[61]);
		printk("PWT:  %c",long_to_binary(v_addr)[60]);
		printk("PCD:  %c",long_to_binary(v_addr)[59]);
		printk("A:  %c",long_to_binary(v_addr)[58]);
		v_addr = v_addr+1;
	}

	pmd = pmd_offset(pud, virtual_addr);

	v_addr = pmd_val(*pmd);
	
	printk( KERN_INFO "[Level 3 ADDRESSES]" );
	for(i=0; i<512; i++){
		printk("Third Level Page Table Entry: %s\n",long_to_binary(v_addr));
		printk("P:  %c",long_to_binary(v_addr)[63]);
		printk("R/W:  %c",long_to_binary(v_addr)[62]);
		printk("U/S:  %c",long_to_binary(v_addr)[61]);
		printk("PWT:  %c",long_to_binary(v_addr)[60]);
		printk("PCD:  %c",long_to_binary(v_addr)[59]);
		printk("A:  %c",long_to_binary(v_addr)[58]);
		v_addr=v_addr+1;
	}


	pte = pte_offset_kernel(pmd, virtual_addr);

	v_addr = pte_val(*pte);

	printk( KERN_INFO "[Level 4 ADDRESSES]" );
	for(i=0; i<512; i++){
		printk("Fourth Level Page Table Entry: %s\n",long_to_binary(v_addr));
		printk("P:  %c",long_to_binary(v_addr)[63]);
		printk("R/W:  %c",long_to_binary(v_addr)[62]);
		printk("U/S:  %c",long_to_binary(v_addr)[61]);
		printk("PWT:  %c",long_to_binary(v_addr)[60]);
		printk("PCD:  %c",long_to_binary(v_addr)[59]);
		printk("NANINAMI:  %c",long_to_binary(v_addr)[58]);
		v_addr=v_addr+1;

	}

	/* Printing physical address */
		unsigned long physical_addr = 0;
		unsigned long page_addr = 0;
		unsigned long page_offset = 0;

		page_addr = pte_val(*pte) & PAGE_MASK;
		page_offset = virtual_addr & ~PAGE_MASK;
	        physical_addr = page_addr | page_offset;

	
		printk("VMU: Physical address = 0x%lu\n", physical_addr);
		printk("ananinami");	

	

	printk(KERN_INFO "COMPLETED\n");
 	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "MODULE REMOVED\n");
}



