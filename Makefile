# Project: Projet1
# Makefile created by Dev-C++ 4.9.9.2

CPP  = g++
CC   = gcc
RES  = 
OBJ  = alg-fst.o api-fst.o be.o lcd.o main.o mcbc.o mod_cand.o mod_grap.o mod_hwd.o mod_list.o mod_msg.o mod_pwd.o mod_scre.o mod_scru.o mod_view.o mod_vote.o mod_wait.o dummy.o libarlbx.o libcrdrd.o mod_lg.o mod_card.o $(RES)
LINKOBJ  = alg-fst.o api-fst.o be.o lcd.o main.o mcbc.o mod_cand.o mod_grap.o mod_hwd.o mod_list.o mod_msg.o mod_pwd.o mod_scre.o mod_scru.o mod_view.o mod_vote.o mod_wait.o dummy.o libarlbx.o libcrdrd.o mod_lg.o mod_card.o $(RES)
LIBS =   
INCS = 
CXXINCS = 
BIN  = mav
CXXFLAGS = $(CXXINCS)  
CFLAGS = $(INCS) -m32
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before mav all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o "mav" /usr/lib/libXbgi.a -lX11 -lm -m32 $(LIBS)

alg-fst.o: alg-fst.c
	$(CC) -c alg-fst.c -o alg-fst.o $(CFLAGS)

api-fst.o: api-fst.c
	$(CC) -c api-fst.c -o api-fst.o $(CFLAGS)

be.o: be.c
	$(CC) -c be.c -o be.o $(CFLAGS)

encdec.o: encdec.c
	$(CC) -c encdec.c -o encdec.o $(CFLAGS)

enckeys.o: enckeys.c
	$(CC) -c enckeys.c -o enckeys.o $(CFLAGS)

encmac.o: encmac.c
	$(CC) -c encmac.c -o encmac.o $(CFLAGS)

hashcomp.o: hashcomp.c
	$(CC) -c hashcomp.c -o hashcomp.o $(CFLAGS)

lcd.o: lcd.c
	$(CC) -c lcd.c -o lcd.o $(CFLAGS)

maccomp.o: maccomp.c
	$(CC) -c maccomp.c -o maccomp.o $(CFLAGS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)

mcbc.o: mcbc.c
	$(CC) -c mcbc.c -o mcbc.o $(CFLAGS)

mod_cand.o: mod_cand.c
	$(CC) -c mod_cand.c -o mod_cand.o $(CFLAGS)

mod_grap.o: mod_grap.c
	$(CC) -c mod_grap.c -o mod_grap.o $(CFLAGS)

mod_hwd.o: mod_hwd.c
	$(CC) -c mod_hwd.c -o mod_hwd.o $(CFLAGS)

mod_list.o: mod_list.c
	$(CC) -c mod_list.c -o mod_list.o $(CFLAGS)

mod_msg.o: mod_msg.c
	$(CC) -c mod_msg.c -o mod_msg.o $(CFLAGS)

mod_pwd.o: mod_pwd.c
	$(CC) -c mod_pwd.c -o mod_pwd.o $(CFLAGS)

mod_scre.o: mod_scre.c
	$(CC) -c mod_scre.c -o mod_scre.o $(CFLAGS)

mod_scru.o: mod_scru.c
	$(CC) -c mod_scru.c -o mod_scru.o $(CFLAGS)

mod_view.o: mod_view.c
	$(CC) -c mod_view.c -o mod_view.o $(CFLAGS)

mod_vote.o: mod_vote.c
	$(CC) -c mod_vote.c -o mod_vote.o $(CFLAGS)

mod_wait.o: mod_wait.c
	$(CC) -c mod_wait.c -o mod_wait.o $(CFLAGS)

psswd.o: psswd.c
	$(CC) -c psswd.c -o psswd.o $(CFLAGS)

dummy.o: dummy.c
	$(CC) -c dummy.c -o dummy.o $(CFLAGS)

libarlbx.o: libarlbx.c
	$(CC) -c libarlbx.c -o libarlbx.o $(CFLAGS)

libcrdrd.o: libcrdrd.c
	$(CC) -c libcrdrd.c -o libcrdrd.o $(CFLAGS)

mod_lg.o: mod_lg.c
	$(CC) -c mod_lg.c -o mod_lg.o $(CFLAGS)

mod_card.o: mod_card.c
	$(CC) -c mod_card.c -o mod_card.o $(CFLAGS)

