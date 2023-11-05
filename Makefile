# Define your variables
OBJDIR = ./obj
BINDIR = ./bin
NAME = $(BINDIR)/a.exe

# Libraries
LIBS = ./lib/*

# Define the source files you want to compile
SRCS = $(wildcard src/*.cpp)

# Create object file names from source files
OBJS = $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Defines
DEFS = -DGLEW_STATIC

# Set your compiler and compiler flags
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror --debug -I./include -I./src $(DEFS)

# Your build target
all: $(NAME)

# Linking the final executable
$(NAME): $(OBJS) $(BINDIR)
	$(CXX) -o $@ $(OBJS) $(LIBS) -lGL

# Compiling individual source files into object files
$(OBJDIR)/%.o: src/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Create the OBJDIR if it doesn't exist
$(OBJDIR):
	mkdir -p $@

# Create the BINDIR if it doesn't exist
$(BINDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(BINDIR)
