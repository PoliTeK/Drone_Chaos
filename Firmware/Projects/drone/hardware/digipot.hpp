#pragma once
#include "daisy_seed.h"

namespace digipot {
    // IMPORTANT: The SCL and SDA pins must match the peripheral!

    /// @brief Daisy Seed peripheral used to communicate with the digipots.
    constexpr daisy::I2CHandle::Config::Peripheral I2C_PERIPHERAL
        = daisy::I2CHandle::Config::Peripheral::I2C_1;

    constexpr daisy::Pin I2C_SCL = daisy::seed::D11;
    constexpr daisy::Pin I2C_SDA = daisy::seed::D12;

    /// @brief Device address for the I2C protocol.
    ///
    /// The device uses internal addresses for each wiper, which are sent separately
    /// and are not related to this one.
    constexpr uint8_t I2C_ADDRESS = 0b0101100;

    constexpr uint32_t I2C_TIMEOUT_MS = 10;

    /// @brief Initializes the I2C peripheral for the digital potentiometer.
    /// @param i2c Daisy I2C handle to initialize.
    /// @return Returns the result of the initialization.
    daisy::I2CHandle::Result init(daisy::I2CHandle &i2c);
}