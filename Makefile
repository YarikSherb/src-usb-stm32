# Hey Emacs, this is a -*- makefile -*-


# Target file name (without extension).
TARGET = main

# List C source files here. (C dependencies are automatically generated.)
# use file-extension c for "c-only"-files
SRC = main.c
SRC += hw_config.c
SRC += usb_desc.c
SRC += usb_endp.c
SRC += usb_istr.c
SRC += usb_prop.c
SRC += usb_pwr.c

SRC += otgd_fs_cal.c
SRC += otgd_fs_dev.c
SRC += otgd_fs_int.c
SRC += otgd_fs_pcd.c
SRC += usb_core.c
SRC += usb_init.c
SRC += usb_int.c
SRC += usb_mem.c
SRC += usb_regs.c
SRC += usb_sil.c

SRC += strip.c
SRC += strip_bb.c
SRC += colors.c
SRC += sys_tick.c

SRC += stm32f10x_dma.c stm32f10x_tim.c

include makefile.mk
