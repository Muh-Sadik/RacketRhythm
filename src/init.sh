#! /bin/sh

#This init file is used to configure the input audio source and output audio source. The file can be edited to change the volume.

pactl set-default-sink bluez_sink.60_AB_D2_57_42_A9.a2dp_sink
pactl set-default-source alsa_input.usb-GeneralPlus_USB_Audio_Device-00.mono-fallback
pactl set-source-volume alsa_input.usb-GeneralPlus_USB_Audio_Device-00.mono-fallback 16000
pactl load-module module-loopback latency_msec=1
