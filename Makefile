all: dut_aes32 dut_aesbitsliced dut_cmpmemcmp dut_cmpct dut_donna dut_donnabad 

OBJS = src/cpucycles.o src/fixture.o src/random.o \
src/ttest.o src/percentile.o
OBJS_AES32 = dut/aes32/rijndael-alg-fst.o
OBJS_DONNA = dut/donna/curve25519-donna.o
OBJS_DONNABAD = dut/donnabad/curve25519-donnabad.o
OBJS_AESBITSLICED = dut/aesbitsliced/afternm_aes128ctr.o \
dut/aesbitsliced/beforenm_aes128ctr.o \
dut/aesbitsliced/common_aes128ctr.o \
dut/aesbitsliced/consts_aes128ctr.o \
dut/aesbitsliced/int128_aes128ctr.o \
dut/aesbitsliced/stream_aes128ctr.o \
dut/aesbitsliced/xor_afternm_aes128ctr.o
# CC=clang
OPTIMIZATION=-O2
#CFLAGS	= -Weverything -O0 -fsanitize=memory -fno-omit-frame-pointer -g -std=c11
CFLAGS	= $(OPTIMIZATION) -std=c11
LIBS	= -lm
#LDFLAGS	= -fsanitize=memory -fno-omit-frame-pointer -g 
#LDFLAGS = -Weverything $(OPTIMIZATION) -std=c11
LDFLAGS = $(OPTIMIZATION) -std=c11

INCS	= -Iinc/

dut_aes32: $(OBJS) $(OBJS_AES32) dut/aes32/dut_aes32.c
	$(CC) $(LDFLAGS) $(INCS) -o dudect_aes32_$(OPTIMIZATION) dut/aes32/$@.c $(OBJS) $(OBJS_AES32) $(LIBS)

dut_aesbitsliced: $(OBJS) $(OBJS_AESBITSLICED) dut/aesbitsliced/dut_aesbitsliced.c
	$(CC) $(LDFLAGS) $(INCS) -o dudect_aesbitsliced_$(OPTIMIZATION) dut/aesbitsliced/$@.c $(OBJS) $(OBJS_AESBITSLICED) $(LIBS)

dut_cmpmemcmp: $(OBJS) dut/cmpmemcmp/dut_cmpmemcmp.c
	$(CC) $(LDFLAGS) $(INCS) -o dudect_cmpmemcmp_$(OPTIMIZATION) dut/cmpmemcmp/$@.c $(OBJS) $(LIBS)

dut_cmpct: $(OBJS) dut/cmpct/dut_cmpct.c
	$(CC) $(LDFLAGS) $(INCS) -o dudect_cmpct_$(OPTIMIZATION) dut/cmpct/$@.c $(OBJS) $(LIBS)

dut_donna: $(OBJS) $(OBJS_DONNA) dut/donna/dut_donna.c
	$(CC) $(LDFLAGS) $(INCS) -o dudect_donna_$(OPTIMIZATION) dut/donna/$@.c $(OBJS) $(OBJS_DONNA) $(LIBS)

dut_donnabad: $(OBJS) $(OBJS_DONNABAD) dut/donnabad/dut_donnabad.c
	$(CC) $(LDFLAGS) $(INCS) -o dudect_donnabad_$(OPTIMIZATION) dut/donnabad/$@.c $(OBJS) $(OBJS_DONNABAD) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OBJS_AES32) $(OBJS_AESBITSLICED) $(OBJS_DONNA) $(OBJS_DONNABAD) dudect_* *.exe a.out
