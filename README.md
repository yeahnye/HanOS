# HanOS - Microkernel-based General Purpose Operating System

[English](https://github.com/jjwang/HanOS/blob/mainline/README.md) | [中文](https://github.com/jjwang/HanOS/blob/mainline/README.zh-cn.md)

![](https://tokei.rs/b1/github/jjwang/HanOS?category=code)

## Design as below

- Bootloader: Limine is used to get the kernel running as quickly possible. Limine boot protocol is chosed by HanOS.
- CPU mode: x86-64 Long Mode is supported in HanOS. HanOS does not has plan to support other x86 modes. 
- GUI: GUI will not be the 1st priority for HanOS. But HanOS will port some GUI libraries in the future.

## Progress update
- [x] Framebuffer based terminal and kernel log system
- [x] Initialize GDT and IDT to handle exceptions
- [x] Physical memory allocator and virtual memory manager
- [x] Parse ACPI tables and initialize MADT
- [x] Start up all CPUs
- [x] Set up APIC(Advanced Programmable Interrupt Controller) interrupt controller
- [x] Read RTC time from CMOS and configure HPET timer
- [x] Scheduling driven by APIC timer
- [x] Keyboard/mouse driver and command line interface
- [x] VFS, FAT32 and RAMFS file system. RAMFS is for loading and executing program from ELF file
- [x] Tasks of kernel and user space
- [x] Background image display for command line interface
- [x] Implement syscalls for bash and other system tools
- [x] Simple userspace shell apps ported from xv6

## Fonts
- Adopted 14px fonts from https://font.gohu.org/ and convert to psf1 using bdf2psf.

`bdf2psf gohufont-14.bdf /usr/share/bdf2psf/standard.equivalents /usr/share/bdf2psf/ascii.set 256 gohufont-14.psf`

## How to run
- The disk image file - "hdd.img" in release folder can be used for test.

`qemu-system-x86_64 -serial stdio -M q35 -m 1G -smp 2 -no-reboot -rtc base=localtime -drive id=handisk,if=ide,format=raw,bus=0,unit=0,file=hdd.img`

## Screenshots
- Oct 17, 2023: Userspace Shell Demo

![Cool~~~](https://raw.githubusercontent.com/jjwang/HanOS/main/screenshot/0005-shell.gif)


