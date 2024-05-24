# Variables
DOXYGEN = doxygen
DOXYFILE = Doxyfile

# Default target
all: docs

# Target to generate documentation
docs:
	$(DOXYGEN) $(DOXYFILE)

# Clean documentation
clean:
	rm -rf docs