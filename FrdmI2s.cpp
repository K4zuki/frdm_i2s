/** MIT LICENSE
Copyright 2017 K4zuki(github.com/K4zuki)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "FrdmI2s.h"
#ifdef DEBUG

static void donothing() {}

FrdmI2s::FrdmI2s(PinName MasterClk, PinName SerialData, PinName WordSelect, PinName BitClk,
                 int rxtx = FrdmI2s::TRANSMIT)
    : _i2s(), _mclk(1228000), _freq(32000), _bit(16), _stereo(0), _rxtx(rxtx) {
    i2s_init(&_i2s, MasterClk, WordSelect, BitClk, SerialData, rxtx);
}

void FrdmI2s::format(int sample = 32000, int bit = 16, int stereo = FrdmI2s::STEREO) {
    i2s_format(&_i2s, _rxtx, sample, bit, stereo);
}

void FrdmI2s::frequency(int wclk = 32000) {}

void FrdmI2s::stop() {
    _stat = STOP;
    _mute = MUTED;
}

void FrdmI2s::start() {
    _stat = RUN;
    _mute = UNMUTED;
}

void FrdmI2s::attach(Callback<void()> func, IrqType type) {
    lock();
    // Disable interrupts when attaching interrupt handler
    core_util_critical_section_enter();
    if (func) {
        _irq[type] = func;
        i2s_irq_set(&_i2s, (SaiIrq)type, 1);
    } else {
        _irq[type] = donothing;
        i2s_irq_set(&_i2s, (SaiIrq)type, 0);
    }
    core_util_critical_section_exit();
    unlock();
}

void FrdmI2s::write(char buf[], int len) {}
void FrdmI2s::write(int buf[], int len) {}
int FrdmI2s::read() {}
void FrdmI2s::read(char buf[], int len) {}
void FrdmI2s::read(int buf[], int len) {}
int FrdmI2s::get_fifo_limit() {}
void FrdmI2s::power(bool pwr) {}
void FrdmI2s::role(int mastermode = MASTER) {}
void FrdmI2s::wordsize(int words = 16) {}
void FrdmI2s::mclk_freq(int mclk = 12288000) {}
void FrdmI2s::set_interrupt_fifo_level(int level) {}
int FrdmI2s::fifo_level() {}
int FrdmI2s::fifo_points() {}
void FrdmI2s::stereomono(int stereomode = STEREO) {}
void FrdmI2s::mute() {}
void FrdmI2s::mute(int mute_en = MUTED) {}

void FrdmI2s::lock() {
    // Stub
}

void FrdmI2s::unlock() {
    // Stub
}

#endif
