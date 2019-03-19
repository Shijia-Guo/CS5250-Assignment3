#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/slab.h>
#include<linux/errno.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/proc_fs.h>
#include<linux/sched.h>
#include<asm/uaccess.h>

#define MAJOR_NUMBER 61

int onebyte_open(struct inode *inode,struct file *filep);
int onebyte_release(struct inode *inode, struct file *filep);
ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);
ssize_t onebyte_write(struct file *filep,const char *buf, size_t count, loff_t *f_pos);
static int onebyte_init(void);
static void onebyte_exit(void);

struct file_operations onebyte_fops = {
	read: onebyte_read,
	write: onebyte_write,
	open: onebyte_open,
        release: onebyte_release
	
};

char *onebyte_data = NULL;
int onebyte_open(struct inode *inode, struct file *filep){
	return 0;
}
int onebyte_release(struct inode *inode, struct file *filep){
	return 0;
}
ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos){
	
        if(*f_pos >= 1)
	   return 0;
	raw_copy_to_user(buf,onebyte_data,1);
	*f_pos += 1;
        return 1;
}
ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos){
	
	if(*f_pos == 0){
          *onebyte_data = *buf;
	  *f_pos += 1;
	  if(count > 1){
	    return -ENOSPC;
	  }
	  return 1;
	}

	return -ENOSPC;
	   
	


}
static int onebyte_init(void){
	int result;
	result = register_chrdev(MAJOR_NUMBER,"onebyte", &onebyte_fops);
	if(result < 0){
	   return result;
	}
	onebyte_data = kmalloc(sizeof(char),GFP_KERNEL);
	if(!onebyte_data){
	    onebyte_exit();
	    return -ENOMEM;
	}
	*onebyte_data = 'X';
	printk(KERN_ALERT "This is a onebyte device module\n");
	return 0;

}
static void onebyte_exit(void){
	if(onebyte_data){
	  kfree(onebyte_data);
	  onebyte_data = NULL;
	}
	unregister_chrdev(MAJOR_NUMBER, "onebyte");
	printk(KERN_ALERT "Onebyte device module is unloaded\n");
}
MODULE_LICENSE("GPL");
module_init(onebyte_init);
module_exit(onebyte_exit);

