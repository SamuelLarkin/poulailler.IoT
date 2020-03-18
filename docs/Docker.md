# Docker

[Installing Docker and Docker Compose on the Raspberry Pi in 5 Simple Steps](https://dev.to/rohansawant/installing-docker-and-docker-compose-on-the-raspberry-pi-in-5-simple-steps-3mgl)

```
sudo apt-get install -y libffi-dev libssl-dev
sudo apt-get install -y python python-pip
sudo apt-get remove python-configparser
curl -sSL https://get.docker.com | sh
sudo usermod -aG docker pi
sudo pip install docker-compose
docker volume create portainer_data
docker \
  run \
    --detach \
    --publish 8000:8000 \
    --publish 9000:9000 \
    --name=portainer \
    --restart=always \
    --volume /var/run/docker.sock:/var/run/docker.sock \
    --volume portainer_data:/data \
    portainer/portainer
```

# IMPORTANT NOTE

We MUST remove configparser in order to get 'docker-compose` to work

```
sudo apt-get remove python-configparser
```
