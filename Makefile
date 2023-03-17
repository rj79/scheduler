SOURCES = $(shell find . \( -name '*.h' -o -name '*.cpp' \) | grep -v .pio)

all: test

clean:
	pio run -t clean

blink:
	pio run -e example_blink -t upload

two_tasks:
	pio run -e example_two_tasks -t upload -t monitor

test: $(SOURCES)
	pio test -e test_lolin_c3_mini

