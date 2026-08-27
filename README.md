# qemu-vault
An unofficial QEMU tool for saving and launching VMs by name.
Lets you save your QEMU commands without having to re-enter full command.
Made for people who prefer a minimal command-line interface.
Build with UNIX philosophy in mind.

## Features
- Add or remove QEMU virtual machines
- List saved virtual machines
- Launch virtual machines
- Store configurations
- Super Lightweight

## Requirements
- UNIX-like OS
- QEMU
- C Compiler (For building from source only)

## Installation
Pre-built '.deb' Debian package can be found in Releases.
Command 'make' is available for non-Debian users. 
  
## Usage
Commands:
- qemu-vault 'vm'                                      Run a saved VM
- qemu-vault --help    | -h                            Show help commands
- qemu-vault --list    | -l                            Show VM list
- qemu-vault --add     | -a <name> <qemu parameters>   Save a VM
- qemu-vault --version | -v                            Show the version
- qemu-vault --remove  | -r <name>                     Remove a VM

## License
qemu-vault is licensed under the MIT License, see LICENSE for details.
this software is distant from the official QEMU project, and is not affiliated with it in any way.
