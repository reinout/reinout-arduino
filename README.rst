Reinout's arduino build/management tools for his model railroad
===============================================================

I'm planning to use arduino nanos to steer signals and switches and led lights
on my model railway. I don't want to use the arduino IDE, instead I want a
proper makefile and my own regular preferred editor (emacs in my case). And I
want it all to have a proper changelog and proper documentation.

So I've made this git repo to get some serious over-engineering done. I'm used
to python and buildout, so that's what I'm going to be using. This isn't a
project that can be re-used and cloned directly.

What I **am going to be making reusable** are the various arduino libraries
I'll be making. They'll have some hardcoded paths in their makefiles to make
it easy for me to use/test them, but the c++ code itself is intended to be
reusable, in any case. And those'll be in separate repositories to make
cloning/sharing/using them easier.


Scratchpad for comments
-----------------------

Mention buildout setup.
