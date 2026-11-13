# Analog-Fattener

An analog fattener audio effect in C++, built on the JUCE framework.

## What this is

Analog-Fattener is an audio plugin: a small program that loads inside a DAW (digital audio workstation, such as Ableton Live, Logic Pro, FL Studio or Reaper) and changes the sound on a track. It adds saturation, the warm, slightly gritty thickening you get when you drive analog gear such as tape or valves a little too hard. A dry bass or vocal comes out sounding fuller and louder, without hard digital clipping.

The plugin window is black with four red knobs: one large knob, **Crank**, at the top, and three smaller ones, **Color**, **Boost** and **Limit**, along the bottom.

## What you can learn from it

Music Technology:

- What saturation does to a sound, and why it makes a track sound "fatter".
- Harmonics: the quieter, higher notes inside every sound that give it its tone. Saturation adds new ones.
- What a waveshaper is: a curve that maps each input sample to an output sample. Bending the curve bends the waveform, which adds harmonics.
- What a limiter does: it stops the level from going over a ceiling.
- Decibels (dB), the scale used for loudness, and why level processing is often done in dB.

Computer Science and Maths:

- How a plugin processes audio one sample at a time.
- Using sine, logarithm and exponential functions on real data.

## Run it

### On your own computer

**You will need:**

