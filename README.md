# qemu-vault
An unofficial QEMU tool for saving and launching VMs by name.
Lets you manage your QEMU VMs without having to re-enter full command.
Made for people who prefer a minimal command-line interface.

## Features
- Add or remove QEMU virtual machines
- List saved virtual machines
- Launch virtual machines
- Store configurations

## Requirements
- Linux
- QEMU
- GCC (only needed if building from source)

## Installation
Pre-built '.deb' file can be found in Releases.
Distros other than Debian have to build from source for now.
  
## Usage
Commands:
  qemu-vault <vm>                                      Run a saved VM
  qemu-vault --help    | -h                            Show this screen
  qemu-vault --list    | -l                            Show VM list
  qemu-vault --add     | -a <name> <qemu parameters>   Save a VM
  qemu-vault --version | -v                            Show the version
  qemu-vault --remove  | -r <name>                     Remove a VM

## License
qemu-vault is licensed under the MIT License.
