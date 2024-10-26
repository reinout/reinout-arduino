# Reinout's arduino build/management tools for his model railroad

I'm planning to use arduino nanos to steer signals and switches and led lights on my
model railway. I don't want to use the arduino IDE, instead I want a proper makefile and
my own regular preferred editor (emacs in my case). And I want it all to have a proper
changelog and proper documentation.

So I've made this git repo to get some serious over-engineering done. I'm used to python
and buildout, so that's what I'm going to be using. This isn't a project that can be
re-used and cloned directly.

What I **am going to be making reusable** are the various arduino libraries I'll be
making. They'll have some hardcoded paths in their makefiles to make it easy for me to
use/test them, but the c++ code itself is intended to be reusable, in any case. And
those'll be in separate repositories to make cloning/sharing/using them easier.


## OSX prerequisites

In 2024, life is much easier than ten years ago. The USB connector is supported out of
the box. No more strange driver from a chinese website. And.... there's `arduino-cli`!

So the prerequisites are installing the regular arduino IDE (2.something at the moment)
and brew-installing `arduino-cli`. Then mostly follow the getting started documentation
of https://arduino.github.io/arduino-cli/ (0.35 at the moment).

- Grab board name from `arduino-cli board listall`. In my case `arduino:avr:uno` for the
  uno.

- `arduino-cli board list` to grab the port

Install libraries:

    $ arduino-cli lib install Bounce2
    $ arduino-cli lib install Servo
    $ arduino-cli lib install AccelStepper
    $ arduino-cli lib install Keypad

And I made a symlink to my local checkout of https://github.com/reinout/servomover into
`~/Documents/Arduino/libraries/`.


## Makefile setup

A `base.mk` provides most of the needed compile/upload functionality.

A `Makefile` per dir just has to include it and perhaps add a `BOARD` variable.


## TODO

Serial log output.
