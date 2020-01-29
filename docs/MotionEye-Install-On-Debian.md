# MotionEye
username/password \
admin/

## In Docker
[Install In Docker](https://github.com/ccrisan/motioneye/wiki/Install-In-Docker)
```
docker pull ccrisan/motioneye:master-armhf
docker run --name="motioneye" \
    -p 8765:8765 \
    --hostname="motioneye" \
    -v /etc/localtime:/etc/localtime:ro \
    -v /etc/motioneye:/etc/motioneye \
    -v /var/lib/motioneye:/var/lib/motioneye \
    --restart="always" \
    --detach=true \
    ccrisan/motioneye:master-armhf
```

### Before Proceeding
* Read the general [[Installation|Installation]] page first.
* These instructions apply to Debian and Debian-based distributions. For [[Ubuntu|Install-On-Ubuntu]] and [[Raspbian|Install-On-Raspbian]] however, you should check out their specific instructions.
* All commands require *root*; use `sudo` before each command or become root using `su`.

### Instructions
1. **Skip this step for Debian 9 or newer**. For older Debian versions, you'll need to add the following repo to your apt sources, replacing [name] with `wheezy` (Debian 7) or `jessie` (Debian 8). This is required for `ffmpeg`:

        echo "deb http://www.deb-multimedia.org [name] main non-free" >> /etc/apt/sources.list
        apt-get update
        apt-get install deb-multimedia-keyring
        apt-get update     # yes, again

2. Install `motion`, `ffmpeg` and `v4l-utils`:

        apt-get install motion ffmpeg v4l-utils

    **note**: Precompiled binaries of motion can be found [here](https://github.com/Motion-Project/motion/releases/), if you're not happy with what you find in the official Debian repos.

3. Install the dependencies from the repositories:

        apt-get install python-pip python-dev python-setuptools curl libssl-dev libcurl4-openssl-dev libjpeg-dev libz-dev

    **note**: Python 2.7 is required. If your system still runs Python 2.6 or older, please upgrade.

4. Install `motioneye`, which will automatically pull Python dependencies (`tornado`, `jinja2`, `pillow` and `pycurl`):

        pip install motioneye

    **note:** If `pillow` installation fails, you can try installing it from official repos using `apt-get install python-pillow`.

5. Prepare the configuration directory:

        mkdir -p /etc/motioneye
        cp /usr/local/share/motioneye/extra/motioneye.conf.sample /etc/motioneye/motioneye.conf

6. Prepare the media directory:

        mkdir -p /var/lib/motioneye

7. Add an init script, configure it to run at startup and start the `motionEye` server:

    * Debian 7, sysvinit-based:

            cp /usr/local/share/motioneye/extra/motioneye.init-debian /etc/init.d/motioneye
            chmod +x /etc/init.d/motioneye
            update-rc.d -f motioneye defaults
            /etc/init.d/motioneye start

    * Debian 8 and newer, systemd-based:

            cp /usr/local/share/motioneye/extra/motioneye.systemd-unit-local /etc/systemd/system/motioneye.service
            systemctl daemon-reload
            systemctl enable motioneye
            systemctl start motioneye

8. To upgrade to the newest version of motioneye, after it has been released, just issue:

            pip install motioneye --upgrade

       **note** that will update all the other required dependecies

    * Debian 7 or earlier:

            service motioneye restart

    * Debian 8 or later:

            systemctl restart motioneye


[source](https://github.com/ccrisan/motioneye/wiki/Install-On-Raspbian)
