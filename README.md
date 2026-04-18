<h1>HO-Cap Data Capture Guide</h1>

<h2>Table of Contents</h2>

- [Hardware Studio Setup](#hardware-studio-setup)
- [Environment Setup](#environment-setup)
- [Camera Calibration](#camera-calibration)
- [References](#references)

## Hardware Studio Setup

Please refer to the [Hardware Studio Setup](./docs/hardware_studio_setup.md) document for details on the hardware setup used for data capture.

---

## Environment Setup

Please refer to the [Environment Setup](./docs/environment_setup.md) document for details on how to set up the software environment for data capture.

---

## Camera Calibration

The [RealSense Calibration & Recording Guide](https://docs.google.com/document/d/1jflhWGTn4lsGj5PiJ14Q37k0xHSnkTuDJvkdFr_j-fo) on DexYCB[^1] provides a good reference for camera calibration. We use the same calibration tool as described in the DexYCB paper, which is based on the [vicalib](https://github.com/arpg/vicalib).

For your convenience, we have also included the calibration tool in our repository. You can find it in the `calibration` folder. Please refer to the [Camera Calibration](./calibration/README.md) document for details on how to build the calibration tool.

---

## References

[^1]: [DexYCB: A Benchmark for Capturing Hand Grasping of Objects](https://arxiv.org/abs/2203.14927)
