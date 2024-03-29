Changelog of reinout-arduino
===================================================

0.6 (unreleased)
----------------

- Started mostly anew now that arduino-cli made our lives much easier!


0.5 (somewhere in 2016)
-----------------------

- Started with 'Kelberg' signalbox (arduino mega) project.


0.4 (2016-03-16)
----------------

- Added Kelberg 'A' signal which uses my new 'pulsemover' library to steer
  Viessmann slow-moving signals.


0.3 (2015-12-02)
----------------

- Using my own 'signalmover' library that works together with the 'servomover'
  one.

- Added ``signals/`` directory for storing ``Makefile`` + ``*.ino`` for the
  individual signals.


0.2 (2015-11-17)
----------------

- Using my own new servo library ('servomover').

- Added quick python script for printing the serial communication from the
  arduino.

- Added Bounce2 library and got the first hp0/hp1/hp2 signal working including
  buttons. Hurray!


0.1 (2015-11-01)
----------------

- Started documentation in the README.

- Added the arduino makefile project to mr.developer.

- Added small example project and first cut at a base makefile.

- Initial project structure created with nensskel 1.37.dev0.
