CC = cc
OUT_FILE_NAME = libsgx_uprotected_cdr.a

CFLAGS= -fPIC -O0 -Wall -c #-DDEBUG

INC = -I.

OBJ_DIR=./obj

OUT_DIR=.

FILES = uprotected_cdr.c	
	

$(OUT_FILE_NAME): $(patsubst %.c,$(OBJ_DIR)/%.o,$(wildcard $(FILES))) 
	
	ar  rcs  $(OUT_DIR)/$@ $^



$(OBJ_DIR)/%.o: %.c dirmake
	$(CC) -c $(INC) $(CFLAGS) -lzmq -o $@  $< 
	
dirmake:
	@mkdir -p $(OUT_DIR)
	@mkdir -p $(OBJ_DIR)
	
clean:
	rm -f $(OBJ_DIR)/*.o $(OUT_DIR)/$(OUT_FILE_NAME)

rebuild: clean build
