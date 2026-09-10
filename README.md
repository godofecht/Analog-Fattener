# Analog-Fattener

A saturation and levelling effect in JUCE. Four controls, in the order the
signal meets them.

| Control | What it does |
|---|---|
| Crank | Input gain into the shaper. |
| Color | Amount of the sine waveshaper mixed against the cranked signal. |
| Boost | Makeup gain applied in the dB domain, before limiting. |
| Limit | The ceiling the soft knee cannot exceed. |

The shaping is a clamped sine transfer curve summed with the dry cranked signal,
so Color adds odd harmonics without replacing the original. Levelling happens in
decibels rather than on the sample: the signal is converted to dB, and anything
over the threshold is compressed through a quadratic knee before being clamped
to Limit.

```cpp
float CrankedSignal = outputData[sample] * crankValue;
float ColoredSignal = clamp(sin(clamp(CrankedSignal, -1, 1) * const1) / const2, -1, 1);
float SummedSignal  = CrankedSignal + ColoredSignal;
float dBL           = amp_dB * log(abs(SummedSignal)) + boostValue;

if (dBL > threshold_dB) {
    over_dB = dBL - threshold_dB;
    over_dB = a * over_dB + b * over_dB * over_dB;   // quadratic knee
    dBL = min(threshold_dB + over_dB, limitValue);
}
```

The editor draws its own rotary sliders through a custom `LookAndFeel`.

## Build

JUCE is a submodule.

```bash
git clone --recurse-submodules https://github.com/godofecht/Analog-Fattener.git
cmake -B build && cmake --build build
```

Or open `AnalogFattener.jucer` in the Projucer.
