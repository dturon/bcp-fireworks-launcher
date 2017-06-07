<a href="https://www.bigclown.com"><img src="https://s3.eu-central-1.amazonaws.com/bigclown/gh-readme-logo.png" alt="BigClown Logo" align="right"></a>

# Project Fireworks Launcher

[![Travis](https://img.shields.io/travis/bigclownlabs/bcp-fireworks-launcher/master.svg)](https://travis-ci.org/bigclownlabs/bcp-fireworks-launcher)
[![Release](https://img.shields.io/github/release/bigclownlabs/bcp-fireworks-launcher.svg)](https://github.com/bigclownlabs/bcp-fireworks-launcher/releases)
[![License](https://img.shields.io/github/license/bigclownlabs/bcp-fireworks-launcher.svg)](https://github.com/bigclownlabs/bcp-fireworks-launcher/blob/master/LICENSE)
[![Twitter](https://img.shields.io/twitter/follow/BigClownLabs.svg?style=social&label=Follow)](https://twitter.com/BigClownLabs)


This repository contains firmware for [Fireworks Launcher Set](https://shop.bigclown.com/products/fireworks-launcher-set).

[![video](https://img.youtube.com/vi/kwgfSJDdBJE/0.jpg)](https://www.youtube.com/watch?v=kwgfSJDdBJE)

## Firmware Programming
```
dfu-util -s 0x08000000:leave -d 0483:df11 -a 0 -D firmware-base.bin
```
```
dfu-util -s 0x08000000:leave -d 0483:df11 -a 0 -D firmware-remote.bin
```

More information about dfu [here](https://doc.bigclown.com/core-module-flashing.html)

## MQTT

### Relay on Power module
* On
    ```
    mosquitto_pub -t 'node/fireworks-launcher-remote/relay/-/state/set' -m true
    ```
* Off
    ```
    mosquitto_pub -t 'node/fireworks-launcher-remote/relay/-/state/set' -m false
    ```
### Relay module
* On
    ```
    mosquitto_pub -t 'node/fireworks-launcher-remote/relay/0:0/state/set' -m true
    mosquitto_pub -t 'node/fireworks-launcher-remote/relay/0:1/state/set' -m true
    ```
* Off
    ```
    mosquitto_pub -t 'node/fireworks-launcher-remote/relay/0:0/state/set' -m false
    mosquitto_pub -t 'node/fireworks-launcher-remote/relay/0:1/state/set' -m false
    ```

## Build

* Clone repository

    ```
    git clone --recursive https://github.com/bigclownlabs/bcp-fireworks-launcher
    ```

* Move to folder
    ```
    cd bcp-fireworks-launcher
    ```

* Make
    all with debug flags
    ```
    make all
    ```
    or all optimize without debug
    ```
    make all-release
    ```
    or independently with debug flags
    ```
    make base
    make remote
    ```
    or independently optimize without debug
    ```
    make base-release
    make remote-release
    ```

* Firmware flash
    ```
    make dfu-base
    make dfu-remote
    ```
    or
    ```
    make dfu-remote-release
    make dfu-base-release
    ```


## License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT/) - see the [LICENSE](LICENSE) file for details.

---

Made with ❤ by [BigClown Labs s.r.o.](https://www.bigclown.com) in Czech Republic.
