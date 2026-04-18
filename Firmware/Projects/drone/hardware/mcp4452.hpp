#pragma once
#include "digipot.hpp"

// 8-bit rheostat
// The 'A' terminal is unavailable for this device, which only provides
// terminals 'W' (wiper) and 'B' (zero-scale terminal).
// R_WB = R/N, where N is a natural number from 0x000 to 0x100
namespace digipot::MCP4452 {
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