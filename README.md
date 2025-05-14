# Linux-kernel-project
# Linux Kernel Module for Real-Time Health Monitoring

  **Group Name**: YARA  
  **Group Members**: Ethan Rango, Hector Agwara, Michael Echebi  
  **Course**: SCIA 360 - Operating System Security  
  **Project**: Linux Kernel Module for Real-Time Health Monitoring  
  **Date**: May 14, 2025

  ## Overview
  This project implements a Linux kernel module (`sys_health_monitor`) that monitors system health metrics in real-time. The module tracks memory usage, CPU load, and disk I/O, displaying them via `/proc/sys_health`, and triggers kernel alerts when free memory falls below a configurable percentage threshold (default: 80%). A kernel timer checks metrics every 5 seconds.

  ## Prerequisites
  - **Operating System**: Ubuntu 24.04 (kernel 6.11.0-24-generic or compatible).
  - **Kernel Headers**: Install `linux-headers-$(uname -r)`:
    ```bash
    sudo apt update
    sudo apt install linux-headers-$(uname -r) build-essential
    ```
  - **Root Privileges**: Required for loading/unloading the kernel module.
  - **Optional Testing Tool**: `stress-ng` for simulating system load:
    ```bash
    sudo apt install stress-ng
    ```

  ## Installation
  1. Clone or navigate to the project directory:
     ```bash
     cd ~/Downloads
     ```
  2. Ensure the following files are present:
     - `sys_health_monitor.c`: Kernel module source code.
     - `Makefile`: Build configuration.
  3. Build the module:
     ```bash
     make
     ```
     This creates `sys_health_monitor.ko`.

  ## Usage
  1. Load the kernel module (default `mem_threshold=80`%):
     ```bash
     sudo insmod sys_health_monitor.ko
     ```
     Optionally, set a custom memory threshold (e.g., 25%):
     ```bash
     sudo insmod sys_health_monitor.ko mem_threshold=25
     ```
  2. Read system health metrics:
     ```bash
     cat /proc/sys_health
     ```
     Example output:
     ```
     --- System Health Monitor (YARA) ---
     Group Members: Ethan Rango, Hector Agwara, Michael Echebi
     Memory Total: 7428 MB
     Memory Free:  1374 MB
     CPU Load Avg (1min, 5min, 15min): 0.15, 0.20, 0.18
     Disk I/O (Sectors Read, Written): 1024, 512
     --------------------------------------
     Memory Alert Threshold (Free % < ): 25%
     ```
  3. Check kernel logs for alerts:
     ```bash
     dmesg | grep YARA
     ```
     Example alert (if free memory < 25%):
     ```
     [YARA] Alert: Free Memory (18%) is below threshold (25%)!
     ```
  4. Unload the module:
     ```bash
     sudo rmmod sys_health_monitor
     ```

  ## Testing
  To test the module, use `stress-ng` to simulate system load:
  ```bash
  sudo apt install stress-ng
  stress-ng --vm 2 --vm-bytes 512M --cpu 2 --io 1 --timeout 60s &
  ```
  While running, check metrics and logs:
  ```bash
  cat /proc/sys_health
  dmesg | grep YARA
  ```
  To trigger an alert, set a high threshold:
  ```bash
  sudo insmod sys_health_monitor.ko mem_threshold=95
  ```

  ### Test Results
  Tested on Ubuntu 24.04 (kernel 6.11.0-24-generic):
  - **Build**: `make` succeeded with minor warnings (compiler mismatch, BTF generation skipped).
  - **Loading**: Loaded with `mem_threshold=25`. Logs confirmed:
    ```
    [YARA] Loading System Health Monitor Module (Members: Ethan, Hector, Michael).
    ```
  - **Metrics**: `/proc/sys_health` showed 7428 MB total, 1374 MB free (18%), CPU load (e.g., 0.15, 0.20, 0.18), and disk I/O (e.g., 1024 sectors read, 512 written).
  - **Alerts**: Alerts triggered every 5 seconds as free memory was below 25%:
    ```
    [YARA] Alert: Free Memory (18%) is below threshold (25%)!
    ```
  Full details are in `PROJECT_REPORT.md`.

  ## Troubleshooting
  - **dmesg Permissions**:
    If `dmesg` fails with `Operation not permitted`:
    ```bash
    sudo usermod -aG adm $(whoami)
    ```
    Log out and back in, then retry `dmesg`.
  - **Compiler Warning**:
    A warning about differing compilers (`x86_64-linux-gnu-gcc-14` vs. `gcc-14`) can be ignored if the module loads successfully.
  - **BTF Generation**:
    The `Skipping BTF generation` warning is due to missing `vmlinux` and does not affect functionality.
  - **Compilation Errors**:
    If `si_meminfo` errors occur, ensure `<linux/mm.h>` is used and kernel headers are installed:
    ```bash
    sudo apt install --reinstall linux-headers-$(uname -r)
    ```

  ## Current Implementation Notes
  - **Implemented**:
    - **Memory Monitoring**: Tracks total and free memory (in MB) using `si_meminfo`, with percentage-based threshold alerts.
    - **CPU Load Monitoring**: Uses `get_avenrun` to retrieve load averages (1, 5, and 15 minutes), displayed in `/proc/sys_health`.
    - **Disk I/O Monitoring**: Uses `part_stat` to collect sectors read and written, displayed in `/proc/sys_health`.
  - **Future Work**: Enhance alert conditions for CPU and disk I/O, add a user-space UI to visualize metrics.
  - **Dependencies**: Requires kernel headers and `linux/mm.h` for `si_meminfo`, `linux/sched.h` for `get_avenrun`, and block layer headers for `part_stat`.

  ## Makefile Targets
  - Build: `make`
  - Clean: `make clean`
  - Load: `make load`
  - Unload: `make unload`

  ## License
  This module is licensed under the GPL.
