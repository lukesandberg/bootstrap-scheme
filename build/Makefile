.PHONY: clean

TARGET =  bootstrap-scheme

SRCS=$(wildcard ../src/*.c ../src/model/*.c ../src/util/*.c ../src/gc/*.c)
INCLUDES=-I../src
OBJECTS=$(SRCS:.c=.o)

CC =gcc -std=c99
CFLAGS =-Wall -Werror -pedantic -pg -g -O3 $(INCLUDES)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@
	
clean:
	rm -f $(TARGET) $(OBJECTS)

depend:	Makefile $(SRCS)
	makedepend $(INCLUDES) $(SRCS) Makefile


# DO NOT DELETE

../src/eval.o: ../src/eval.h ../src/model/model.h /usr/include/stdio.h
../src/eval.o: /usr/include/features.h /usr/include/bits/predefs.h
../src/eval.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
../src/eval.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
../src/eval.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
../src/eval.o: /usr/include/libio.h /usr/include/_G_config.h
../src/eval.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
../src/eval.o: /usr/include/bits/sys_errlist.h ../src/gc/gc.h
../src/eval.o: /usr/include/stdlib.h /usr/include/sys/types.h
../src/eval.o: /usr/include/time.h /usr/include/endian.h
../src/eval.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
../src/eval.o: /usr/include/sys/select.h /usr/include/bits/select.h
../src/eval.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
../src/eval.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
../src/eval.o: /usr/include/alloca.h ../src/util/arraylist.h
../src/eval.o: ../src/model/environment.h ../src/model/object.h
../src/eval.o: ../src/util/string.h ../src/util/hashtable.h
../src/eval.o: ../src/util/stack.h ../src/write.h ../src/util/util.h
../src/read.o: ../src/read.h /usr/include/stdio.h /usr/include/features.h
../src/read.o: /usr/include/bits/predefs.h /usr/include/sys/cdefs.h
../src/read.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
../src/read.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
../src/read.o: /usr/include/bits/typesizes.h /usr/include/libio.h
../src/read.o: /usr/include/_G_config.h /usr/include/wchar.h
../src/read.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
../src/read.o: ../src/model/model.h ../src/gc/gc.h /usr/include/stdlib.h
../src/read.o: /usr/include/sys/types.h /usr/include/time.h
../src/read.o: /usr/include/endian.h /usr/include/bits/endian.h
../src/read.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
../src/read.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
../src/read.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
../src/read.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
../src/read.o: ../src/util/arraylist.h ../src/model/environment.h
../src/read.o: ../src/model/object.h ../src/util/string.h
../src/read.o: ../src/util/hashtable.h ../src/util/stack.h
../src/read.o: /usr/include/ctype.h /usr/include/xlocale.h
../src/repl.o: /usr/include/stdio.h /usr/include/features.h
../src/repl.o: /usr/include/bits/predefs.h /usr/include/sys/cdefs.h
../src/repl.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
../src/repl.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
../src/repl.o: /usr/include/bits/typesizes.h /usr/include/libio.h
../src/repl.o: /usr/include/_G_config.h /usr/include/wchar.h
../src/repl.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
../src/repl.o: ../src/model/model.h ../src/gc/gc.h /usr/include/stdlib.h
../src/repl.o: /usr/include/sys/types.h /usr/include/time.h
../src/repl.o: /usr/include/endian.h /usr/include/bits/endian.h
../src/repl.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
../src/repl.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
../src/repl.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
../src/repl.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
../src/repl.o: ../src/util/arraylist.h ../src/model/environment.h
../src/repl.o: ../src/model/object.h ../src/util/string.h
../src/repl.o: ../src/util/hashtable.h ../src/util/stack.h ../src/read.h
../src/repl.o: ../src/eval.h ../src/write.h
../src/write.o: /usr/include/stdio.h /usr/include/features.h
../src/write.o: /usr/include/bits/predefs.h /usr/include/sys/cdefs.h
../src/write.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
../src/write.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
../src/write.o: /usr/include/bits/typesizes.h /usr/include/libio.h
../src/write.o: /usr/include/_G_config.h /usr/include/wchar.h
../src/write.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
../src/write.o: /usr/include/stdlib.h /usr/include/sys/types.h
../src/write.o: /usr/include/time.h /usr/include/endian.h
../src/write.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
../src/write.o: /usr/include/sys/select.h /usr/include/bits/select.h
../src/write.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
../src/write.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
../src/write.o: /usr/include/alloca.h ../src/util/util.h ../src/write.h
../src/write.o: ../src/model/model.h ../src/gc/gc.h ../src/util/arraylist.h
../src/write.o: ../src/model/environment.h ../src/model/object.h
../src/write.o: ../src/util/string.h ../src/util/hashtable.h
../src/write.o: ../src/util/stack.h
../src/model/environment.o: ../src/model/environment.h ../src/model/object.h
../src/model/environment.o: ../src/gc/gc.h /usr/include/stdlib.h
../src/model/environment.o: /usr/include/features.h
../src/model/environment.o: /usr/include/bits/predefs.h
../src/model/environment.o: /usr/include/sys/cdefs.h
../src/model/environment.o: /usr/include/bits/wordsize.h
../src/model/environment.o: /usr/include/gnu/stubs.h
../src/model/environment.o: /usr/include/gnu/stubs-32.h
../src/model/environment.o: /usr/include/sys/types.h
../src/model/environment.o: /usr/include/bits/types.h
../src/model/environment.o: /usr/include/bits/typesizes.h /usr/include/time.h
../src/model/environment.o: /usr/include/endian.h /usr/include/bits/endian.h
../src/model/environment.o: /usr/include/bits/byteswap.h
../src/model/environment.o: /usr/include/sys/select.h
../src/model/environment.o: /usr/include/bits/select.h
../src/model/environment.o: /usr/include/bits/sigset.h
../src/model/environment.o: /usr/include/bits/time.h
../src/model/environment.o: /usr/include/sys/sysmacros.h
../src/model/environment.o: /usr/include/bits/pthreadtypes.h
../src/model/environment.o: /usr/include/alloca.h ../src/util/arraylist.h
../src/model/environment.o: /usr/include/stdio.h /usr/include/libio.h
../src/model/environment.o: /usr/include/_G_config.h /usr/include/wchar.h
../src/model/environment.o: /usr/include/bits/stdio_lim.h
../src/model/environment.o: /usr/include/bits/sys_errlist.h
../src/model/environment.o: ../src/util/string.h ../src/util/util.h
../src/model/environment.o: ../src/util/hashtable.h ../src/util/stack.h
../src/model/environment.o: ../src/model/model.h /usr/include/string.h
../src/model/environment.o: /usr/include/xlocale.h
../src/model/model.o: /usr/include/stdio.h /usr/include/features.h
../src/model/model.o: /usr/include/bits/predefs.h /usr/include/sys/cdefs.h
../src/model/model.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
../src/model/model.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
../src/model/model.o: /usr/include/bits/typesizes.h /usr/include/libio.h
../src/model/model.o: /usr/include/_G_config.h /usr/include/wchar.h
../src/model/model.o: /usr/include/bits/stdio_lim.h
../src/model/model.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
../src/model/model.o: /usr/include/sys/types.h /usr/include/time.h
../src/model/model.o: /usr/include/endian.h /usr/include/bits/endian.h
../src/model/model.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
../src/model/model.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
../src/model/model.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
../src/model/model.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
../src/model/model.o: /usr/include/assert.h /usr/include/string.h
../src/model/model.o: /usr/include/xlocale.h ../src/util/util.h
../src/model/model.o: ../src/util/hashtable.h ../src/util/stack.h
../src/model/model.o: ../src/model/model.h ../src/gc/gc.h
../src/model/model.o: ../src/util/arraylist.h ../src/model/environment.h
../src/model/model.o: ../src/model/object.h ../src/util/string.h
../src/model/model.o: ../src/read.h ../src/write.h ../src/eval.h
../src/model/model.o: ../src/model/procedures.h
../src/model/object.o: ../src/model/object.h ../src/gc/gc.h
../src/model/object.o: /usr/include/stdlib.h /usr/include/features.h
../src/model/object.o: /usr/include/bits/predefs.h /usr/include/sys/cdefs.h
../src/model/object.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
../src/model/object.o: /usr/include/gnu/stubs-32.h /usr/include/sys/types.h
../src/model/object.o: /usr/include/bits/types.h
../src/model/object.o: /usr/include/bits/typesizes.h /usr/include/time.h
../src/model/object.o: /usr/include/endian.h /usr/include/bits/endian.h
../src/model/object.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
../src/model/object.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
../src/model/object.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
../src/model/object.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
../src/model/object.o: ../src/util/arraylist.h /usr/include/stdio.h
../src/model/object.o: /usr/include/libio.h /usr/include/_G_config.h
../src/model/object.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
../src/model/object.o: /usr/include/bits/sys_errlist.h ../src/util/string.h
../src/model/object.o: ../src/model/model.h ../src/model/environment.h
../src/model/object.o: ../src/util/hashtable.h ../src/util/stack.h
../src/model/object.o: ../src/util/util.h ../src/model/procedures.h
../src/model/object.o: ../src/write.h
../src/model/procedures.o: ../src/model/procedures.h ../src/model/model.h
../src/model/procedures.o: /usr/include/stdio.h /usr/include/features.h
../src/model/procedures.o: /usr/include/bits/predefs.h
../src/model/procedures.o: /usr/include/sys/cdefs.h
../src/model/procedures.o: /usr/include/bits/wordsize.h
../src/model/procedures.o: /usr/include/gnu/stubs.h
../src/model/procedures.o: /usr/include/gnu/stubs-32.h
../src/model/procedures.o: /usr/include/bits/types.h
../src/model/procedures.o: /usr/include/bits/typesizes.h /usr/include/libio.h
../src/model/procedures.o: /usr/include/_G_config.h /usr/include/wchar.h
../src/model/procedures.o: /usr/include/bits/stdio_lim.h
../src/model/procedures.o: /usr/include/bits/sys_errlist.h ../src/gc/gc.h
../src/model/procedures.o: /usr/include/stdlib.h /usr/include/sys/types.h
../src/model/procedures.o: /usr/include/time.h /usr/include/endian.h
../src/model/procedures.o: /usr/include/bits/endian.h
../src/model/procedures.o: /usr/include/bits/byteswap.h
../src/model/procedures.o: /usr/include/sys/select.h
../src/model/procedures.o: /usr/include/bits/select.h
../src/model/procedures.o: /usr/include/bits/sigset.h
../src/model/procedures.o: /usr/include/bits/time.h
../src/model/procedures.o: /usr/include/sys/sysmacros.h
../src/model/procedures.o: /usr/include/bits/pthreadtypes.h
../src/model/procedures.o: /usr/include/alloca.h ../src/util/arraylist.h
../src/model/procedures.o: ../src/model/environment.h ../src/model/object.h
../src/model/procedures.o: ../src/util/string.h ../src/util/hashtable.h
../src/model/procedures.o: ../src/util/stack.h ../src/util/util.h
../src/model/procedures.o: ../src/write.h ../src/read.h ../src/eval.h
../src/model/procedures.o: /usr/include/string.h /usr/include/xlocale.h
../src/util/arraylist.o: /usr/include/string.h /usr/include/features.h
../src/util/arraylist.o: /usr/include/bits/predefs.h /usr/include/sys/cdefs.h
../src/util/arraylist.o: /usr/include/bits/wordsize.h
../src/util/arraylist.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
../src/util/arraylist.o: /usr/include/xlocale.h ../src/util/arraylist.h
../src/util/arraylist.o: /usr/include/stdlib.h /usr/include/sys/types.h
../src/util/arraylist.o: /usr/include/bits/types.h
../src/util/arraylist.o: /usr/include/bits/typesizes.h /usr/include/time.h
../src/util/arraylist.o: /usr/include/endian.h /usr/include/bits/endian.h
../src/util/arraylist.o: /usr/include/bits/byteswap.h
../src/util/arraylist.o: /usr/include/sys/select.h /usr/include/bits/select.h
../src/util/arraylist.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
../src/util/arraylist.o: /usr/include/sys/sysmacros.h
../src/util/arraylist.o: /usr/include/bits/pthreadtypes.h
../src/util/arraylist.o: /usr/include/alloca.h /usr/include/stdio.h
../src/util/arraylist.o: /usr/include/libio.h /usr/include/_G_config.h
../src/util/arraylist.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
../src/util/arraylist.o: /usr/include/bits/sys_errlist.h ../src/util/util.h
../src/util/hashtable.o: ../src/util/hashtable.h ../src/util/stack.h
../src/util/hashtable.o: ../src/util/util.h /usr/include/stdlib.h
../src/util/hashtable.o: /usr/include/features.h /usr/include/bits/predefs.h
../src/util/hashtable.o: /usr/include/sys/cdefs.h
../src/util/hashtable.o: /usr/include/bits/wordsize.h
../src/util/hashtable.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
../src/util/hashtable.o: /usr/include/sys/types.h /usr/include/bits/types.h
../src/util/hashtable.o: /usr/include/bits/typesizes.h /usr/include/time.h
../src/util/hashtable.o: /usr/include/endian.h /usr/include/bits/endian.h
../src/util/hashtable.o: /usr/include/bits/byteswap.h
../src/util/hashtable.o: /usr/include/sys/select.h /usr/include/bits/select.h
../src/util/hashtable.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
../src/util/hashtable.o: /usr/include/sys/sysmacros.h
../src/util/hashtable.o: /usr/include/bits/pthreadtypes.h
../src/util/hashtable.o: /usr/include/alloca.h /usr/include/stdio.h
../src/util/hashtable.o: /usr/include/libio.h /usr/include/_G_config.h
../src/util/hashtable.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
../src/util/hashtable.o: /usr/include/bits/sys_errlist.h
../src/util/hashtable.o: /usr/include/string.h /usr/include/xlocale.h
../src/util/stack.o: /usr/include/stdlib.h /usr/include/features.h
../src/util/stack.o: /usr/include/bits/predefs.h /usr/include/sys/cdefs.h
../src/util/stack.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
../src/util/stack.o: /usr/include/gnu/stubs-32.h /usr/include/sys/types.h
../src/util/stack.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
../src/util/stack.o: /usr/include/time.h /usr/include/endian.h
../src/util/stack.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
../src/util/stack.o: /usr/include/sys/select.h /usr/include/bits/select.h
../src/util/stack.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
../src/util/stack.o: /usr/include/sys/sysmacros.h
../src/util/stack.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
../src/util/stack.o: ../src/util/stack.h ../src/util/util.h
../src/util/string.o: /usr/include/stdlib.h /usr/include/features.h
../src/util/string.o: /usr/include/bits/predefs.h /usr/include/sys/cdefs.h
../src/util/string.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
../src/util/string.o: /usr/include/gnu/stubs-32.h /usr/include/sys/types.h
../src/util/string.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
../src/util/string.o: /usr/include/time.h /usr/include/endian.h
../src/util/string.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
../src/util/string.o: /usr/include/sys/select.h /usr/include/bits/select.h
../src/util/string.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
../src/util/string.o: /usr/include/sys/sysmacros.h
../src/util/string.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
../src/util/string.o: /usr/include/string.h /usr/include/xlocale.h
../src/util/string.o: /usr/include/assert.h ../src/util/string.h
../src/util/string.o: ../src/util/util.h
../src/util/util.o: ../src/util/util.h /usr/include/stdlib.h
../src/util/util.o: /usr/include/features.h /usr/include/bits/predefs.h
../src/util/util.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
../src/util/util.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
../src/util/util.o: /usr/include/sys/types.h /usr/include/bits/types.h
../src/util/util.o: /usr/include/bits/typesizes.h /usr/include/time.h
../src/util/util.o: /usr/include/endian.h /usr/include/bits/endian.h
../src/util/util.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
../src/util/util.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
../src/util/util.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
../src/util/util.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
../src/util/util.o: /usr/include/stdio.h /usr/include/libio.h
../src/util/util.o: /usr/include/_G_config.h /usr/include/wchar.h
../src/util/util.o: /usr/include/bits/stdio_lim.h
../src/util/util.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
../src/util/util.o: /usr/include/xlocale.h
../src/gc/gc.o: ../src/gc/gc.h /usr/include/stdlib.h /usr/include/features.h
../src/gc/gc.o: /usr/include/bits/predefs.h /usr/include/sys/cdefs.h
../src/gc/gc.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
../src/gc/gc.o: /usr/include/gnu/stubs-32.h /usr/include/sys/types.h
../src/gc/gc.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
../src/gc/gc.o: /usr/include/time.h /usr/include/endian.h
../src/gc/gc.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
../src/gc/gc.o: /usr/include/sys/select.h /usr/include/bits/select.h
../src/gc/gc.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
../src/gc/gc.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
../src/gc/gc.o: /usr/include/alloca.h ../src/util/arraylist.h
../src/gc/gc.o: /usr/include/stdio.h /usr/include/libio.h
../src/gc/gc.o: /usr/include/_G_config.h /usr/include/wchar.h
../src/gc/gc.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
../src/gc/gc.o: ../src/util/util.h
