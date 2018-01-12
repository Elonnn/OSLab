// \file path.c

/*
  ------------------------------------
  Create date : 2015-04-21 19:14
  Modified date: 2015-04-21 20:04
  Author : zyy
  Email : 121672309@qq.com
  ------------------------------------
*/

#include<linux/module.h>
#include<linux/fs.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<asm/unistd.h>
#include<asm/uaccess.h>
#include<linux/types.h>
#include<linux/fcntl.h>
#include<linux/dirent.h>
#include<linux/kdev_t.h>


static char *absolute_path;
module_param_named(path,absolute_path,charp,0644);
MODULE_PARM_DESC(absolute_path,"absolute path : type=charp\n");



static int __init path_init(void)
{
    struct inode *inode=NULL;
    struct dentry *dentry=NULL;
    struct file *file=NULL;
    file=filp_open(absolute_path,O_DIRECTORY,0);
    printk("\n\n********************\n\n");
    if(IS_ERR(file)){
        
        file=filp_open(absolute_path,O_RDONLY,0444);
        if(IS_ERR(file))
        {
            printk("The path %s is error!\n",absolute_path);
            return 0;
        }
        
        //struct dentry *dentry = dget(file->f_dentry);
        inode = file_inode(file);
        //inode=file->f_dentry->d_inode;
        printk("This is a file , show my information : \n");
        printk("1.inode number : %ld\n",inode->i_ino);
        printk("2.access right : %o\n",inode->i_mode);
        printk("3.reference count : %d\n",inode->i_count.counter);
        printk("4.hard link count : %u\n",inode->i_nlink);
        printk("5.user Id : %d\n",inode->i_uid);
        printk("6.group Id : %d\n",inode->i_uid);
        printk("7.version : %lu\n",(long unsigned int)inode->i_version);
        printk("8.file size(byte) : %d\n",(int)inode->i_size);
        printk("9.block count : %lu\n",inode->i_blocks);
        printk("10.file state : %lu\n",inode->i_state);
        return 0;
    }   
    else
    {
        dentry=file->f_dentry;
        printk("This is a diretory , show my information : \n");
        printk("1.direntry count : %d\n",dentry->d_count);
        printk("2.dentry flag : %u\n",dentry->d_flags);
        printk("3.hash table number : %u\n",dentry->d_name.hash);
        printk("4.directory name : %s\n",dentry->d_name.name);
        printk("5.name length : %u\n",dentry->d_name.len);
        printk("6.short name : %s\n",dentry->d_iname);
        printk("7.time : %lu\n",dentry->d_time);
        return 0;
    }
}






static void __exit path_exit(void)
{
   printk("\n\n********************\n\n"); 
   printk("Goodbye!\n"); 
   return ;
}


module_init(path_init);
module_exit(path_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("121672309@qq.com");