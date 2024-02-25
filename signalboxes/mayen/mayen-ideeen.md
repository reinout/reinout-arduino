# Ideeen voor de hendels in Mayen


## Input

- 4x Fahrstrassenhebel (die grote). 4x2

- 6x Signalhebel (2x voor het ene inrijsein, 4x voor de uitrijseinen).

- 5x Weichenhebel.

Dat zijn er 19. Hier ook 15 rode leds bij.

**34 pins**.


## Output

- 2 draden richting inrijsein.

- 5 draden richting wissels via relais (voorlopig de conrad versie houden?).

- 4 draden richting uitrijseinen (TODO: hoe die aan te sturen?)

Nakijken hoe het zit met de polarisatie: volgens mij vanuit de wisselaandrijving geregeld, behalve de voorste. Misschien daar een extra relais voor nemen? Of die ook in eerste instantie zo laten? Ja.

Zowieso even nakijken hoe de wissels nu aan de schakelaar hangen. Enkelpolig? Hoe?

**11 pins**

## Uitrijseinen

Aandrijven met een servo, lijkt me. Losse arduino die het doet? Of zo'n servo-aandrijf-module rechtstreeks aan de mega hangen? Dat lijkt me de lekkerste combi. Maar twee draadjes (I2C) nodig.

Hm, dan kan ik ook servo's voor de wissels gebruiken...

https://learn.adafruit.com/16-channel-pwm-servo-driver

Dus.. maar 2 pins.

Als ik de wissels met servo's doe heb ik de relais daar niet voor nodig. Maar wel automatisch voor de puntstukpolarisatie.

Dan wordt de aansturing verder 2 inrijsein, 5 voor wisselrelais, 2 voor uitrijseinen, totaal 9.

## Display

- 4x2 + 1x2 (inrijsein simpel houden) leds om de seinen weer te geven. = 10
- Per wissel 2 leds: 5x2=10.
- 6x2 om met geel de sporen aan te geven. = 12
- Zie schets, 10x aansluiting voor groene leds. = 10

10 + 10 + 12 + 10 = 42......

Mega heeft 54 digitale pins, oepsie. Oh, maar de 16 analoge pins zijn als digitale bruikbaar. 70 totaal.


## Best veel: simpeler versie

- Voor de zes sporen *een* gele led, zoals nu. (Nou ja, twee meer, voor de opstelsporen).
- Vijf groene leds voor de seinen. Geen rode.

Dan wordt het:

- 34 pins voor schakelaars en rode leds.
- 9 richting seinen en wissels.
- 11 voor display.

54 totaal. Precies wat er aan digitale pins is :-)


## Relais om de servo's pas na initialisatie van stroom te voorzien

http://moba.noethlich.info/?page_id=333
