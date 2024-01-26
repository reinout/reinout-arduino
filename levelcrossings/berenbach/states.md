# State diagram 'Berenbach' level crossing

The two IR detectors can be reasonably quick to detect the train entering and
somewhat slower to detect the train leaving to give some margin of safety. And
ideally a waiting period before it "arms" again.

- Detector A is just to the right of the level crossing.
- Detector B is near the Monreal tunnel.

```mermaid
stateDiagram-v2
    m1: Active direction Mayen
    m2: Occupied direction Mayen
    m3a: Occupied direction Mayen, B has armed
    m3b: Off, awaiting arming of B
	m3c: Off, awaiting release by detector B
    b1: Active direction Berenbach
    b2: Occupied direction Berenbach

    Off --> m1: button pressed in station building
    m1 --> m2: Detector A becomes active
	m2 --> m3a: Detector B becomes active
    m3a --> m3c: Detector A becomes inactive
	m2 --> m3b: Detector A becomes inactive
    m3b --> m3c: Detector B becomes active
	m3c --> Off: Detector B becomes inactive

	Off --> b1: Detector B becomes active
	b1 --> b2: Detector A becomes active
	b2 --> Off: Detector A becomes inactive

```

Pseudocode for the main loop:

```python
# Per state, look at which transition is possible (and ignore the rest)
if state == off:
    # Two actions are possible, "off --> b1"  and "off -> m1".
    if sensor_b == active:
        state = b1
    if button_in_station_building == pressed:
        state = m1

if state == b1:
    if sensor_a == active:
        state = b2
# etc etc etc

# Once the state is known, activate/deactivate the level crossing.
if state in [off, m3b, m3c]:
    level_crossing = blinking
else:
    level_crossing = quiet
```
