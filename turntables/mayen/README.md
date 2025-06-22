# Turntable Mayen

"NEMA 17" stepper motor with 200 steps driving the regular wormwheel-mechanism of a Roco turntable.

`TMC2130` chip driving it.

"Wormwheel" means I can switch off the driver chip to prevent it from getting warm. It should be able to survive running all the time and apparently 80 degrees Celcius is fine, but I don't like to have something that hot next to a flamable wooden structure.

My turntable doesn't actually need to turn more than 360 degrees. I like my locos pointing in the same direction continuously, that way moving the knob on my *multimaus* to the left always moves my loco to the left. At least for my steam locos it is consistent with practice on the Koblenz-Mayen railway, where locos drove with their exhaust in the direction of Mayen. I've extended that practice to my Mayen-Kelberg.

There's a keypad driving the turntable. "A" selects the five "A" tracks, typing a number moves the turntable to that track. Same with B and C.

Initially, the turntable moves a bit away from the zero point (which is track C-10, or C-0 as the keypad has only one digit). Then it moves towards C-0. That track, and only that track, has the regular Roco pin between the tracks that makes contact with the turntable bridge. I use that for detecting the zero position.

All the other positions are defined as `POS_B2` relative to the zero position.

There is a bit of play in the wormwheel mechanism. When moving away from the zero position, the position works fine as configured in the `.ino` file. But when moving back towards the zero position, my code first "overshoots" the target position and then moves backwards a bit, to make sure the play in the mechanism is consistent.
