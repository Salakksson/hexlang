CC 		= gcc
CFLAGS		= -Wall

DIR_SRC 	= ./src
DIR_BUILD	= ./bin

TARGET 		= hexlang

SRCS 		= $(wildcard $(DIR_SRC)/*.c)
OBJS 		= $(filter-out $(DIR_BUILD)/main.o, $(patsubst $(DIR_SRC)/%.c, $(DIR_BUILD)/%.o, $(SRCS)))

$(TARGET): $(OBJS) $(DIR_BUILD)/main.o
	$(CC) $(CFLAGS) $^ -o $@

$(DIR_BUILD)/%.o: $(DIR_SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(DIR_BUILD)/*.o $(DIR_BUILD)/$(TARGET)
