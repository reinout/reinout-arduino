BASE_DIR = $(HOME)/zelf/arduino
USB = /dev/cu.usbserial-11110
BOARD = arduino:avr:uno

info:
	@echo "Possible commands: compile, upload, info, clean"
	arduino-cli board list

compile: .compilemarker

upload: compile
	arduino-cli upload -p ${USB} --fqbn ${BOARD}

clean:
	rm -f .compilemarker



.compilemarker: *.ino
	# Assumption: there is only one .ino file per directory
	arduino-cli compile --fqbn ${BOARD} *.ino
	touch .compilemarker
