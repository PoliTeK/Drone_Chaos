#include "mcp4452.hpp"

using namespace daisy;

namespace digipot::MCP4452 {
    // I2CHandle::Result init_digipot(daisy::I2CHandle &i2c, Wiper wiper, )

    uint8_t get_TCON_address(Wiper wiper) {
        switch (wiper)
        {
        case Wiper::Wiper0:
        case Wiper::Wiper1:
            return 0x4; // TCON0
        
        case Wiper::Wiper2:
        case Wiper::Wiper3:
            return 0xA; // TCON1

        default:
            return 0;
        }
    }

    I2CHandle::Result set_value(daisy::I2CHandle &i2c, Wiper wiper, uint16_t value) {
        uint8_t wiper_addr = static_cast<uint8_t>(wiper);

        uint8_t data[2];
        // Command: Write data. Four MSBs address the wiper.
        // Then two command bits (00 = write). Two LSBs are data bits D9:8. D9 is unused.
        data[0] = (wiper_addr << 4) | ((value >> 8) & 1); // A3:A0 0 0 D9:D8
        data[1] = value & 0xFF; // D7:0

        // Timeout in milliseconds
        return i2c.TransmitBlocking(I2C_ADDRESS, data, sizeof(data)/sizeof(*data), I2C_TIMEOUT_MS);
    }
}