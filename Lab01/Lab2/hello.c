//实现系统调用函数
sudo gedit kernel/sys.c

#include<linux/linkag.h>
asmlinkage int sys_helloworld(const char __user *str, int size){
	struct file *filep;
	loff_t pos;
	filep = filp_open("/tmp/_hellosys_", O_CREAT | O_RDWR, 0644);
	if(IS_ERR(filep)){
		printk("Cannot open file\n");
		return -1;
	}
	pos = 0;
	printk("writting %s size: %d\n", str, size);
	vfs_write(filep, str, size, &pos);
	filp_close(filep, 0);
}


//增加系统调用号
sudo gedit arch/x86/entry/syscalls/syscall_32.tbl
359		i386	hellosys		sys_hellosys

//声明系统函数
asmlinkage int sys_hellosys(const char __user * str, int size);



//测试函数
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){
	static char str[100] = "hello world, my name is wangzhi";
	syscall(359, str, strlen(str));
	return 0;
}




















menuentry 'Ubuntu，Linux 3.16.0-23-generic' --class ubuntu --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-3.16.0-23-generic-advanced-1a48e952-5abc-49c3-ba27-0ce9746074f7' {
	recordfail
	load_video
	gfxmode $linux_gfx_mode
	insmod gzio
	insmod part_msdos
	insmod ext2
	set root='hd0,msdos1'
	if [ x$feature_platform_search_hint = xy ]; then
	  search --no-floppy --fs-uuid --set=root --hint-bios=hd0,msdos1 --hint-efi=hd0,msdos1 --hint-baremetal=ahci0,msdos1  1a48e952-5abc-49c3-ba27-0ce9746074f7
	else
	  search --no-floppy --fs-uuid --set=root 1a48e952-5abc-49c3-ba27-0ce9746074f7
	fi
	echo	'载入 Linux 3.16.0-23-generic ...'
	linux	/boot/vmlinuz-3.16.0-23-generic root=UUID=1a48e952-5abc-49c3-ba27-0ce9746074f7 ro  quiet splash $vt_handoff
	echo	'载入初始化内存盘...'
	initrd	/boot/initrd.img-3.16.0-23-generic
}

menuentry 'Ubuntu' --class ubuntu --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-simple-1a48e952-5abc-49c3-ba27-0ce9746074f7' {
	recordfail
	load_video
	gfxmode $linux_gfx_mode
	insmod gzio
	insmod part_msdos
	insmod ext2
	set root='hd0,msdos1'
	if [ x$feature_platform_search_hint = xy ]; then
	  search --no-floppy --fs-uuid --set=root --hint-bios=hd0,msdos1 --hint-efi=hd0,msdos1 --hint-baremetal=ahci0,msdos1  1a48e952-5abc-49c3-ba27-0ce9746074f7
	else
	  search --no-floppy --fs-uuid --set=root 1a48e952-5abc-49c3-ba27-0ce9746074f7
	fi
	linux	/boot/vmlinuz-4.2.6 root=UUID=1a48e952-5abc-49c3-ba27-0ce9746074f7 ro  quiet splash $vt_handoff
	initrd	/boot/initrd.img-4.2.6
}
menuentry 'Ubuntu，Linux 4.2.6' --class ubuntu --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-4.2.6-advanced-1a48e952-5abc-49c3-ba27-0ce9746074f7' {
	recordfail
	load_video
	gfxmode $linux_gfx_mode
	insmod gzio
	insmod part_msdos
	insmod ext2
	set root='hd0,msdos1'
	if [ x$feature_platform_search_hint = xy ]; then
	  search --no-floppy --fs-uuid --set=root --hint-bios=hd0,msdos1 --hint-efi=hd0,msdos1 --hint-baremetal=ahci0,msdos1  1a48e952-5abc-49c3-ba27-0ce9746074f7
	else
	  search --no-floppy --fs-uuid --set=root 1a48e952-5abc-49c3-ba27-0ce9746074f7
	fi
	echo	'载入 Linux 4.2.6 ...'
	linux	/boot/vmlinuz-4.2.6 root=UUID=1a48e952-5abc-49c3-ba27-0ce9746074f7 ro  quiet splash $vt_handoff
	echo	'载入初始化内存盘...'
	initrd	/boot/initrd.img-4.2.6
}

menuentry 'Ubuntu，Linux 3.16.0-23-generic' --class ubuntu --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-3.16.0-23-generic-advanced-1a48e952-5abc-49c3-ba27-0ce9746074f7' {
	recordfail
	load_video
	gfxmode $linux_gfx_mode
	insmod gzio
	insmod part_msdos
	insmod ext2
	set root='hd0,msdos1'
	if [ x$feature_platform_search_hint = xy ]; then
	  search --no-floppy --fs-uuid --set=root --hint-bios=hd0,msdos1 --hint-efi=hd0,msdos1 --hint-baremetal=ahci0,msdos1  1a48e952-5abc-49c3-ba27-0ce9746074f7
	else
	  search --no-floppy --fs-uuid --set=root 1a48e952-5abc-49c3-ba27-0ce9746074f7
	fi
	echo	'载入 Linux 3.16.0-23-generic ...'
	linux	/boot/vmlinuz-3.16.0-23-generic root=UUID=1a48e952-5abc-49c3-ba27-0ce9746074f7 ro  quiet splash $vt_handoff
	echo	'载入初始化内存盘...'
	initrd	/boot/initrd.img-3.16.0-23-generic
}


