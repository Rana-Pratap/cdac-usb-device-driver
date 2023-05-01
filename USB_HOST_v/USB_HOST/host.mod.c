#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x8152c6d1, "module_layout" },
	{ 0xbe1e8fb9, "usb_deregister" },
	{ 0xa7d7ef35, "usb_register_driver" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x56470118, "__warn_printk" },
	{ 0xc959d152, "__stack_chk_fail" },
	{ 0x295e92c8, "usb_bulk_msg" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xd0aa33d, "usb_register_dev" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("usb:v07CEp4E14d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "2C3CF232DF368CAE2998A31");
