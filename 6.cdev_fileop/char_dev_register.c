#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>

dev_t dev_num;
int first_num = 0;
int second_num = 0;

struct class *cdev_class = NULL;
struct device *dev = NULL;

/**
 * @brief 字符设备测试读取函数
 *
 * @param filp 文件指针
 * @param buf 用户空间缓冲区指针
 * @param count 要读取的字节数
 * @param f_pos 文件当前位置指针
 * @return ssize_t 实际读取的字节数
 */
static ssize_t cdev1_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    printk("cdev1_read\n");
    return 0;
}

/**
 * @brief 字符设备测试写入函数
 *
 * @param filp 文件指针
 * @param buf 用户空间缓冲区指针
 * @param count 要写入的字节数
 * @param f_pos 文件当前位置指针
 * @return ssize_t 实际写入的字节数
 */
static ssize_t cdev1_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    printk("cdev1_write\n");
    return 0;
}

/**
 * @brief 字符设备开启函数
 * @param inode inode指针
 * @param filp 文件指针
 * @return int 0表示成功
 */
static int cdev1_open(struct inode *inode, struct file *filp)
{
    printk("cdev1_open\n");
    return 0;
}

/**
 * @brief 字符设备测试关闭函数
 *
 * @param inode inode指针
 * @param filp 文件指针
 * @return int 0表示成功
 */
static int cdev1_release(struct inode *inode, struct file *filp)
{
    printk("cdev1_release\n");
    return 0;
}

/**
 * @brief 字符设备结构体(句柄)
 *
 * @param owner 模块指针
 */
struct cdev cdev1_t = {
    .owner = THIS_MODULE,
};

/**
 * @brief 字符设备文件操作结构体
 *
 * @param owner 模块指针
 * @param read 读取函数指针
 * @param write 写入函数指针
 * @param open 开启函数指针
 * @param release 关闭函数指针
 */
struct file_operations cdev1_fp = {
    .owner = THIS_MODULE,
    .read = cdev1_read,
    .write = cdev1_write,
    .open = cdev1_open,
    .release = cdev1_release,
};
static int __init char_dev_register_init(void)
{
    /* 动态注册字符设备号 */
    alloc_chrdev_region(&dev_num, 0, 1, "cdev1");

    // first_num = MAJOR(dev_num);
    // second_num = MINOR(dev_num);
    // printk("dev_num is %d, first_num is %d, second_num is %d\n", dev_num, first_num, second_num);

    /* 初始化cdev1 */
    cdev_init(&cdev1_t, &cdev1_fp);

    /* 添加字符设备到内核 */
    cdev_add(&cdev1_t, dev_num, 1);

    /* 创建类 */
    cdev_class = class_create("cdev_class");

    /* 创建设备节点 */
    device_create(cdev_class, NULL, dev_num, NULL, "cdev1");

    return 0;
}

static void __exit char_dev_register_exit(void)
{
    /* 删除设备节点 */
    device_destroy(cdev_class, dev_num);
    
    /* 删除类 */
    class_destroy(cdev_class);
    
    /* 删除字符设备 */
    cdev_del(&cdev1_t);

    /* 删除设备号 */
    unregister_chrdev_region(dev_num, 1);

    printk("Goodbye cdev1 example!\n");
}

module_init(char_dev_register_init);
module_exit(char_dev_register_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zmor");
MODULE_DESCRIPTION("A cdev module");
