all: dut_aes32 dut_aesbitsliced dut_donna dut_donnabad dut_simple

OBJS_AES32 = examples/aes32/rijndael-alg-fst.o
OBJS_DONNA = examples/donna/curve25519-donna.o
OBJS_DONNABAD = examples/donnabad/curve25519-donnabad.o
OBJS_AESBITSLICED = examples/aesbitsliced/afternm_aes128ctr.o \
examples/aesbitsliced/beforenm_aes128ctr.o \
examples/aesbitsliced/common_aes128ctr.o \
examples/aesbitsliced/consts_aes128ctr.o \
examples/aesbitsliced/int128_aes128ctr.o \
examples/aesbitsliced/stream_aes128ctr.o \
examples/aesbitsliced/xor_afternm_aes128ctr.o
# CC=clang
#CFLAGS	= -Weverything -fsanitize=memory -fno-omit-frame-pointer -g -std=c11
CFLAGS	= -std=c11
LIBS	= -lm
#LDFLAGS	= -fsanitize=memory -fno-omit-frame-pointer -g 
#LDFLAGS = -Weverything -std=c11
LDFLAGS = -std=c11

INCS	= -Isrc/

dut_aes32: $(OBJS_AES32) examples/aes32/dut_aes32.c
	$(CC) $(LDFLAGS) -O2 $(INCS) -o dudect_aes32_O2 examples/aes32/$@.c $(OBJS_AES32) $(LIBS)

dut_aesbitsliced: $(OBJS_AESBITSLICED) examples/aesbitsliced/dut_aesbitsliced.c
	$(CC) $(LDFLAGS) -O2 $(INCS) -o dudect_aesbitsliced_O2 examples/aesbitsliced/$@.c $(OBJS_AESBITSLICED) $(LIBS)

dut_donna: $(OBJS_DONNA) examples/donna/dut_donna.c
	$(CC) $(LDFLAGS) -O2 $(INCS) -o dudect_donna_O2 examples/donna/$@.c $(OBJS_DONNA) $(LIBS)

dut_donnabad: $(OBJS_DONNABAD) examples/donnabad/dut_donnabad.c
	$(CC) $(LDFLAGS) -O2 $(INCS) -o dudect_donnabad_O2 examples/donnabad/$@.c $(OBJS_DONNABAD) $(LIBS)

dut_simple: examples/simple/example.c
	# higher compiler optimization levels can make this constant time
	$(CC) $(LDFLAGS) -O0 $(INCS) -DMEASUREMENTS_PER_CHUNK=100000 -o dudect_simple_O0 examples/simple/example.c $(LIBS)
	$(CC) $(LDFLAGS) -O2 $(INCS) -DMEASUREMENTS_PER_CHUNK=100000 -o dudect_simple_O2 examples/simple/example.c $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -O2 $(INCS) -c $< -o $@

clean:
	rm -f $(OBJS_AES32) $(OBJS_AESBITSLICED) $(OBJS_DONNA) $(OBJS_DONNABAD) dudect_* *.exe a.out

test: all
	python test.py
