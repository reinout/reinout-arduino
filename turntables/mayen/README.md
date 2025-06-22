# Draaischijf Mayen

TODO: fix description

"NEMA 17" Stappenmotor met 200 stappen.

`TMC2208` aansturing, dus die kan 1/2 tot 1/16 stap aan en is lekker geluidsarm.

Aandrijving met wormwiel en 50 tands tandwiel. Dan kan ik de motor uitzetten zonder dat de draaischijf kan bewegen.

Ik ga uit van een halve minuut voor een halve cirkel draaien. 25 tandjes van het tandwiel, dus 25 rondjes voor de motor. Max 1000 stappen per seconde haalt de aansturing. Met 200 stappen is dat max 1/5 vertraging => 1/4 microstepping.

200 * 4 = 800, 800 * 30 = 24000

310mm doorsnee => 973.4 omtrek. Per stap 0.04 mm verplaatsing. 25 stappen per mm verplaatsing.

## Aansturing

Eerst teruglopen tot eindschakelaar aangetikt wordt. Motor gelijk stoppen. Nulpunt instellen.

Daarna reageren op de toetsenbordmatrix. En met wat vertraging richting het gekozen punt draaien. Zodra je op de locatie bent: motoraansturing uit om te zorgen dat de aansturing niet te warm wordt.
