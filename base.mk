# USB = /dev/cu.usbserial-11110
# USB = /dev/cu.usbserial-110
# USB = /dev/cu.usbserial-210
USB = /dev/cu.usbserial-2110
BOARD = arduino:avr:uno

info:
	@echo "Possible commands: compile, upload, info, clean"
	arduino-cli board list

compile: .compilemarker

upload: compile
	arduino-cli upload -p ${USB} --fqbn ${BOARD}

monitor: upload
	arduino-cli monitor -p ${USB} --fqbn ${BOARD}


clean:
	rm -f .compilemarker

# test is just a dummy so that I can do easy "ctrl-c t" with my emacs setup to
# test/upload stuff....
test: upload


.compilemarker: *.ino $(wildcard *.h) $(wildcard*.cpp)
	# Assumption: there is only one .ino file per directory
	arduino-cli compile --fqbn ${BOARD} *.ino
	touch .compilemarker
