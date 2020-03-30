# SilverGL-Soft_Render_Pipline

### 0. 致谢 & 为什么做这个开源项目？

感谢 zxx43 大佬的指导，让我对软渲染管线有进一步的理解。

感谢 Angelo1211 大佬的项目，让我加深了对渲染管线的理解。

图形学作为计算机科学的三大浪漫，一直以来都对程序员有着很深的吸引力。尤其是3A电脑游戏越加流行的今天，对想要去进入游戏行业的我来说，我渴望打破游戏当中渲染的黑盒。我不想再依托于Opengl/DX去实现一些炫酷的效果，我想深入了解如何从一行C++去实现一个表现效果良好的PBR渲染器，这就是我做这个开源项目的核心目的。我做这个项目的目标整理如下：

（1） 让我的C++知识有用武之地。

（2） 加深我对图形学的理解。

（3） 打破渲染的黑盒，将学习到的渲染理论加以实战。

### 1. 项目基本信息
* 编程语言：C++
* 基于框架：Windows
* 其他引用库：stb_image
* 实验环境：Visual Studio 2017
* 说明：这个项目只需要你具有C++知识就可以进行阅读学习。其绘制函数使用的是Windows框架，这意味着你只需要查一查几个函数弄清楚win的绘制机理，就不需要补充其他任何知识了。整个项目完全使用C++来对图形学渲染理论进行实战，这意味着如果你希望了解渲染是怎么做的，但是受制于图形API或其他图形框架的陡峭学习曲线，那么这个项目可以告诉你渲染究竟是怎么做到的。

### 2. 项目实现的功能
* 基础的数学库，包含MATRIX,VECTOR等。
* 基础的数据结构,包括FrameBuffer，DepthBuffer，Triangle等。
* 光栅化渲染，使用扫描线算法与重心坐标法进行的光栅化计算，项目中包含3种不同方式的实现。
* 一个较为完善的图形渲染管线，包含初始化设备，坐标转换，视口映射和逆映射，三角形的Clip与Cull等操作。
* 一个基于函数指针的基础Shader系统，包含Vertex Shader和Fragment Shader,可以实现基础的Diffuse光照。
* 一个基于虚函数的进阶Shader系统，实现Blinn Phong， Texture Shader, PBR Cook Torrance等经典光照模型。
* 一个BMP图像加载模块，以及jp、png图像加载模块（stb_image）。
* 较为完善的Texture模块，支持Linear、Nearest以及Tile采样，支持颜色、法线图、AO图等的读取。
* 完整的模型加载系统，支持obj模型的加载。
* 基于Windows框架的显示管理器。
* 基于Windows框架的按键控制系统。
* 基础的GameObject系统以及变换系统。
* 基础的资源管理系统，可以管理Texture和Base Shader的资源管理器。

### 3. 目标任务
* 将代码进行整理和调优，还需要进行解耦。
* 资源管理器需要进一步解耦并且使其更加灵活。
* 场景部分需要使其更加灵活，达到随意增加。
* 考虑重构代码，构建材质模块，使渲染过程OOP化。

### 4. 目前实现的效果

![基础的Cube绘制（Diffuse光照）](https://github.com/OneSilverBullet/SilverGL-Soft_Render_Pipline/blob/master/Output/Result.png)
![PBR绘制木箱（不同颜色光照效果）](https://github.com/OneSilverBullet/SilverGL-Soft_Render_Pipline/blob/master/Output/Res0.png)
![PBR绘制木箱（不同颜色光照效果）](https://github.com/OneSilverBullet/SilverGL-Soft_Render_Pipline/blob/master/Output/Res1.png)
![PBR绘制木箱（不同颜色光照效果）](https://github.com/OneSilverBullet/SilverGL-Soft_Render_Pipline/blob/master/Output/Res2.png)
![PBR绘制火枪](https://github.com/OneSilverBullet/SilverGL-Soft_Render_Pipline/blob/master/Output/Res3.png)
![PBR绘制消防栓](https://github.com/OneSilverBullet/SilverGL-Soft_Render_Pipline/blob/master/Output/Res4.png)
