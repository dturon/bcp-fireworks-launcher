<a href="https://www.bigclown.com"><img src="https://s3.eu-central-1.amazonaws.com/bigclown/gh-readme-logo.png" alt="BigClown Logo" align="right"></a>

# BigClown bcp-fireworks-launcher

[![Travis](https://img.shields.io/travis/bigclownlabs/bcp-fireworks-launcher/master.svg)](https://travis-ci.org/bigclownlabs/bcp-fireworks-launcher)
[![Release](https://img.shields.io/github/release/bigclownlabs/bcp-fireworks-launcher.svg)](https://github.com/bigclownlabs/bcp-fireworks-launcher/releases)
[![License](https://img.shields.io/github/license/bigclownlabs/bcp-fireworks-launcher.svg)](https://github.com/bigclownlabs/bcp-fireworks-launcher/blob/master/LICENSE)
[![Twitter](https://img.shields.io/twitter/follow/BigClownLabs.svg?style=social&label=Follow)](https://twitter.com/BigClownLabs)


This repository contains firmware for [Fireworks Launcher Set](https://shop.bigclown.com/products/fireworks-launcher-set).

## Build

* Clone repository

    ```
    clone --recursive https://github.com/bigclownlabs/bcp-fireworks-launcher
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
