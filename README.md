# Arm9LoaderHax for Nintendo 3DS

## Explanation

This is my personal implementation of the arm9loaderhax exploit, documented [here](http://3dbrew.org/wiki/3DS_System_Flaws) and also presented [in this conference](https://media.ccc.de/v/32c3-7240-console_hacking), which provides ARM9 code execution directly at the console boot, exploiting a vulnerability present in 9.6+ version of New3DS arm9loader.

It works on both New and OLD 3DS.

This exploit was found by **plutoo** and **yellows8**, i do not own the idea, just the implementation of it.

Screen_init was implemented by [**dark-samus** pull request](https://github.com/delebile/arm9loaderhax/pull/9) (thank you!).

## Usage

Once you installed the exploit, it will launch the **arm9loaderhax.bin** file from the root of your SD card directly as the console boots.

If the file is not found, the console will shut down.


## Installation

After the compilation you'll have three files in the **data_output** directory:

* arm9loaderhax.3dsx
* arm9loaderhax.bin
* arm9loaderhax.pack

The *.pack* file contains all the content that will be installed (in case of a full package, your console-unique data too), and has to be placed in the root of your SD card.

The *.bin* file is an indipendent payload that can be launched from Brahma2, CakeHax, Arm9LoaderHax itself (mainly for update the exploit), and so on.
It is the installeing software, once you find your way to launch it, just follow the instruction.

The *.3dsx* file is a pre-buildt Brahma2 3dsx that can be launched on consoles with firmware below 9.2 through the Homebrew Launcher.
It is a loader for the *.bin* file, wich is included in the 3dsx.

## Software Update

When some essential parts of the software will be released, you'll be able to update your setup with the installer by using  *.pack* files that i will provide in future releases.


## Setup
####**Required Files**
Some files are needed in order to make the setup compilable, be sure to put the following files in the **data_input** folder, you have to find them on your own:

| Name          | Description           | SHA-256  |
| ------------- |---------------| ------|
| **new3ds10.firm**| New3DS NATIVE_FIRM from system version 10.2.| d253c1cc0a5ffac6b383dac1827cfb3b2d3d566c6a1a8e5254e389c2950623e5 |
| **new3ds90.firm**|New3DS NATIVE_FIRM from system version 9.0.|d7be76e1813f398dcea85572d0c058f7954761a1d5ea03b5eb5047ac63ac5d6b |
|**secret_sector.bin**|The New 3DS secret 0x96 key sector.|    82f2730d2c2da3f30165f987fdccac5cbab24b4e5f65c981cd7be6f438e6d9d3 |
|**otp.bin**|A dump of your console OTP data from region 0x10012000-0x10012100. Using other console's OTP will result in a brick.|     |

####**Required Enviroment**

* devkitARM r45
* libctru (ver. 1.0.0)
* Python with PyCcrypto (2.7 or 3.x should work)
* GCC or MinGW (Only needed to compile the buildt-in tool, you can download [a pre-compiled windows build here](https://mega.nz/#!j0RkxLjb!4Am-3yDAR9g4VDxY93pWhXVYNDiylSW1cKJntOLfDWU), place it in **common** folder)

####**Compilation Modes**

* **make** : This will compile all the files
* **make hax** : This will skip the 3DSX installer compilation
* **make stage2_update** : This will generate an updater pack with only the secondary payload, mainly for updating the exploit files.

## Credits

*Copyright 2016, Jason Dellaluce*


*Licensed under GPLv2 or any later version, refer to the license.txt file included.*

* Smealum and contributors for libctru
* Normmatt for sdmmc.c and .h, and also for .ld files and the log from XDS by ichfly that provided us some of the information needed to get screen init
* Christophe Devine for the SHA codes
* Archshift for i2c.c and .h
* Megazig for crypto.c and .h
* Patois for original BRAHMA code
* Smealum, Derrek, Plutoo for publishing the exploit
* Yellows8 and Plutoo as ideators of it
* [3dbrew community](http://3dbrew.org/)
* bilis/b1l1s for his screen init code, and work on inegrating it into stage 2
* dark_samus for work on integrating screen init into stage 2
