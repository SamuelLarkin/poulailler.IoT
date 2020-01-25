# Mosquitto

* https://randomnerdtutorials.com/esp8266-and-node-red-with-mqtt/
* http://www.steves-internet-guide.com/mosquitto_pub-sub-clients/
* http://www.steves-internet-guide.com/mqtt/

`mosquitto -v`
```
1579231786: mosquitto version 1.5.7 starting
1579231786: Using default config.
1579231786: Opening ipv4 listen socket on port 1883.
1579231786: Error: Address already in use
```

```
mosquitto_sub -t '#' -v
poulailler/temperature 18.20
poulailler/humidite 24.10
```

```
mosquitto_sub -t '#' -v -d
Client mosqsub|6076-raspberryp sending CONNECT
Client mosqsub|6076-raspberryp received CONNACK (0)
Client mosqsub|6076-raspberryp sending SUBSCRIBE (Mid: 1, Topic: #, QoS: 0)
Client mosqsub|6076-raspberryp received SUBACK
Subscribed (mid: 1): 0
Client mosqsub|6076-raspberryp received PUBLISH (d0, q0, r0, m0, 'poulailler/temperature', ... (5 bytes))
poulailler/temperature 18.50
Client mosqsub|6076-raspberryp received PUBLISH (d0, q0, r0, m0, 'poulailler/humidite', ... (5 bytes))
poulailler/humidite 24.70
Client mosqsub|6076-raspberryp received PUBLISH (d0, q0, r0, m0, 'poulailler/temperature', ... (5 bytes))
poulailler/temperature 18.50
Client mosqsub|6076-raspberryp received PUBLISH (d0, q0, r0, m0, 'poulailler/humidite', ... (5 bytes))
poulailler/humidite 24.90
```
