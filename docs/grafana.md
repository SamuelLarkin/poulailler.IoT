# Grafana

http://192.168.X.Y:3000

* [Grafana - Home](https://grafana.com/)
* [Howto Install InfluxDB an Grafana on a Raspberry pi 3](http://blog.centurio.net/2018/10/28/howto-install-influxdb-and-grafana-on-a-raspberry-pi-3/)

```
wget https://dl.grafana.com/oss/release/grafana-rpi_6.5.3_armhf.deb
sudo dpkg -i grafana-rpi_6.5.3_armhf.deb
sudo systemctl list-unit-files
sudo systemctl start grafana-server
sudo systemctl unmask grafana-server.service
sudo systemctl enable grafana-server
```
