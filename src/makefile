CFLAGS = -Wall

ALLEGRO_LIBS = `pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags`

LIBS = -lallegro_dialog -lm

OBJECTS = main.o physics.o controls.o scores.o

all: play

play: $(OBJECTS)
	gcc $(OBJECTS) -o play $(LIBS) $(ALLEGRO_LIBS)

main.o: main.c
	gcc -c main.c $(CFLAGS)

controls.o: ./libs/controls.c ./libs/controls.h
	gcc -c ./libs/controls.c $(CFLAGS)

physics.o: ./libs/physics.c ./libs/physics.h
	gcc -c ./libs/physics.c $(CFLAGS)

scores.o: ./libs/scores.c ./libs/scores.h
	gcc -c ./libs/scores.c $(CFLAGS)

clean:
	-rm -f *~ *.o

purge:	clean
	-rm -f play