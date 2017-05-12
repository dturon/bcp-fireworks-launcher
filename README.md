# BigClown bcp-fireworks-launcher


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
    Or
    ```
    make dfu-remote-release
    make dfu-base-release
    ```
