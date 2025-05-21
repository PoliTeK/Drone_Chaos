#include "math/models.hpp"
#include "daisy_seed.h"
#include "daisysp.h"

template<class T>
class ChaosOsc
{
private:
    enum
    {
        x,
        y,
        z,
        w,
    };
    ChaoticModel<T> *_model;
    TimerHandle *_timer;
    uint16_t _startTick;
    uint16_t _waitTick;
    uint16_t _endTikc;
public:
    
    ChaosOsc(ChaoticModel<T> model, TimerHandle timer)
    {
        _model = &model;
        _timer = &timer;
    }
    ~ChaosOsc();
    void setState(T state)
    {
        _model->state = state;
    }
    float get(uint8_t i)
    {
        return _model->state[i];
    }
    void init()
    {
        _startTick = timer->getTick();   
    }
    void setFrequency(float freq)
    {
        _waitTick = (float)_timer->getFreq()/ freq;
    }
    void play()
    {
        _endTikc = _startTick + _waitTick;
        if (_endTikc > _startTick)
        {
              if(_timer->getTick() - > _endTikc)
        {
            _model->step(_model->state);
            _startTick = _timer->getTick();
        }
        }
        if(_endTikc < _startTick)
        {
            if (_timer->getTick() < _endTikc)
            {
                _model->step(_model->state);
                _startTick = _timer->getTick();
            }
        } 
    }
};
