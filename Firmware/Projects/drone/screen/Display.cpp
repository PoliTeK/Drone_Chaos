#include "Display.h"

void SSD130X::InitDisplay(){
    SSD130xI2CTransport::Config display_transport_config;
    
    display_transport_config.i2c_config.pin_config.scl = seed::D11;
    display_transport_config.i2c_config.pin_config.sda = seed::D12;
    display_transport_config.i2c_config.speed = I2CHandle::Config::Speed::I2C_400KHZ;
    
    // Configure display
    daisy::OledDisplay<daisy::SSD130xI2c128x64Driver>::Config display_config;
    display_config.driver_config.transport_config = display_transport_config;
    
    display.Init(display_config);
    ClearDisplay();
}

void SSD130X::ClearDisplay(){
    display.Fill(false);
    display.DrawLine(63, 0, 63, 63, true);
    display.DrawLine(127, 0, 127, 63, true);
    display.DrawLine(63, 0, 127, 0, true);
    display.DrawLine(63, 63, 127, 63, true);
}

void SSD130X::WriteText(int pos, const char* text){
    display.SetCursor(2, 1+(pos*10));
    display.WriteString(text, Font_6x8, true);
}

void SSD130X::SelectText(int pos){
    display.DrawRect(1, pos*10, 60, pos*10+9, true, false);
}

// 0<x<63, 0<y<63
void SSD130X::DrawPoint(uint8_t x, uint8_t y){
    display.DrawPixel(63+x, y, true);
}

void SSD130X::UpdateDisplay(){
    display.Update();
}