#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"
#include <stdio.h>
#include <string.h>

#define DISPLAY_ADDR 0x3C

using namespace daisy;

class SSD130X {
    private:
        daisy::OledDisplay<daisy::SSD130xI2c128x64Driver> display;

    public:
        SSD130X() {};
        void InitDisplay();
        void ClearDisplay();
        void WriteText(int pos, const char *text);
        void SelectText(int pos);
        void UpdateDisplay();
        void DrawPoint(uint8_t x, uint8_t y);
};
