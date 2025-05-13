#include "daisy_seed.h"
#include "daisysp.h"

#include "math/models.hpp"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = in[0][i];
		out[1][i] = in[1][i];
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);

	math::vec3f test1{10.0f, -1.0f, 2.4f}, test2{0.1f, 2.0f, -0.7f};
	test1 + test2;

	math::Henon henz;
	auto x1 = henz.step({0.1, 0.3});

	while(1) {}
}
