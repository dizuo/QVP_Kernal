QVP_Kernal
==========

A node based visually programing framework. It is kernal of IMMComposer.

# IMMComposer


IMMComposer是一个基于结点的通用可视化编程软件。主要内容包含一套基于结点的可视化编程模型，界面友好的交互式创作环境，可视化交互子系统以及插件系统。目前功能模块包含：图像、视频、声音、数学。图像、视频部分基于OpenCV实现，声音部分基于Bass库实现，数学部分基于Lua的math包。

### 图像处理
![IMMComposer](https://github.com/dizuo/QVP_Kernal/blob/master/res/image.jpg)


### 视频处理
![IMMComposer](https://github.com/dizuo/QVP_Kernal/blob/master/res/video.jpg)


### 数学模块
基于Lua实现了一个通用的二元计算控件（BinaryComputer）。下图中BinaryComputer的第二个输入参数Script为Lua脚本，
它定义了该二元计算器的计算功能。通过编写不同的lua脚本，可以实现各种二元计算操作。

![IMMComposer](https://github.com/dizuo/QVP_Kernal/blob/master/res/math.jpg)

图中的lua脚本代码如下：
```
functionbinary(input1, input2)
      return input1 * input2 + math.sqrt(10000/2) – 200
end
```
上述代码中math是Lua自带的数学运算包。除开方函数之外，math函数包还提供了各种常见三角函数，开方，随机数，幂次，对数，运算，取模，绝对值等数值运算函数。

BinaryComputer是一个通用型二元计算器，它兼容一元计算器（将input2置为0即可），更高元计算器可以通过二元计算器组合得到。三元运算器可以通过两个BinaryComputer组合，四元运算器可以通过三个BinaryComputer组合得到，依次类推N元计算器可以通过N-1个BinaryComputer组合得到。
由此可见本文的BinaryComputer足以应对各种数值计算操作。

### 声音模块
![IMMComposer](https://github.com/dizuo/QVP_Kernal/blob/master/res/audio.jpg)


### 互动游戏设计
Face_Detection输出人脸的位置信息，其他控件使用该信息可以实现一些互动效果。
下图为人脸识别互动游戏，左边窗体显示人脸识别图像，人脸用红色圆圈标记，右边窗体显示2D游戏场景。
![IMMComposer](https://github.com/dizuo/QVP_Kernal/blob/master/res/game.jpg)

#### 游戏场景中，
海底的鱼雷左右运动随机向水面发射炸弹，用户通过人脸控制游戏中的小船左右运动躲避炸弹，如果小船遇到炸弹则游戏结束。

#### 控件场景中：
- CvFullWindow是图像窗口代理控件，它与CvWindow不同之处是它所管理的OpenCV窗体大小可以任意改变，而且始终保持图像充满整个窗体。
- StringConsumer控件用来显示图像中的人脸坐标数据，首先对输入的字符串进行解码操作然后显示。
- GameConsumer为2D游戏主窗口的代理控件，它首先将图像中人脸的X坐标变换到游戏的窗口坐标系中，然后将变换后的X坐标赋给游戏场景中的小船，从而实现通过人脸控制二维场景中的小船运动。

### 系统改进设想
1）目前系统的界面以及可视化交互系统是基于Qt的graphics-view框架开发，将来可以将界面和核心分离，界面在web上基于js开发，核心放到服务器上。二者通过HTTP通信。
![IMMComposer](https://github.com/dizuo/QVP_Kernal/blob/master/res/future.jpg)

2）插件系统的改进
目前插件系统存在一个问题，插件DLL中创建的控件对象在系统主框架中删除。控件的创建和删除都应该在DLL中进行。

IMMComposer更加适合触屏下创作啊。。。
