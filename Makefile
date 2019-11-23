# mainWindow make file

DIR_SRC  = ./src/
DIR_OBJ  = ./obj/
DIR_LIB1 = ./lib/
DIR_LIB2 = /usr/local/lib
DIR_INC1 = ./include
DIR_INC2 = /usr/local/include/

#source file
#源文件，自动找所有.c，并将目标定义为同名.o文件
SOURCE = $(wildcard $(DIR_SRC)*.c) 
OBJS   = $(patsubst %.c,$(DIR_OBJ)%.o,$(notdir ${SOURCE}))
  
#target you can change mainWindow to what you want
#目标文件名，输入任意你想要的执行文件名
TARGET = mainWindow
  
#compile and lib parameter
#编译参数
#CC     = gcc                            本地编译指令
CC      :=  gcc
LDFLAGS  = -L$(DIR_LIB1) -L$(DIR_LIB2)
#CFLAGS   =  -muclibc -I$(DIR_INC1) -I$(DIR_INC2) -Wall -Wstrict-prototypes -pipe 
#CXXFLAGS =  $(CFLAGS) -DHAVE_CONFIG_H
CXXFLAGS = $(LDFLAGS) -I$(DIR_INC1) -I$(DIR_INC2)
LIBS     =  -lmgplus -lminigui_ths -lpthread -lm -ldl -lpng -ljpeg -lfreetype -lmgncs -lmgplus -lmgeff -lmgncs4touch
            
$(TARGET):$(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)
${DIR_OBJ}%.o:${DIR_SRC}%.c
	$(CC) $(CXXFLAGS) -c  $< -o $@ 


.PHONY:clean
clean:
	rm mainWindow;cd ./obj;rm *.o 

