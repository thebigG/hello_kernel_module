#include <linux/module.h>
#include <linux/kernel.h>
//#include <linux/init.h>
#include <linux/fs.h>
//#include <asm/uaccess.h>
#include <linux/uaccess.h>
//#include <linux/sched.h>
//#include <linux/fs_struct.h>


/**
Example in https://tldp.org/LDP/lkmpg/2.6/html/lkmpg.html#AEN569
*/

volatile static int is_open = 0;

static char message[1024];
int num_bytes = 0;
static int Major;

ssize_t hello_read(struct file *filep, char __user *outb, size_t nbytes, loff_t * offset)
{
	int bytes_read = 0;
	
	if (offset == NULL)
	{
		return -EINVAL;
	}
	if (*offset >= num_bytes)
	{
		return 0;
	}
	while((bytes_read < nbytes) && ( *offset < num_bytes))
	{
		put_user(message[*offset], &(outb[bytes_read]));
		
		bytes_read++;
		(*offset)++;
	}

	return bytes_read;
}

ssize_t hello_write (struct file *filp, const char __user * inpb, size_t nbytes, loff_t * offset)
{
	int bytes_written = 0;

	if(offset == NULL)
	{
		return -EINVAL;
	}

	if(*offset >= 1023)
	{
		return -EINVAL;
	}

	while((bytes_written < nbytes) && (*offset < 1023) )
	{
		get_user(message[*offset], &inpb[bytes_written]);
		*offset = *offset + 1;
		bytes_written++;
	}
	
	num_bytes = strlen(inpb);

	printk(KERN_INFO "%s was written to device.\n", message);
	return bytes_written;
}

int hello_open (struct inode * inodep, struct file * filep)
{
	if(is_open ==  1)
	{
		printk(KERN_INFO "Error: hello device already open.\n");
		return -EBUSY;
	}

	is_open = 1;

	return 0;
}

int hello_release (struct inode * inodep, struct file *filep)
{
	if(is_open == 0)
	{
		printk(KERN_INFO "Error: device was not opened.\n");
		return -EBUSY;
	}
	is_open = 0;

	return 0;
}

static struct file_operations fops = {
	read: hello_read,
	write: hello_write,
	open: hello_open,
	release: hello_release
};

static int __init hello_start(void)
{
	printk(KERN_INFO "Hello, I'm here to help\n");
	
	strncpy(message, "Hello World", 1023);

	num_bytes = strlen(message);

	Major = register_chrdev(0, "hello", &fops);
	printk(KERN_INFO "The hello device is major:%d\n", Major);   

	return 0;
}

static void __exit hello_end(void)
{
  printk(KERN_INFO "Goodbye, I hope I was helpful\n");
  unregister_chrdev(Major, "hello");
}

module_init(hello_start);
module_exit(hello_end);
