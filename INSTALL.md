```
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
```

```
git submodule update --init --recursive
```

It helps to have an installed [`picotool`](https://github.com/raspberrypi/picotool).
If not, it will probably be installed by the SDK.

Or, just use the following commands:
```
mkdir build
cd build
cmake ..
make
cp *.uf2 /media/$(user)/RPI-RP2
```


Also, to enable the other eight channels (to appear as second device):

```
# HID_QUIRK_MULTI_INPUT is 0x40
echo "options usbhid quirks=0x5050:0x4004:0x40" | sudo tee /etc/modprobe.d/sbus_hid_quirk.conf

$ sudo update-initramfs -u
# Requires a reboot to take effect, e.g.:
$ sudo shutdown -r now
```