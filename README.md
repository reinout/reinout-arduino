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

I'm building this all on OSX. What I used to get it all running is mostly from the
[installation instructions of the
bare-arduino-project](https://github.com/ladislas/Bare-Arduino-Project/blob/master/INSTALL.md):

    $ brew tap sudar/arduino-mk
    $ brew install arduino-mk

At the end, the brew install says:

    To use the Arduino-Makefile, please add the following into your per-project Makefile

    include /opt/homebrew/opt/arduino-mk/Arduino.mk


## New install

Regular venv/pip:

    $ python3 -m venv venv
    $ venv/bin/pip install -r requirements.txt




## Old info




The also-needed pyserial is a dependency in my ``setup.py``, so that's
installed automatically by buildout.

By buildout? Yes, I'm using buildout instead of virtualenv/pip so that I can
use some of the automation I'm used to (like automatic sphinx documentation
setup). Nobody else but me will need that part, so just make sure you're
pip-installing the current directory as all my dependencies are neatly in the
``setup.py``, of course :-)
