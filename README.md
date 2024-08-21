# Unreal Pokémon
*Created by Retro & Chill*

[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=retroandchill_unreal-pokemon&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=retroandchill_unreal-pokemon) [![Coverage](https://sonarcloud.io/api/project_badges/measure?project=retroandchill_unreal-pokemon&metric=coverage)](https://sonarcloud.io/summary/new_code?id=retroandchill_unreal-pokemon)

This project is a starter kit for developing a Pokémon game in Unreal Engine 5. The intent of the project is to make the development of games like this as seamless as possible. 

**Note:** This project is still in active development, so the feature set is limited, and the stability of the game/editor is still a concern. If you encounter any bugs/crashes when using this kit, or there are certain features that you would like me to add/prioritize, feel free to [submit an issue](https://github.com/retroandchill/unreal-pokemon/issues/new/choose).

## Getting Started
Getting started with this kit will require a few things to use properly. This section outlines each of the requirements.

### The source code
It is recommended that you grab the latest version from the [releases page](https://github.com/retroandchill/unreal-pokemon/releases) to download the code, but if you want the most up-to-date changes as the project is being developed and are willing to put up with the stability issues that will incur, you can also check out the repo using the following commands using CMD/PowerShell inside your desired directory.

```sh
git clone https://github.com/retroandchill/unreal-pokemon.git --recursive
```

### Unreal Engine 5.4 (or above)
This kit is currently being developed for Unreal Engine, and will not build on 5.3 or below. To use this kit you will need to either [download Unreal Engine](https://www.unrealengine.com/en-US/download) using the Epic Games Launcher, or [build the engine from source](https://www.unrealengine.com/en-US/ue-on-github) by requesting access on GitHub. For the later, however, I will not add any support for modified versions of the Engine, so use that at your own risk.

### Visual Studio 2022
While this kit is designed to not require the user to write any C++ code, you still need to have MSVC installed to compile the project. You can download the latest version of Visual Studio 2022 Community from [here](https://visualstudio.microsoft.com/downloads/).

### Marketplace Content
This kit is designed to be 100% free to use, and does not require the installation of any paid plugins, and all other third-party dependencies are contained within this repo already. However, there are some plugins that are free on the UE Marketplace that are being used that you will need to download and install into the engine in order to use. 
There are as follows
- [Paper ZD](https://www.unrealengine.com/marketplace/en-US/product/paperzd)

## Building for the first time
After downloading and installing the above requirements you should be able to open the project from the Epic Games Launcher/Unreal Engine 5. It will ask you if you would like to rebuild the modules. Click yes, and after it finishes compiling that (and likely the shaders as well :joy:) you should be able to use the project. Enjoy!
