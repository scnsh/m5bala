# m5bala
m5bala sample created on platform io.

# setup

## package install
``` bash
$ sudo apt install mosquitto
$ sudo apt install mosquitto-clients
```

# usage

```
$ mosquitto_pub -h localhost -t m5stack/control -m w # 前進
$ mosquitto_pub -h localhost -t m5stack/control -m s # 後退
$ mosquitto_pub -h localhost -t m5stack/control -m d # 右旋回
$ mosquitto_pub -h localhost -t m5stack/control -m a # 左旋回

# wsda 以外の入力で停止
$ mosquitto_pub -h localhost -t m5stack/control -m f # 停止
```

# referelce
m5bala
- porting from
    - https://github.com/m5stack/M5Bala
- applied PR
    - https://github.com/m5stack/M5Bala/pull/6

mqtt
- refered from
    - https://github.com/tsukasaJapan9/m5stack/tree/master/TestRobotMQTT