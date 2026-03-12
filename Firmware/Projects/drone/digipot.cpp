#include "digipot.hpp"

using namespace daisy;

namespace digipot {

    I2CHandle::Result init(daisy::I2CHandle &i2c) {
        I2CHandle::Config i2c_config;

        // choose which Daisy peripheral to use
        i2c_config.periph = I2C_PERIPHERAL;
        // I2C speed
        i2c_config.speed = I2CHandle::Config::Speed::I2C_100KHZ;
        // I2C mode
        i2c_config.mode = I2CHandle::Config::Mode::I2C_MASTER;

        // configure the I2C pins - use pins corresponding to I2C1
        i2c_config.pin_config.scl = seed::D11;
        i2c_config.pin_config.sda = seed::D12;

        return i2c.Init(i2c_config);
    }

    // I2CHandle::Result init_port

    I2CHandle::Result set_value(daisy::I2CHandle &i2c, Wiper wiper, uint16_t value) {
        uint8_t wiper_addr = static_cast<uint8_t>(wiper);

        uint8_t data[2];
        // Command: Write data. Four MSBs address the wiper.
        // Then two command bits (00 = write). Two LSBs are data bits D9:8. D9 is unused.
        data[0] = (wiper_addr << 4) | ((value >> 8) & 1); // A3:A0 0 0 D9:D8
        data[1] = value & 0xFF; // D7:0

        // Timeout in milliseconds
        return i2c.TransmitBlocking(I2C_ADDRESS, data, sizeof(data)/sizeof(*data), I2C_TIMEOUT);
    }

    // I2CHandle::Result set_value_all(daisy::I2CHandle &i2c, uint16_t value);
}