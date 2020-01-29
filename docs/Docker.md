# Docker

[Installing Docker and Docker Compose on the Raspberry Pi in 5 Simple Steps](https://dev.to/rohansawant/installing-docker-and-docker-compose-on-the-raspberry-pi-in-5-simple-steps-3mgl)

```
sudo apt-get install -y libffi-dev libssl-dev
sudo apt-get install -y python python-pip
sudo apt-get remove python-configparser
curl -sSL https://get.docker.com | sh
sudo usermod -aG docker pi
docker run hello-world
sudo pip install docker-compose
```
