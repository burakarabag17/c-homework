# Derleyici ve bayraklar
CC = gcc
CFLAGS = -g -Wall

# Hedef dosya ve kaynak dosyalar
TARGET = main
SRCS = main.c
OBJS = $(SRCS:.c=.o)

# Kural: hedef dosya
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Kural: .o dosyalarını oluşturma
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Temizlik kuralı
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: clean
