SOURCES = $(shell find . \( -name '*.h' -o -name '*.cpp' \) | grep -v .pio)

all: test_arduino_uno

clean:
	pio run -t clean

example_blink_uno:
	pio run -e example_blink_uno -t upload

example_two_tasks_uno:
	pio run -e example_two_tasks_uno -t upload -t monitor

example_blink_lolin_d1:
	pio run -e example_blink_lolin_d1 -t upload

example_two_tasks_lolin_d1:
	pio run -e example_two_tasks_lolin_d1 -t upload -t monitor

example_blink_lolin_c3:
	pio run -e example_blink_lolin_c3_mini -t upload

example_two_tasks_lolin_c3:
	pio run -e example_two_tasks_lolin_c3_mini -t upload -t monitor

test_arduino_uno: $(SOURCES)
	pio test -e test_arduino_uno

test_lolin_d1: $(SOURCES)
	pio test -e test_lolin_d1

test_esp32_c3: $(SOURCES)
	pio test -e test_lolin_c3_mini

