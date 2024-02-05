# CloudComputing
System vs OS Virtualization

Learning Objectives
The aims of the assignment include the following learning objectives:
● Understanding the real-world use case of two virtualization techniques / technologies:
○ System Virtualization via QEMU
○ OS Virtualization via Docker
● Understand the operations related to each technologies
● Understand and demonstrate the performance differences of each technology using
benchmarks and finally compare the results in a comprehensive report.
Environment Setup
You are free to use your personal computer for this assignment. The set of requirements for
using your personal computers is as follows:
● x64 CPU with at least 2 cores or a Mac Apple Silicon
● 4 GB memory
● 20 GB free disk space
● OS: Windows, Linux (Ubuntu preferred) or Mac OS X
If you are using a personal computer that does not support such requirements, please do reach
out to me, so that we can discuss some options.

System Virtualization (QEMU) Setup

What is QEMU?
QEMU is a free and open-source hypervisor. It emulates the machine's processor through
dynamic binary translation and provides a set of different hardware and device models for the
machine, enabling it to run a variety of guest operating systems. It is more lightweight than
virtualbox and allows near-native speed (when using QEMU with KVM on Linux). QEMU
supports various types of CPUs, such as x64 and ARM, and also supports integrations with
various host operating systems. It is less user-friendly and requires more effort to setup.

Installing QEMU

For All Operating Systems

For this assignment, you will be installing a Ubuntu guest virtual machine. Thus you must
download the appropriate Ubuntu 20.04 or 16.04 server ISO image from one of the links below:

● x64 CPU on Linux or Mac: Ubuntu 20.04 Server
● x64 CPU on Windows: Ubuntu 16.04 Server
● ARM (Apple Silicon): Ubuntu 20.04 Server for ARM

From here, the installation instruction varies between which OS you are running.
Optional helpful tips:

● You can set -nographic to run directly in the terminal.
● Another option is to SSH from your host machine to the VM by adding the following flags
to enable port mapping from 8888 to 22 and run the SSH command shown below.
● -netdev user,id=net0,hostfwd=tcp::8888-:22
● $ ssh -p 8888 localhost
● Do the above only if you have successfully launched a VM!

Linux Installation instructions

QEMU is the easiest to install if you have Ubuntu. You can install qemu (under Ubuntu) simply
by typing the following command in the terminal:

$ sudo apt-get install qemu
You can then create the QEMU Image by running the following command.

$ sudo qemu-img create ubuntu.img 10G -f qcow2
Given the image, install the VM using the command below (which takes the iso file as a “cdrom”
and the qemu image as a “hard disk”):

$ sudo qemu-system-x86_64 -hda ubuntu.img -boot d -cdrom ./[UBUNTU_SERVER_ISO_FILE_NAME] -m 2046 -boot strict=on

For each of these commands, please refer to the QEMU manual for more details [1].

Mac OS X (x64) Installation instructions

The instructions to install QEMU on the x64 Macs can be found in the following link
https://wiki.qemu.org/Hosts/Mac. I suggest using the homebrew method as it is the easiest. You
can find homebrew installation instructions in [2], if it is not already installed in your machine.
Once QEMU is installed, you can follow a similar procedure as the Linux installation to create a
QEMU image and install the VM. Things to watch out for:

● Make sure to update homebrew
● Try 20.04, if it doesn’t work 16.04 may work.

Mac OS X (Apple Silicon) Installation instructions

Given that Apple Silicon is a relatively new processor, there are not many resources available to
simply install QEMU with a single command line. Once you install qemu using brew as similar to
above, first make sure you have version 8.2.0+. If not, you should upgrade to 6.2.0.
Also, make sure you are not running your terminal via Rosetta. To check, type in arch in the
terminal to make sure it is arm64, not i386.
Here is a set of commands that may work:

● $ qemu-system-aarch64 \
-accel hvf -cpu cortex-a57 -M virt,highmem=off -m 2048
-smp 2 \
-drive
file=/usr/local/share/qemu/edk2-aarch64-code.fd,if=pflash,format
=raw,readonly=on \
-drive if=none,file=ubuntu.img,format=qcow2,id=hd0 \
-device virtio-blk-device,drive=hd0,serial="dummyserial" \
-device virtio-net-device,netdev=net0 \
-netdev user,id=net0 \
-vga none -device ramfb \
-cdrom ubuntu-20.04.4-live-server-arm64.iso \
-device usb-ehci -device usb-kbd -device usb-mouse -usb \
-nographic

Note that the VM image file names may be different based on which version you have
downloaded. Make sure you download the ARM64 image.

Windows Installation Instructions

Running QEMU on Windows is NOT RECOMMENDED, as most servers are Linux/Unix based
and most open source virtualization works best with Unix based OSes. However, it is certainly
possible. Here is an article that describes the entire process in detail in [3], which requires you
to be able to change some Window environments variables.

● You may need to install VcXsrv to connect to the VM instance. Once you have
connected Ubuntu to the display, follow the Linux instructions on the homework.
OS Virtualization (Docker) Setup


What is Docker?

Docker is a set of platforms that enable OS-level virtualization to deliver software in packages
called containers. Docker is the most popular container management platform.
Installing Docker

While we will be installing Docker Desktop (for Windows and Mac), we will be interacting mostly
with Docker CLI. Docker Desktop installation will include Docker Engine, Docker CLI client,
Docker Compose and more. You can learn more about the Docker CLI in [5]. I have included the
corresponding links to find the installation instructions for each type of OSes. Installing Docker
should be much simpler than installing QEMU.

Linux Installation instructions

Note: Ubuntu does not come with Docker Desktop, so we would just need to install the Docker
Engine and the Docker CLI separately.

● https://docs.docker.com/engine/install/ubuntu/
Mac OS X Installation instructions
● x64 CPU: https://docs.docker.com/desktop/mac/install/
● ARM (Apple Silicon):: https://docs.docker.com/desktop/mac/apple-silicon/
Windows Installation instructions
● https://docs.docker.com/desktop/windows/install/

Installing sysbench

Sysbench[4] is an open-source and multi-purpose benchmark utility that evaluates the
parameter features tests for CPU, memory, I/O and much more. We will use sysbench to
understand the performance characteristics of each of the virtualization technologies.
https://github.com/akopytov/sysbench/issues/140
For Ubuntu Guest VM
Sysbench installation on Ubuntu is very simple. Just run the following commands:

$ sudo apt update
$ sudo apt install sysbench

Note: Do not be confused with the password for the guest VM.

For Docker

For Docker, you will be creating your own image by adding sysbench to a base image.
Based on your choice of Ubuntu version and your architecture, you will choose a base image in
Docker Hub. You can find the correct Docker docker images in https://hub.docker.com/_/ubuntu.
Make sure to match the ubuntu version and your chip! Very important!
Once the base image is downloaded, you must install sysbench similar to how you installed it on
the VM. Note that the base image is quite light, so it may not contain simple commands like
sudo or ping.
You must describe the commands you ran to create your image, your final image ID, and
the screenshot of your image history!

Note on Sysbench Versions!

The sysbench version on your linux machine and the Docker container may differ, as the
sysbench version is updated continuously. Therefore, the output values may seem like they are
different. To eliminate this issue, make sure you have the same version of sysbench on both
experiment