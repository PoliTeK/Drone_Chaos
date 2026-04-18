#include "digipot.hpp"
#include "mcp4452.hpp"

namespace digipot {
    using namespace daisy;

    I2CHandle::Result init(daisy::I2CHandle &i2c) {
        I2CHandle::Config i2c_config;

        // choose which Daisy peripheral to use
        i2c_config.periph = I2C_PERIPHERAL;
        i2c_config.speed = I2CHandle::Config::Speed::I2C_100KHZ;
        i2c_config.mode = I2CHandle::Config::Mode::I2C_MASTER;

        // configure the I2C pins
        i2c_config.pin_config.scl = I2C_SCL;
        i2c_config.pin_config.sda = I2C_SDA;

        return i2c.Init(i2c_config);
    }
}