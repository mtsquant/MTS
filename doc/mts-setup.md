[TOC]

# MTS 安装手册

MTS 提供多个系统平台版本，以满足不同用户的需要

## Windows 版本

### 系统需求
1. 64 位 windows （7 SP1 ，8，10）
1. Microsoft Visual Studio 2015 x64 运行时

### 安装步骤
1. 安装 Anaconda2 x64
请注意更改默认安装路径到 `C:\Anaconda2`。
如果系统已经安装有别的版本的python 2（必须是64位版本），可以使用mklink 建立符号连接到 C:\Anaconda2
1. 安装MTS
当前版本暂时不可以选择安装路径，自动安装到系统C盘。

### MTS说明
安装MTS后，会在系统中产生：
1. MTS python 库`(C:\Anaconda2\lib\MTS)`，供MTS python 策略调用。 
1. MTS 可执行组件Cindy和Julia(`C:\MTS`)，分别提供实盘模拟盘监控以及策略研发环境。

***

## linux 版本

### 敬请期待

***

## docker 版本

### 系统需求
1. 64 位Ubuntu 系统
2. docker ce 环境

### 安装步骤
1. 拉取镜像
```bash
sudo docker pull registry.cn-hangzhou.aliyuncs.com/mts/mts:20180314
```
2. 运行测试
```bash
sudo docker run --rm registry.cn-hangzhou.aliyuncs.com/mts/mts:20180314
```
此时屏幕上将出现不断滚动的行情更新信息，表示工作正常

### 运行方式
1. 运行当前目录下的策略脚本
```bash
#假设运行当前目录下的tactic1.py策略脚本
sudo docker run -it --rm -v "$PWD":/root/dev/MTS/workspace/tactic1 mts:20180314 python /root/dev/MTS/workspace/tactic1/tactic1.py
```
***

## 在线 Web 版本

### 敬请期待



