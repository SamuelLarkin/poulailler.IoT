```
docker pull homeassistant/raspberrypi4-homeassistant:0.105.2
``

```
docker \
  run \
    --init \
    --detach \
    --name="home-assistant" \
    --env "TZ=America/New_York" \
    --volume /home/pi/git/hass:/config \
    --net=host \
    homeassistant/raspberrypi4-homeassistant:0.105.2
```
