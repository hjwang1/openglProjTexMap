# 3D视觉感知小工具Tool

这里开源的工程是一个小工具，是在研究视觉感知算法时的辅助工具，是以OpenGL的纹理映射技术为基础，可以模拟投影仪或3D扫描仪。

## OpenGL投影纹理映射

OpenGL加载图片纹理后，根据投影原理，把其投射到可视空间内3D物体的表面（Surface），不同的表面形状会对纹理产生各自不同的扭曲。
不同物体表面的投影：![image](https://raw.githubusercontent.com/hjwang1/openglProjTexMap/master/img/326238085.jpg)
小工具的功能与使用方法：

 1. **二维图片纹理加载** ，使用SOIL加载，支持多种不同格式的图片纹理；
 2. 着色器文件加载，GLSL实现的着色器程序以文件形式存储，支持着色器文件加载；
### 鼠标操作
多视角观察投影：![image](https://raw.githubusercontent.com/hjwang1/openglProjTexMap/master/img/1189853088.jpg)
 |    动作   |功能                          |备注                         
|----------------|-------------------------------|-----------------------------|
|点击左键+移动|转换视角            |可以左右上下切换视角          |
|滚轮前后滚动          |视角拉近拉远            |前滚拉远视角&后滚拉近视角           |
|点击右键          |视角重置            |视角切换到原始状态           |
|点击滚轮或中键          |调出菜单|调出菜单后鼠标左键点击选择|

### 键盘操作
 |    动作   |功能                          |备注                         
|----------------|-------------------------------|-----------------------------|
|点击‘w’键|Model上移            |3D空间内的所有物体向上移动          |
|点击‘s’键|Model下移            |3D空间内的所有物体向下移动          |
|点击‘a’键|Model左移            |3D空间内的所有物体向左移动          |
|点击‘d’键|Model右移            |3D空间内的所有物体向右移动          |
> 键盘操作的代码已经注释，需要时可以打开。
> glTranslatef(dx, dy, dz);
### 菜单操作
菜单简单操作：![菜单操作](https://raw.githubusercontent.com/hjwang1/openglProjTexMap/master/img/777228779.jpg)

## 模拟投影仪
典型投影环境
### 平面正投
![平面正投](https://raw.githubusercontent.com/hjwang1/openglProjTexMap/master/img/334946071.jpg)
### 平面斜投
![平面斜投](https://raw.githubusercontent.com/hjwang1/openglProjTexMap/master/img/301850103.jpg)
### 曲面投影
![曲面投影](https://raw.githubusercontent.com/hjwang1/openglProjTexMap/master/img/487757520.jpg)


## 模拟3D扫描仪
同理，您可以自行实现模拟3D扫描仪的功能。

## 系统环境
- [x] ubuntu 16.04 linux
- [x] libGL
- [x] libGLU
- [x] libglut
- [x] libGLEW
- [x] libSOIL
