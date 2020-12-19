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
OPTIMIZATION=-O2
#CFLAGS	= -Weverything -O0 -fsanitize=memory -fno-omit-frame-pointer -g -std=c11
CFLAGS	= $(OPTIMIZATION) -std=c11
LIBS	= -lm
#LDFLAGS	= -fsanitize=memory -fno-omit-frame-pointer -g 
#LDFLAGS = -Weverything $(OPTIMIZATION) -std=c11
LDFLAGS = $(OPTIMIZATION) -std=c11

INCS	= -Isrc/

dut_aes32: $(OBJS_AES32) examples/aes32/dut_aes32.c
	$(CC) $(LDFLAGS) $(INCS) -o dudect_aes32_$(OPTIMIZATION) examples/aes32/$@.c $(OBJS_AES32) $(LIBS)

dut_aesbitsliced: $(OBJS_AESBITSLICED) examples/aesbitsliced/dut_aesbitsliced.c
	$(CC) $(LDFLAGS) $(INCS) -o dudect_aesbitsliced_$(OPTIMIZATION) examples/aesbitsliced/$@.c $(OBJS_AESBITSLICED) $(LIBS)

dut_donna: $(OBJS_DONNA) examples/donna/dut_donna.c
	$(CC) $(LDFLAGS) $(INCS) -o dudect_donna_$(OPTIMIZATION) examples/donna/$@.c $(OBJS_DONNA) $(LIBS)

dut_donnabad: $(OBJS_DONNABAD) examples/donnabad/dut_donnabad.c
	$(CC) $(LDFLAGS) $(INCS) -o dudect_donnabad_$(OPTIMIZATION) examples/donnabad/$@.c $(OBJS_DONNABAD) $(LIBS)

dut_simple: examples/simple/example.c
	# higher compiler optimization levels can make this constant time
	# do not optimize
	$(CC) -O0 $(INCS) -o dudect_simple_O0 examples/simple/example.c $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f $(OBJS_AES32) $(OBJS_AESBITSLICED) $(OBJS_DONNA) $(OBJS_DONNABAD) dudect_* *.exe a.out

test: all
	python test.py
