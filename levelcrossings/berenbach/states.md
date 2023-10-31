# State diagram 'Berenbach' level crossing

The two IR detectors can be reasonably quick to detect the train entering and
somewhat slower to detect the train leaving to give some margin of safety. And
ideally a waiting period before it "arms" again.

```mermaid
stateDiagram-v2
    m1: Active direction Mayen
    m2: Occupied direction Mayen
	m3: Off, awaiting release by detector B
    Off --> m1: button pressed in station building
    m1 --> m2: Detector A becomes active
	m2 --> m3: Detector A becomes inactive
	m3 --> Off: Detector B becomes inactive

```