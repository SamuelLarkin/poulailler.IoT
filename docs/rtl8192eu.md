# WiFi USB Dongle

* [Drivers for the rtl8192eu chipset for wireless adapters (D-Link DWA-131 rev E1 included!)](https://github.com/Mange/rtl8192eu-linux-driver)
* [Wlan Stick with RTL8192EU Chipset](https://www.raspberrypi.org/forums/viewtopic.php?t=148389&start=25)
* [RTL8192EU driver compilation - PKGBUILD](https://archlinuxarm.org/forum/viewtopic.php?f=59&t=8866#p46867)
* [RTL8192EU Wireless Adapter on Void Linux](https://medium.com/@leandroembu/rtl8192eu-wireless-adapter-on-void-linux-dafc295c6e67)
* [Installing drivers USB WiFi](https://www.raspberrypi.org/forums/viewtopic.php?t=89447)
* [How to load or unload a Linux kernel module](https://opensource.com/article/18/5/how-load-or-unload-linux-kernel-module)
* [Can't install rtl8192eu-linux-driver on UBUNTU 18.10](https://askubuntu.com/questions/1156519/cant-install-rtl8192eu-linux-driver-on-ubuntu-18-10)
* [Tutorial: Compiling a working kernel driver for rtl8192eu](https://discourse.osmc.tv/t/tutorial-compiling-a-working-kernel-driver-for-rtl8192eu/5252)


```
git clone https://github.com/jeremyb31/rtl8192eu-linux-driver
```

```diff
diff --git a/Makefile b/Makefile
index a9d4b93..3938197 100644
--- a/Makefile
+++ b/Makefile
@@ -80,7 +80,8 @@ CONFIG_RTW_SDIO_PM_KEEP_POWER = y
 ###################### MP HW TX MODE FOR VHT #######################
 CONFIG_MP_VHT_HW_TX_MODE = n
 ###################### Platform Related #######################
-CONFIG_PLATFORM_I386_PC = y
+CONFIG_PLATFORM_ARMV6 = y
+CONFIG_PLATFORM_I386_PC = n
 CONFIG_PLATFORM_ANDROID_X86 = n
 CONFIG_PLATFORM_ANDROID_INTEL_X86 = n
 CONFIG_PLATFORM_JB_X86 = n
@@ -916,6 +917,14 @@ EXTRA_CFLAGS += -mfloat-abi=hard
 endif
 endif

+ifeq ($(CONFIG_PLATFORM_ARMV6),y)
+       EXTRA_CFLAGS+=-DCONFIG_LITTLE_ENDIAN
+ARCH:=arm
+KVER  := $(shell uname -r)
+KSRC := /lib/modules/$(KVER)/build
+MODDESTDIR := /lib/modules/$(KVER)/kernel/drivers/net/wireless/
+endif
+
 ifeq ($(CONFIG_APPEND_VENDOR_IE_ENABLE), y)
 EXTRA_CFLAGS += -DCONFIG_APPEND_VENDOR_IE_ENABLE
 endif
```

```
make
sudo make install
sudo insmod 8192eu.ko
modinfo 8192eu.ko 
```
