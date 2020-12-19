# mosquitto-1.6.7-send_connect_status
二次开发mosquitto 1.6.7，新增了一个小特性，在客户端在线离线时，发送一个消息到特定的客户端、实时监听客户端在线离线状态.
## 1. 安装uthash 

git clone *https://github.com/troydhanson/uthash

cd uthash

mv ./include/* /usr/include

ldconfig

## 2.安装websocket

git clone *https://github.com/imanel/libwebsocket

mkdir build 

cd build

cmake ..

make

make install

ldconfig

## 3. 安装mosquitto

git clone *https://github.com/jeffreywang1988/mosquitto-1.6.7-send_connect_status

make clean

make 

make install

## 4. mosquitto 项目配置

vim /etc/mosquitto/mosquitto.conf

再配置文件末尾新增如下配置项

topic_notice_online client/online #设备上线发送json消息到topic

topic_notice_offline client/offline #设备离线发送消息到topic

topic_notice_prefix CLIENT_PREIX #处理客户端id为 CLIEN_PREFIX前缀的客户端

## 5. 配置系统系统命令

vim /etc/lib/systemd/system/mosquitto.service

```
[Unit]
Description=Mosquitto MQTT v3.1/v3.1.1 Broker
Documentation=man:mosquitto.conf(5) man:mosquitto(8)
After=network-online.target
Wants=network-online.target
 
[Service]
ExecStart=/usr/sbin/mosquitto -c /etc/mosquitto/mosquitto.conf
User=mosquitto
LimitNOFILE=65535 #重要，mosquitto可以打开的文件句柄数
[Install]
WantedBy=multi-user.target
```
systemctl enable mosquitto.service

systemctl start mosquitto.service

## 6. example 

订阅在线


mosquitto_sub -h xxx  -p 1883 -u xxx -P xxx -t client/online

订阅离线

mosquitto_sub -h xxx  -p 1883 -u xxx -P xxx -t client/ofline

消息体

```
{"clientid", "CLIENT_PREFIX:APP:XXX:timestamp", "type", "1", "time", "1608363225753"}

```

## 引用

参考项目 *https://github.com/eclipse/mosquitto/tree/1.6.x

参考项目 *https://github.com/houjixin/mosquitto-1.4.11-opt

## 联系方式

 yuzhoumeigui@qq.com
 
 https://www.cnblogs.com/yuzhoumeigui
 
