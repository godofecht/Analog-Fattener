# AnalogFattener: A JUCE Audio Plugin

Hey there, welcome to the AnalogFattener project. This is a simple audio plugin I'm building with the JUCE framework, designed to add a bit of analog warmth and saturation to your audio. It's a fun little project, and I'm excited to share it with you.

## The Goal

The main idea behind this plugin is to create a simple, intuitive tool for adding some analog character to audio signals. It's not meant to be a complex, multi-faceted processor, but rather a focused tool for adding a bit of grit and warmth.

## The UI

I've spent a good amount of time working on the UI to make it both aesthetically pleasing and easy to use. I've gone for a clean, modern look with a dark grey gradient background and a set of custom-drawn rotary knobs. The layout is designed to be intuitive, with the main "Crank" knob front and center, and the smaller "Color," "Boost," and "Limit" knobs arranged symmetrically below it.

Here's a quick overview of the controls:

*   **Crank:** This is the main control, and it's where the magic happens. It's a subtle effect, but it adds a nice bit of saturation and warmth to the signal.
*   **Color:** This knob lets you dial in the character of the saturation. You can go from a subtle, transparent warmth to a more pronounced, gritty sound.
*   **Boost:** This is a simple output gain control, so you can level-match the processed signal to the original.
*   **Limit:** This is a simple limiter to catch any peaks that might result from the saturation and boost.

## The Build Saga

Now, let's talk about the build process. I'm not going to lie, getting this thing to build on Linux was a journey. I ran into a whole host of issues with missing dependencies and incorrect build configurations, and it took a lot of trial and error to get it right. But hey, that's how we learn, right?

The good news is that I've finally got a solid, reproducible build system in place using CMake. I've also documented all the required dependencies, so you shouldn't have to go through the same pain I did.

### Dependencies

To build this project on a Debian-based Linux distribution (like Ubuntu), you'll need to install the following packages:

```bash
sudo apt-get install -y \
    libasound2-dev \
    libjack-jackd2-dev \
    ladspa-sdk \
    libcurl4-openssl-dev \
    libfreetype6-dev \
    libfontconfig1-dev \
    libx11-dev \
    libxcomposite-dev \
    libxcursor-dev \
    libxext-dev \
    libxinerama-dev \
    libxrandr-dev \
    libxrender-dev \
    libwebkit2gtk-4.1-dev \
    libglu1-mesa-dev \
    mesa-common-dev
```

### Building the Project

Once you've got all the dependencies installed, building the project is super simple. Just run the following commands from the root of the project directory:

```bash
# Initialize the JUCE submodule
git submodule update --init --recursive

# Configure and build the project
cmake -B build
cmake --build build
```

This will create a `build` directory and compile the project. You'll find the standalone executable in the `build/AnalogFattener_artefacts/Standalone` directory.

## What's Next?

Now that the build is sorted and the UI is in a good place, I'm excited to start working on the actual audio processing code. I've got a few ideas for how I want to implement the saturation and limiting, so stay tuned for more updates.

And hey, if you've got any feedback or suggestions, I'd love to hear them. This is a learning process for me, and I'm always open to new ideas.

Thanks for checking out the project!
