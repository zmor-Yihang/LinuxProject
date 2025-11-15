#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

static int first_dev_num = 0;
static int second_dev_num = 0;

module_param(first_dev_num, int, S_IRUGO);
module_param(second_dev_num, int, S_IRUGO);

static int __init dev_num_application_init(void)
{
    printk("dev_number_application example!\n");

    dev_t dev_num;
    int ret = 0;

    /* MKDEV宏将主设备号和次设备号合并为一个32位的设备号 */
    dev_num = MKDEV(first_dev_num, second_dev_num);
    printk("dev_num is %d\n", dev_num);

    if (first_dev_num)
    {
        /* 静态注册字符设备号 */
        ret = register_chrdev_region(dev_num, 1, "dev_number_application_example_static");
        if (ret < 0)
        {
            printk("register_chrdev_region failed!\n");
            return ret;
        }
        printk("register_chrdev_region success!\n");
    }
    else
    {
        /* 动态注册字符设备号 */
        ret = alloc_chrdev_region(&dev_num, 0, 1, "dev_number_application_example_dynamic");
        if (ret < 0)
        {
            printk("alloc_chrdev_region failed!\n");
            return ret;
        }
        printk("alloc_chrdev_region success!\n");

        first_dev_num = MAJOR(dev_num);
        second_dev_num = MINOR(dev_num);
        printk("first_dev_num is %d, second_dev_num is %d\n", first_dev_num, second_dev_num);
    }

    return 0;
}

static void __exit dev_num_application_exit(void)
{
    printk("Goodbye dev_number_application example!\n");
}

module_init(dev_num_application_init);
module_exit(dev_num_application_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zmor");
MODULE_DESCRIPTION("A dev_number_application module");
