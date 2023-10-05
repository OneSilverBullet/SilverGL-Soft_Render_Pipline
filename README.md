# SilverGL-Soft_Render_Pipline

### 0. Acknowledgments & Why do you do this open source project?

Thanks to the guidance of **zxx43**, I have a better understanding of the soft rendering pipeline.

Thanks to **Angelo1211** for his project, which helped me deepen my understanding of the rendering pipeline.

Computer Graphics, as the three major romances of computer science, always have a deep attraction for programmers. Especially today, when AAA computer games are becoming more and more popular, for me who wants to enter the game industry, I am eager to break the black box of rendering in games. I no longer want to rely on OpenGL/DX to achieve some cool effects. I want to have a deeper understanding of how to implement a PBR renderer with good performance from one line of C++. This is the core purpose of this open-source project. My goals for this project are summarized as follows:

(1) Put my C++ knowledge to use.

(2) Deepen my understanding of graphics.

(3) Break the black box of rendering and put the learned rendering theory into practice.


### 1. Basic Information
* Programming language: C++
* Based on the framework: Windows
* Other reference libraries: stb_image
* Experimental environment: Visual Studio 2017
* Note: This project only requires you to have C++ knowledge. I develop this project based on the Windows framework. You only need to check a few functions to understand the drawing mechanism of Win, and you do not need to add any other knowledge. The entire project uses C++ to practice graphics rendering theory. This means that if you want to understand how rendering is done, but are limited by the steep learning curve of graphics APIs or other graphics frameworks, then this project can help you.

### 2. Features
* Basic mathematics library, including MATRIX, VECTOR, etc.
* Basic data structures, including FrameBuffer, DepthBuffer, Triangle, etc.
* Raster rendering, raster calculation using scan line algorithm and barycenter coordinate method, the project includes 3 different implementations.
* A relatively complete graphics rendering pipeline, including initialization of equipment, coordinate conversion, viewport mapping and inverse mapping, triangle Clip and Cull, and other operations.
* A basic Shader system based on function pointers, including Vertex Shader and Fragment Shader, which can implement basic Diffuse lighting.
* An advanced Shader system based on virtual functions that implement classic lighting models such as Blinn Phong, Texture Shader, and PBR Cook Torrance.
* A relatively complete Texture module that supports Linear, Nearest, and Tile sampling, and supports the reading of colors, normal maps, AO maps, etc.
* Complete model loading system, supporting the loading of obj models.
* Display manager based on Windows framework.
* Key control system based on Windows framework.
* Basic GameObject system and transformation system.
* Basic resource management system, which can manage the resource manager of Texture and Base Shader.


### 3. Performance

![基础的Cube绘制（Diffuse光照）](https://github.com/OneSilverBullet/SilverGL-Soft_Render_Pipline/blob/master/Output/Result.png)
![PBR绘制木箱（不同颜色光照效果）](https://github.com/OneSilverBullet/SilverGL-Soft_Render_Pipline/blob/master/Output/Res0.png)
![PBR绘制木箱（不同颜色光照效果）](https://github.com/OneSilverBullet/SilverGL-Soft_Render_Pipline/blob/master/Output/Res1.png)
![PBR绘制木箱（不同颜色光照效果）](https://github.com/OneSilverBullet/SilverGL-Soft_Render_Pipline/blob/master/Output/Res2.png)
![PBR绘制火枪](https://github.com/OneSilverBullet/SilverGL-Soft_Render_Pipline/blob/master/Output/Res3.png)
![PBR绘制消防栓](https://github.com/OneSilverBullet/SilverGL-Soft_Render_Pipline/blob/master/Output/Res4.png)
