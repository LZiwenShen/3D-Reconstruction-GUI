3D Turntable Laser Scanner GUI

This project is a Qt-based GUI application developed as part of Brown University's ENGN2502 Fall 2024 course: **3D Photography**. It is used to control a Raspberry Pi–based laser scanner to automate the image capture, calibration, and 3D reconstruction process.
 
> Author: Ziwen Shen  
> Based on code and framework by Prof. Gabriel Taubin

---

## Overview

This software connects to a Raspberry Pi–based 3D laser scanner via socket and Samba protocols, allowing users to:

- Automate capture of images while rotating a turntable and controlling lasers
- Perform **camera**, **turntable**, and **laser plane** calibration
- Load and visualize scan sequences
- Run triangulation-based 3D reconstruction

The scanner captures images with different laser configurations (blank, laser1, laser2), and the GUI provides modular panels for each processing stage.

---

## Key Features

### Scanner Control
- Connects over LAN or WiFi (supports `.local` hostname resolution)
- Sends scan commands with customizable frame angle and number
- Supports real-time image reception (`Send Images`) or file-based sharing (`Save Images` via Samba)

### Calibration Panels
- **Camera Calibration**: Estimates intrinsic parameters using OpenCV and checkerboard detection  
- **Turntable Calibration**: Estimates rotation axis and surface plane  
- **Laser Plane Calibration**: Computes laser planes using checkerboard-illuminated points

### Scanning & Reconstruction
- Supports multi-frame, multi-laser scanning
- Performs frame-wise triangulation
- Optional color point cloud generation (`With Color`)
  ```bash
  brew install opencv
