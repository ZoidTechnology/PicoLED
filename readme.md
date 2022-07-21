# PicoLED
Control up to 16 channels of addressable LEDs connected to a Raspberry Pi Pico over USB.
## Building
1. PicoLED uses the [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk), therefore all requirements for that must be installed. The SDK itself is included as a Git submodule so is not required.
2. Clone this repository and ensure that the SDK submodule has been initialised. The Pico SDK also includes submodules which must be initialised.
3. Create a directory called "build" within the root directory of this project. From within the build directory run `cmake ..` then `make`.
## Usage
The PicoLED firmware interfaces with a computer over a USB CDC virtual serial port. This port expects a two byte header followed by up to 4095 bytes of data. The first 4 bits of the header encode the output channel. The remaining 12 bits encode the number of data bytes which follow in big endian format. Once all data has been processed, the firmware will respond with a single byte containing the channel number. Once the response is received another header can be sent. Packets will only be processed while the DTR signal is deasserted. When DTR is asserted the firmware's internal state is reset. This ensures the firmware starts from a consistent state as most software will assert DTR when a port is opened.