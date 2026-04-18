# Intel® RealSense™ Documentation

ref: https://dev.intelrealsense.com/docs/docs-get-started

## Firmware Update

- [D400 Series firmware downloads](https://dev.realsenseai.com/docs/firmware-releases-d400/)
- [Update firmware](https://dev.realsenseai.com/docs/firmware-update-tool/)
  - List all available devices

    ```sh
    rs-fw-update -l
    ```

    Example output:

    ```
    Connected devices:
    1) Name: Intel RealSense D455, serial number: 105322251564, update serial number: 104623060340, firmware version: 05.13.00.50, USB type: 3.2
    2) Name: Intel RealSense D455, serial number: 108222250342, update serial number: 108323060216, firmware version: 05.13.00.50, USB type: 3.2
    ```

  - Update firmware

    ```sh
    rs-fw-update -f <path_to_firmware_file> -s <device_serial_number>
    ```

    Example command:

    ```sh
    rs-fw-update -f Signed_Image_UVC_5_13_0_50.bin -s 105322251564
    ```

    ```
    Search for device with serial number: 105322251564

    Updating device:
    Name: Intel RealSense D455, serial number: 105322251564, update serial number: 104623060340, firmware version: 05.13.00.50, USB type: 3.2
     07/12 13:13:31,627 ERROR [139925643202880] (librealsense-exception.h:52) get_xu(...). xioctl(UVCIOC_CTRL_QUERY) failed Last Error: Protocol error

    Firmware update started

    Firmware update progress: 100[%]

    Firmware update done
     07/12 13:13:55,828 ERROR [139925634782976] (librealsense-exception.h:52) get_dev_capabilities: Cannot open '/dev/video7 Last Error: Permission denied
     07/12 13:13:55,829 ERROR [139925634782976] (librealsense-exception.h:52) get_dev_capabilities: Cannot open '/dev/video5 Last Error: Permission denied

    Device 105322251564 successfully updated to FW: 05.13.00.50
    ```

  - Update firmware when camera in recovery state
    - List all available devices

    ```sh
    rs-fw-update -l
    ```

    ```
    connected devices:
    1. Name: Intel RealSense D455, serial number: 108222250342, update serial number: 108323060216, firmware version: 05.13.00.50, USB type: 3.2
    2. Name: Intel RealSense D4xx Recovery, serial number: unknown, update serial number: 104623060340, firmware version: unknown, USB type: unknown
    ```

    - Recover all connected devices which are in recovery mode

    ```sh
    # rs-fw-update -r -f <path_to_firmware_file>
    ```

    Example command:

    ```sh
    rs-fw-update -r -f Signed_Image_UVC_5_13_0_50.bin
    ```

    ```
     07/12 13:17:31,627 ERROR [139925643202880] (librealsense-exception.h:52) get_xu(...). xioctl(UVCIOC_CTRL_QUERY) failed Last Error: Protocol error

     07/12 13:17:31,628 ERROR [139925643202880] (librealsense-exception.h:52) get_dev_capabilities: Cannot open '/dev/video7 Last Error: Permission denied
     07/12 13:17:31,628 ERROR [139925643202880] (librealsense-exception.h:52) get_dev_capabilities: Cannot open '/dev/video5 Last Error: Permission denied

     Device 104623060340 successfully

    update to FW: Signed_Image_UVC_5_13_0_50.bin

    recovering device:
    Name: Intel RealSense D4xx Recovery, serial number: unknown, ASIC serial number: 012345678901, firmware version: unknown, USB type: unknown

    firmware update started

    firmware update progress: 100[%]

    firmware update done

    recovery done
    ```

## Useful Commands

- List all connected D455 devices

  ```sh
  lsusb | grep "8086:0b5c"
  ```

  > Bus 002 Device 029: ID 8086:0b5c Intel Corp. Intel(R) RealSense(TM) Depth Camera 455
  > Bus 002 Device 027: ID 8086:0b5c Intel Corp. Intel(R) RealSense(TM) Depth Camera 455
  > Bus 002 Device 025: ID 8086:0b5c Intel Corp. Intel(R) RealSense(TM) Depth Camera 455
  > Bus 002 Device 023: ID 8086:0b5c Intel Corp. Intel(R) RealSense(TM) Depth Camera 455
  > Bus 002 Device 021: ID 8086:0b5c Intel Corp. Intel(R) RealSense(TM) Depth Camera 455
  > Bus 002 Device 019: ID 8086:0b5c Intel Corp. Intel(R) RealSense(TM) Depth Camera 455
  > Bus 002 Device 017: ID 8086:0b5c Intel Corp. Intel(R) RealSense(TM) Depth Camera 455
  > Bus 002 Device 015: ID 8086:0b5c Intel Corp. Intel(R) RealSense(TM) Depth Camera 455

  ```sh
  rs-enumerate-devices -S
  ```

- Kernel log messages

  ```sh
  # retrieve the last Linux Kernel log messages with timestamps
  dmesg -T

  # monitor the Linux Kernel log messages when there any change
  dmesg -wH

  # clear the dmesg buffer
  sudo dmesg -c
  ```
