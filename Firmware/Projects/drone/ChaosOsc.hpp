#pragma once

#include "daisy_seed.h"
#include "daisysp.h"
#include "math/models.hpp"

namespace chaosdrone {

    template<class T>
    struct ChaosOsc {
    private:
        uint32_t _startTick;
        uint32_t _waitTick;
        uint32_t _endTick;

    public:
        math::ChaoticModel<T> model;
        T model_state;
        daisy::TimerHandle timer;
        
        ChaosOsc(math::ChaoticModel<T> model, daisy::TimerHandle timer)
            : model(model), timer(timer) {}

        void init()
        {
            _startTick = timer.GetTick();
        }

        void setFrequency(float freq)
        {
            _waitTick = (float) timer.GetFreq() / freq;
        }

        void play()
        {
            _endTick = _startTick + _waitTick;
            if (_endTick > _startTick)
            {
                if(timer.GetTick() > _endTick)
            {
                model.step(model.state);
                _startTick = timer.GetTick();
            }
            }
            if(_endTick < _startTick)
            {
                if (timer.GetTick() < _endTick)
                {
                    model.step(model.state);
                    _startTick = timer.GetTick();
                }
            } 
        }
    };

}