#include <linux/module.h> 
#include <linux/init.h> 
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/io.h>
#include <linux/of_device.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
#include <linux/moduleparam.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/ioport.h>


#define CLASS_NAME "testled"

static dev_t dev_num;
static struct cdev *my_cdev;
struct class *my_class;
static struct miscdevice imx2_wdt_miscdev;

int init_characterDevice(void)
{
        int retval;
        /* Request the kernel for EEP_NBANK devices */
        retval = alloc_chrdev_region(&dev_num, 0, 1, "linux");
        if (retval) {
                pr_info("Can not register major\n");
                return retval;
        }
        /* Let's create our device's class, visible in /sys/class */
        my_class = class_create(THIS_MODULE, "linux");
        device_create(my_class, NULL, dev_num, NULL, CLASS_NAME);

        /* ALloc memory for device file*/
        my_cdev = cdev_alloc();
        if (my_cdev == NULL) {
                pr_info("Can not alloc memory for device file\n");
        return -1;
        }

        cdev_init(my_cdev, &fops);
        /* Regis device file with kernel*/
        retval = cdev_add(my_cdev, dev_num, 1);
        if (retval < 0) {
                pr_info("Can not add\n");
                return -1;
        }
        return 0;
}

static ssize_t testled_write(struct file *filep, char __user *buf, size_t len, loff_t *offset)
{
	return 0;
}

static const struct file_operations imx2_wdt_fops = {
	.owner = THIS_MODULE,
	//.unlocked_ioctl = imx2_wdt_ioctl,
	//.open = imx2_wdt_open,
	//.release = imx2_wdt_close,
	.write = testled_write,
};

static struct miscdevice imx2_wdt_miscdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "testled",
	.fops = &testled_fops,
};

static int testled_probe(struct platform_device *pdev)
{
	struct resource *iores;	
	void __iomem *base;
	int retval;

	iores = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	printk(KERN_EMERG "__________________________________iores = %d\n",iores->start);
	base = devm_ioremap_resource(&pdev->dev, iores);

	retval = init_characterDevice();
	if (retval) {
		printk(KERN_EMERG "______________________________Can not use character device\n");
	}

	dev_info(&pdev->dev,"__________________________________Test led enable\n");
	return 0;
}

static int testled_remove(struct platform_device *pdev)
{
	cdev_del(my_cdev);
	device_detroy(my_class, dev_num);
	class_detroy(my_class);
	unregister_chrdev(dev_num, CLASS_NAME);
	return 0;
}

static struct of_device_id testled_driver_of_match[] = {
	{ .compatible = "imx,test-led", },
	{ /* sentinel */ }
};

static struct platform_driver testled_driver = {
	.driver = {
		.name = "imx6sx-testled",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(testled_driver_ of_match),
	},
	.probe = testled_probe,
	.remove = testled_remove,
};

static int __init testled_init(void)
{
    return platform_driver_register(&testled_driver);
}

static void __exit testled_exit(void)
{
    platform_driver_unregister(&testled_driver);
}
 
arch_initcall(testled_init);
module_exit(testled_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("HungBX");
MODULE_DESCRIPTION("test_led driver module");