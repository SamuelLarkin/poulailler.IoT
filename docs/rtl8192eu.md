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

you need to block the previous driver rtl8xxxu with this command 
```
echo "blacklist rtl8xxxu" | sudo tee /etc/modprobe.d/rtl8xxxu.conf;
```

And force RTL8192EU Driver to be active from boot:
```
echo -e "8192eu\n\nloop" | sudo tee /etc/modules;
```

Newer versions of Ubuntu has weird plugging/replugging issue (Check #94). This includes weird idling issues, To fix this:
```
echo "options 8192eu rtw_power_mgnt=0 rtw_enusbss=0" | sudo tee /etc/modprobe.d/8192eu.conf;
```

I aslo did this but it said that the build was identical as the `make install` output thus not overridding.
```
sudo dkms add .
$ sudo dkms install rtl8192eu/1.0
```

```
sudo wpa_passphrase  <SSID> <WIFI_PASSWORD> >> /etc/wpa_supplicant/wpa_supplicant.conf
```

In order to have a unique USB WIFI ID, raspbian assigns a string like `ifconfig` that can be found two ways
```
dmesg | grep 'renamed from'
[   19.813091] smsc95xx 1-1.1:1.0 enxb827eb1c965a: renamed from eth0
[   24.772286] rtl8192eu 1-1.2:1.0 enx1cbfce2ca48f: renamed from wlan0
```

```
ifconfig
enx1cbfce2ca48f: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.81.96  netmask 255.255.255.0  broadcast 192.168.81.255
        inet6 fe80::87fc:acd0:9836:2282  prefixlen 64  scopeid 0x20<link>
        ether 1c:bf:ce:2c:a4:8f  txqueuelen 1000  (Ethernet)
        RX packets 22313  bytes 2368891 (2.2 MiB)
        RX errors 0  dropped 7  overruns 0  frame 0
        TX packets 81  bytes 15929 (15.5 KiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

enxb827eb1c965a: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.81.70  netmask 255.255.255.0  broadcast 192.168.81.255
        inet6 fe80::8c6c:fa7e:7fb4:cab5  prefixlen 64  scopeid 0x20<link>
        ether b8:27:eb:1c:96:5a  txqueuelen 1000  (Ethernet)
        RX packets 1036829  bytes 57182990 (54.5 MiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 1847630  bytes 2598033656 (2.4 GiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 159154  bytes 1085828525 (1.0 GiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 159154  bytes 1085828525 (1.0 GiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

```

Replace `<wlan0>` with the proper UUID in `/etc/network/interfaces.d/wlan0`
```
auto <wlan0>
allow-hotplug <wlan0>
iface <wlan0> inet manual
wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf
```

```
sudo ifup enx1cbfce2ca48f
```
