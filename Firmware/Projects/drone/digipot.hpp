#pragma once
#include "daisy_seed.h"

namespace digipot {
    /// @brief Daisy Seed peripheral used to communicate with the digipots.
    constexpr daisy::I2CHandle::Config::Peripheral I2C_PERIPHERAL
        = daisy::I2CHandle::Config::Peripheral::I2C_1;

    /// @brief Device address for the I2C protocol.
    ///
    /// The device uses internal addresses for each wiper, which are sent separately
    /// and are not related to this one.
    constexpr uint8_t I2C_ADDRESS = 0x0101100;

    /// @brief Timeout for I2C operations, in milliseconds.
    constexpr int I2C_TIMEOUT = 10;

    /// @brief Wiper selection codes.
    enum class Wiper {
        Wiper0 = 0x0,
        Wiper1 = 0x1,
        Wiper2 = 0x6,
        Wiper3 = 0x7
    };

    // Terminal control registers (TCONx, x=0,1)
    // TODO: do.
    enum class TerminalControl {
    };

    /// @brief Initializes the I2C peripheral for the digital potentiometer.
    /// @param i2c Daisy I2C handle to initialize.
    /// @return Returns the result of the initialization.
    daisy::I2CHandle::Result init(daisy::I2CHandle &i2c);

    /// @brief  Sets the potentiometer value using I2C communication.
    /// @param i2c Daisy I2C handle.
    /// @param wiper Selected wiper.
    /// @param value Value to set the potentiometer to (0-256, 9 bits total).
    /// @return Returns the result of the I2C transmission.
    I2CHandle::Result set_value(daisy::I2CHandle &i2c, Wiper wiper, uint16_t value);

}