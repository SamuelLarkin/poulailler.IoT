```bash
INFLUXDB_USERNAME=admin INFLUXDB_PASSWORD=admin docker-compose  up --detach
WARNING: Some networks were defined but are not used by any service: poulailler
Creating network "docker_default" with the default driver
Creating influxdb  ... done
Creating mosquitto ... done
Creating grafana   ... done
Creating nodered   ... done
```

```bash
pi@raspberrypi4:~/git/poulailler.IoT/docker $ INFLUXDB_USERNAME=admin INFLUXDB_PASSWORD=admin docker-compose  run influxdb  /init-influxdb.sh
```

Grafana default username and password: `admin`/`admin` then you will be asked to change your password.


```bash
docker volume rm docker_grafana-lib docker_grafana-log docker_influxdb-lib
```
