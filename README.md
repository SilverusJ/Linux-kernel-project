Linux Kernel Module for Real-Time Health Monitoring
  Group Name: YARA  Group Members: Ethan Rango, Hector Agwara, Michael Echebi  Course: SCIA 360 - Operating System Security  Project: Linux Kernel Module for Real-Time Health Monitoring
Overview
  This project implements a Linux kernel module (sys_health_monitor) that monitors system health metrics in real-time. The module creates a /proc/sys_health entry to display memory usage (total and free memory in MB) and triggers kernel alerts when free memory falls below a configurable threshold (default: 80% of total memory). The module uses a kernel timer to check metrics every 5 seconds. Placeholders for CPU load and disk I/O metrics are included but not implemented in this version.
Prerequisites

Operating System: Ubuntu 24.04 (kernel 6.11.0-24-generic or compatible).
Kernel Headers: Install linux-headers-$(uname -r):sudo apt update
sudo apt install linux-headers-$(uname -r) build-essential


Root Privileges: Required for loading/unloading the kernel module.
Optional Testing Tool: stress-ng for simulating memory load:sudo apt install stress-ng



Installation

Clone or navigate to the project directory:cd ~/Downloads


Ensure the following files are present:
sys_health_monitor.c: The kernel module source code.
Makefile: The build configuration.


Build the module:make

This creates sys_health_monitor.ko.

Usage

Load the kernel module (default mem_threshold=80%):sudo insmod sys_health_monitor.ko

Optionally, set a custom memory threshold (e.g., 25%):sudo insmod sys_health_monitor.ko mem_threshold=25


Read system health metrics:cat /proc/sys_health

Example output:--- System Health Monitor (YARA) ---
Group Members: Ethan Rango, Hector Agwara, Michael Echebi
Memory Total: 7428 MB
Memory Free:  1374 MB
--------------------------------------
Memory Alert Threshold (Free % < ): 25%


Check kernel logs for alerts:dmesg | grep YARA

Example alert (if free memory < 25%):[YARA] Alert: Free Memory (18%) is below threshold (25%)!


Unload the module:sudo rmmod sys_health_monitor



Testing
  To test memory alerts, use stress-ng to consume memory:
sudo apt install stress-ng
stress-ng --vm 2 --vm-bytes 512M --timeout 60s &

  While running, check /proc/sys_health and dmesg:
cat /proc/sys_health
dmesg | grep YARA

  To trigger an alert, set a high threshold:
sudo insmod sys_health_monitor.ko mem_threshold=95

Test Results
  The module was tested on Ubuntu 24.04 (kernel 6.11.0-24-generic):

Build: make succeeded, producing sys_health_monitor.ko. A compiler mismatch warning appeared but did not affect functionality.
Loading: Loaded with sudo insmod sys_health_monitor.ko mem_threshold=25. Logs confirmed successful loading:[YARA] Loading System Health Monitor Module (Members: Ethan, Hector, Michael).
[YARA] /proc/sys_health entry created.
[YARA] Health check timer started (runs every 5 seconds).


Metrics: /proc/sys_health reported:Memory Total: 7428 MB
Memory Free:  1374 MB
Memory Alert Threshold (Free % < ): 25%


Alerts: Free memory (1374 MB) was 18% of total, below the 25% threshold, triggering alerts:[YARA] Alert: Free Memory (18%) is below threshold (25%)!


Issues: Initial dmesg access required sudo due to permissions (fixed by adding user to adm group).

Troubleshooting

dmesg Permissions:If dmesg fails with Operation not permitted:sudo usermod -aG adm $(whoami)

Log out and back in, then retry dmesg.
Compiler Warning:A warning about differing compilers (x86_64-linux-gnu-gcc-14 vs. gcc-14) can be ignored if the module loads successfully.
BTF Generation:The Skipping BTF generation warning is due to missing vmlinux and does not affect functionality.

Current Implementation Notes

Implemented: Memory monitoring (total/free memory in MB) with percentage-based threshold alerts.
Not Implemented: CPU load and disk I/O metrics are placeholders. Future versions may use get_avenrun for CPU load and block layer stats for disk I/O.
Known Issues: Ensure kernel headers are installed. If compilation fails with si_meminfo errors, verify <linux/mm.h> is accessible via linux-headers-$(uname -r).

Makefile Targets

Build: make
Clean: make clean
Load: make load
Unload: make unload

License
  This module is licensed under the GPL.
