# SilverGL-Soft_Render_Pipeline

## 🌟 Introduction

Welcome to **SilverGL-Soft_Render_Pipeline** – an open-source project focused on implementing a high-performance soft rendering pipeline in C++. This project is built from scratch to demystify the inner workings of rendering in computer graphics, while pushing the boundaries of what can be achieved with a pure C++ approach. 

### 🏆 Why Open Source?

Thanks to the valuable guidance from **zxx43**, my understanding of the soft rendering pipeline has significantly deepened. I also want to acknowledge **Angelo1211** for their project, which further influenced the development of this pipeline.

As a computer graphics enthusiast, I’ve always been fascinated by the complexities of rendering, especially in the world of AAA game development. By embarking on this project, I aim to:

- Apply my **C++** knowledge to a real-world rendering pipeline.
- Gain deeper insights into **computer graphics** theory and implementation.
- Move beyond the black box of game engines, creating a custom **PBR renderer** and implementing lighting models from scratch.

---

## 📋 Project Overview

### 📍 Key Features

Here’s a quick overview of what this project offers:

| **Feature**                          | **Description**                                                                 |
|--------------------------------------|---------------------------------------------------------------------------------|
| **Math Library**                     | MATRIX, VECTOR, and other essential math utilities.                              |
| **Data Structures**                  | FrameBuffer, DepthBuffer, Triangle, etc., to support rendering functionality.   |
| **Raster Rendering**                 | Scan line algorithm, barycentric coordinates, and multiple rasterization methods.|
| **Graphics Pipeline**                | Complete pipeline: initialization, coordinate conversion, clipping, culling, etc.|
| **Shader System**                    | Basic & advanced shaders: Diffuse, Blinn Phong, Texture, and PBR Cook-Torrance.  |
| **Texture Module**                   | Supports Linear, Nearest, and Tile sampling with colors, normal maps, and AO.   |
| **Model Loading**                    | .obj model loader for 3D assets.                                                 |
| **Resource Management**              | Efficient management of textures and shaders.                                    |
| **Control Systems**                  | Key input system and display manager based on the Windows framework.            |

---

### ⚙️ Technical Details

- **Programming Language**: C++
- **Framework**: Windows
- **Reference Libraries**: stb_image
- **Development Environment**: Visual Studio 2017

> **Note**: This project requires only basic C++ knowledge. It’s designed with simplicity in mind, so even beginners to graphics programming can get started with the fundamentals of rendering.

---

## 📸 Performance Showcase

The following images demonstrate the rendering performance and visual effects achievable with **SilverGL-Soft_Render_Pipeline**.

- **PBR Lighting Effects**:

    ![PBR Wooden Box 1](https://github.com/OneSilverBullet/SilverGL-Soft_Render_Pipline/blob/master/Output/Res0.png)
    ![PBR Wooden Box 2](https://github.com/OneSilverBullet/SilverGL-Soft_Render_Pipline/blob/master/Output/Res1.png)
    ![PBR Wooden Box 3](https://github.com/OneSilverBullet/SilverGL-Soft_Render_Pipline/blob/master/Output/Res2.png)