- [Git](https://git-scm.com/downloads), to download the code and JUCE together.
- [CMake](https://cmake.org/download/) 3.15 or later, a tool that sets up a C++ build for your computer.
- A C++ compiler:
  - Windows: [Visual Studio 2022 Community](https://visualstudio.microsoft.com/vs/community/) with the "Desktop development with C++" workload.
  - macOS: [Xcode](https://developer.apple.com/xcode/).
- A DAW that loads VST3 plugins (Windows and macOS) or AU plugins (macOS). [Reaper](https://www.reaper.fm/) has a free evaluation.

You do not need to download JUCE separately. It is included as a git submodule (a link to another repository, fetched along with this one). The submodule points to JUCE 8.0.12.

The CMake build in this repo does not compile yet. See "Project status". These are the intended steps:

1. Download the code and JUCE:

   ```bash
   git clone --recurse-submodules https://github.com/godofecht/Analog-Fattener.git
   cd Analog-Fattener
   ```

2. Configure and build:

   ```bash
   cmake -B build
   cmake --build build
   ```

3. The build is set to copy the plugin to your system's plugin folder (`COPY_PLUGIN_AFTER_BUILD TRUE`). Rescan plugins in your DAW and add **AnalogFattener** to a track.

The older route is to open `AnalogFattener.jucer` in the Projucer, the project tool that comes with [JUCE](https://juce.com/download/). It has a Visual Studio 2019 exporter and was saved with Projucer 5.4.5.

Controls:

| Knob | Range | Default | What you hear |
|---|---|---|---|
| **Crank** | 1 to 1.2 | 1 | How hard the signal is pushed into the shaper. |
| **Color** | 1 to 100 | 1 | How strongly the waveshaper bends the sound. Higher values add more harmonics. |
| **Boost** | 0 to 9 dB | 0 | Extra loudness, added before the limiter. |
| **Limit** | -9 to 0 dB | -3 | The ceiling the output cannot go over. |

There are no number boxes on the knobs, so listen as you turn them.

## How it works

Each sample goes through three steps.

1. Crank multiplies the sample to make it louder.
2. Color passes it through a sine-shaped curve and adds the result back to the cranked signal. This rounds off the peaks and adds harmonics.
3. The level is converted to decibels. Boost is added. Anything over a threshold is squeezed by a gentle curve, then held below the Limit ceiling. The level is then turned back from decibels into a sample.

| File or folder | What is in it |
|---|---|
| `Source/PluginProcessor.cpp` | `processBlock`, where every sample is shaped and limited. |
| `Source/PluginProcessor.h` | The values the knobs change (`crankValue`, `colorValue`, `boostValue`, `limitValue`) and the fixed constants of the curve. |
| `Source/PluginEditor.cpp` | Knob ranges, defaults, labels and layout. |
| `Source/PluginEditor.h` | `OtherLookAndFeel`, which draws the red knobs, and `sliderValueChanged`, which passes knob values to the processor. |
| `CMakeLists.txt` | The CMake build. |
| `AnalogFattener.jucer` | The older Projucer project. |
| `JUCE/` | The JUCE framework, as a git submodule. |

## Ideas to try

1. Give Crank more range. In `Source/PluginEditor.cpp`, change `crankSlider.setRange(1,1.2,0.01)` to `setRange(1,4,0.01)` and listen to the saturation get heavier.
2. Change the knob colours in `OtherLookAndFeel` in `Source/PluginEditor.h`.
3. Add a number box under each knob. In `Source/PluginEditor.cpp`, the sliders use `NoTextBox`. Try `Slider::TextBoxBelow`.
4. Swap the waveshaper. In `Source/PluginProcessor.cpp`, replace the `sin(...)` curve with `std::tanh` and compare the sound.
5. Ambitious: turn the four values into real plugin parameters with `juce::AudioProcessorValueTreeState`, so a DAW can automate them and save them with a project. [JUCE-Ladder-Filter-Implementation](https://github.com/godofecht/JUCE-Ladder-Filter-Implementation) shows one way to do it.

## Technical details

From the previous README.

A saturation and levelling effect in JUCE. Four controls, in the order the signal meets them.

| Control | What it does |
|---|---|
| Crank | Input gain into the shaper. |
| Color | Amount of the sine waveshaper mixed against the cranked signal. |
| Boost | Makeup gain applied in the dB domain, before limiting. |
| Limit | The ceiling the soft knee cannot exceed. |

The shaping is a clamped sine transfer curve summed with the dry cranked signal, so Color adds odd harmonics without replacing the original. Levelling happens in decibels rather than on the sample: the signal is converted to dB, and anything over the threshold is compressed through a quadratic knee before being clamped to Limit.

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

Further details from the code:

- `const1 = colorValue / 200 * π` and `const2 = sin(const1)`. Dividing by `const2` normalises the curve so an input of 1 still gives an output of 1.
- `amp_dB = 8.6562` converts natural log to decibels (close to 20 / ln 10). `baseline_threshold_dB = -5.0`, so the knee starts 5 dB below the Limit setting. The knee constants are `a = 1.017` and `b = -0.025`.
- The output is `exp(dBL / amp_dB)` with the sign of the original input sample.
- The CMake target uses `juce_add_plugin` with `COMPANY_NAME "MyCompany"`, `PLUGIN_CODE "AbCd"`, `INSTALL_VST3 TRUE` and `INSTALL_AU TRUE`.

## Project status

The DSP code dates from 2020. A CMake build and UI layout were added in January 2026. Known problems:

- The CMake build will not compile as committed. The source files include `../JuceLibraryCode/JuceHeader.h`, which is not in the repo, and `CMakeLists.txt` does not generate a JUCE header.
- The `.jucer` file asks for plugin formats that current JUCE no longer supports (RTAS) or that need extra SDKs (VST2, AAX).
- The knob values are plain variables. A DAW cannot automate them, and they are not saved with a project.
- The Limit knob shows -3 dB when the plugin opens, but the processor starts with 0 dB until you move the knob.
- There is no screenshot or audio example yet.

## Licence

You can use, copy and change the plugin source in `Source/` for any purpose, as long as you keep the copyright notice. See [LICENSE](LICENSE) (MIT).

JUCE in `JUCE/` keeps its own licence. See [THIRD-PARTY-NOTICES.md](THIRD-PARTY-NOTICES.md). A plugin you build and share is covered by whichever JUCE licence applies to you.

## Credits

- Written by Abhishek Shivakumar.
- Built with [JUCE](https://juce.com/).
