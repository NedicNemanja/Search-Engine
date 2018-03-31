INCLUDE_DIR = ./include
SOURCE_DIR = ./source
OBJECT_DIR = ./object

CC = gcc
CFLAGS=-g -I$(INCLUDE_DIR) -lm
OUT = minisearch

_DEPS = Arguments.h AVL_tree.h Console.h ErrorCodes.h FileMapping.h Highlight.h List.h Post.h PostingList.h QuickSort.h ReallocDocument.h Score.h StringManipulation.h Trie.h Word.h
#pattern matching from  _DEPS to include directory
DEPS = $(patsubst %,$(INCLUDE_DIR)/%,$(_DEPS))

_OBJ = main.o Arguments.o AVL_tree.o Console.o FileMapping.o List.o Post.o PostingList.o QuickSort.o ReallocDocument.o Score.o StringManipulation.o Trie.o Word.o
#same pattern matching principe
OBJ = $(patsubst %,$(OBJECT_DIR)/%,$(_OBJ))

############goals#######################

#general rule for all object files
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#default goal
$(OUT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

count:
	wc $(SOURCE_DIR)/*.c $(DEPS)

clean:
	rm -f $(OBJECT_DIR)/*.o ./minisearch
