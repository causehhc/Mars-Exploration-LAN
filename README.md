# Mars-Exploration-LAN
火 星 探 索  
+ stm32部分
  + stm32：增量式PI控制器、OLED-I2C显示电机参数  
  + esp8266-wifi：透明传输、sta模式、开机自启  
  + 底盘驱动：直流编码电机、~~循迹模块~~
+ python部分
  + keyboard：捕获键盘事件
  + socket：创建TCP服务端  
  + threading：多线程
  + queue：线程间通信
+ 特别说明：
  + 上下板都需要接电池，不然瞬时功耗过大可能会使stm32复位...
+ 就这：

![avatar](PIC/demo.jpg) 
