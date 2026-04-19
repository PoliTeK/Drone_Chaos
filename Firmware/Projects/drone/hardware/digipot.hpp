#pragma once
#include "daisy_seed.h"

// 8-bit rheostat
// The 'A' terminal is unavailable for this device, which only provides
// terminals 'W' (wiper) and 'B' (zero-scale terminal).
// R_WB = R/N, where N is a natural number from 0x000 to 0x100

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

    /* MODEL-SPECIFIC CODE */

    /// @brief Wiper selection codes.
    enum class Wiper {
        Wiper0 = 0x0,
        Wiper1 = 0x1,
        Wiper2 = 0x6,
        Wiper3 = 0x7
    };

    // Terminal control registers (TCONx, x=0,1)
    enum class TCON {
        TCON0 = 0x4,
        TCON1 = 0xA,
        // ConnectB = 0,
        // ConnectWiper = 1,
        // Shutdown = 4,
    };

    /// @brief  Sets the potentiometer value using I2C communication.
    /// @param i2c Daisy I2C handle.
    /// @param wiper Selected wiper.
    /// @param value Value to set the potentiometer to (0-256, 9 bits total).
    /// @return Returns the result of the I2C transmission.
    daisy::I2CHandle::Result set_value(daisy::I2CHandle &i2c, Wiper wiper, uint16_t value);
}