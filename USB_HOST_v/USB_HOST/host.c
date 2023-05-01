
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>

#define MIN(a, b) (((a) <= (b)) ? (a) : (b))
#define BULK_EP_OUT 0x01
#define BULK_EP_IN 0x81
#define MAX_PKT_SIZE 512

static struct usb_device *device;
static struct usb_class_driver class;
static unsigned char bulk_buf[MAX_PKT_SIZE];

static int stm_open(struct inode *i, struct file *f)
{
	printk(KERN_INFO "[%s]: STM Device file is opened.\n", THIS_MODULE->name);
	return 0;
}
static int stm_close(struct inode *i, struct file *f)
{
	printk(KERN_INFO "[%s]: STM Device file is closed.\n", THIS_MODULE->name);
	return 0;
}

static ssize_t stm_read(struct file *f, char __user *buf, size_t cnt, loff_t *off)
{
	int retval;
	int read_cnt = 0, nbytes, bytes_to_read;
	unsigned int pipe;
	printk(KERN_INFO "%s: STM Device read operation invoked.\n", THIS_MODULE->name);

	/* Read the data from the bulk endpoint */
	pipe = usb_rcvbulkpipe(device, BULK_EP_IN);
	printk(KERN_INFO "%s: STM Device pipe = %u.\n", THIS_MODULE->name, pipe);
	retval = usb_bulk_msg(device, pipe, bulk_buf, MAX_PKT_SIZE, &read_cnt, 5000);
	if (retval)
	{
		printk(KERN_ERR "%s: Bulk message returned %d\n", THIS_MODULE->name, retval);
		return retval;
	}
	bytes_to_read = MIN(cnt, read_cnt);
	nbytes = bytes_to_read - copy_to_user(buf, bulk_buf, bytes_to_read);
	return nbytes;
}

static ssize_t stm_write(struct file *f, const char __user *buf, size_t cnt, loff_t *off)
{
	int retval;
	int bytes_to_write, nbytes;
	unsigned int pipe;
	printk(KERN_INFO "[%s]: STM Device write operation invoked.\n", THIS_MODULE->name);

	bytes_to_write = MIN(cnt, MAX_PKT_SIZE);
	nbytes = bytes_to_write - copy_from_user(bulk_buf, buf, bytes_to_write);
	if (nbytes == 0)
	{
		printk(KERN_INFO "%s: No data to write.\n", THIS_MODULE->name);
		return -EFAULT;
	}

	/* Write the data into the bulk endpoint */
	pipe = usb_sndbulkpipe(device, BULK_EP_OUT);
	retval = usb_bulk_msg(device, pipe, bulk_buf, nbytes, &nbytes, 5000);
	if (retval)
	{
		printk(KERN_ERR "%s: Bulk message returned %d\n", THIS_MODULE->name, retval);
		return retval;
	}
	return nbytes;
}

static struct file_operations fops =
	{
		.owner = THIS_MODULE,
		.open = stm_open,
		.release = stm_close,
		.read = stm_read,
		.write = stm_write,
};

// device arival detection method
static int usb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	int retval;
	printk(KERN_INFO "[%s]: STM usb device(%04x:%04x) is plugged in \n", THIS_MODULE->name, id->idVendor, id->idProduct);
	device = interface_to_usbdev(interface);
	// get info about usb device using "device" pointer

	class.name = "usb/stm%d";
	class.fops = &fops;
	if ((retval = usb_register_dev(interface, &class)) < 0)
		printk(KERN_ERR "[%s]: Not able to get a minor for this device.\n", THIS_MODULE->name);
	else
		printk(KERN_INFO "[%s]: Minor obtained: %d\n", THIS_MODULE->name, interface->minor);
	return retval;
}

// device removal detection method
static void usb_disconnect(struct usb_interface *interface)
{
	printk(KERN_INFO "[%s]: STM usb device is unplugged\n", THIS_MODULE->name);
}

static struct usb_device_id usb_table[] = {
	{USB_DEVICE(0x07CE, 0x4E14)},
	{} // terminating entry
};

MODULE_DEVICE_TABLE(usb, usb_table);

static struct usb_driver usb_driver = {
	.name = "usb_driver",
	.id_table = usb_table,		
	.probe = usb_probe,			// probe() will get called, when device descriptor match with data present in usb_table
	.disconnect = usb_disconnect, // when stm/client device get remove from the host, disconnect() will get called
};

static int __init stm_init(void)	// custom host driver get loaded into the kernel during insmod
{
	int iRetVal;
	iRetVal = usb_register(&usb_driver);
	if (iRetVal)
		printk(KERN_ERR "[%s]: usb_register failed. Error number %d\n", THIS_MODULE->name, iRetVal);
	else
		printk(KERN_INFO "[%s]: USB device driver module initialized ...\n", THIS_MODULE->name);
	return (iRetVal);
}

static void __exit stm_exit(void)	// custom host driver get unloaded from the kernel during rmmod
{
	usb_deregister(&usb_driver);
	printk(KERN_INFO "[%s]: Existing from usb device driver module\n", THIS_MODULE->name);
}

module_init(stm_init);
module_exit(stm_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("USB DEVICE DRIVER ");
MODULE_AUTHOR("NICK<nickkadu10@gmail.com>");
