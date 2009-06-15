AR = ar
RANLIB = ranlib
CODE_FOLDER = source/
ZLIB_FOLDER = $(CODE_FOLDER)zlib/
CORONA_FOLDER = $(CODE_FOLDER)corona/
BROCKOLY_FOLDER = $(CODE_FOLDER)brockoly/
SDL_FOLDER = $(CODE_FOLDER)sdl_header/
FMOD_FOLDER = $(CODE_FOLDER)fmod_header/

BROCKOLY_OBJS = \
		$(BROCKOLY_FOLDER)brockoly.o \
		$(BROCKOLY_FOLDER)engine.o \
		$(BROCKOLY_FOLDER)blending.o \
		$(BROCKOLY_FOLDER)image.o \
		$(BROCKOLY_FOLDER)texture.o \
		$(BROCKOLY_FOLDER)video.o \
		$(BROCKOLY_FOLDER)input.o \
		$(BROCKOLY_FOLDER)timer.o \
		$(BROCKOLY_FOLDER)sound.o \
		$(BROCKOLY_FOLDER)formats.o \
		$(BROCKOLY_FOLDER)mungo.o \


ZLIB_OBJS = \
		$(ZLIB_FOLDER)adler32.o \
		$(ZLIB_FOLDER)compress.o \
		$(ZLIB_FOLDER)crc32.o \
		$(ZLIB_FOLDER)gzio.o \
		$(ZLIB_FOLDER)uncompr.o \
		$(ZLIB_FOLDER)deflate.o \
		$(ZLIB_FOLDER)trees.o \
		$(ZLIB_FOLDER)zutil.o \
		$(ZLIB_FOLDER)inflate.o \
		$(ZLIB_FOLDER)infblock.o \
		$(ZLIB_FOLDER)inftrees.o \
		$(ZLIB_FOLDER)infcodes.o \
		$(ZLIB_FOLDER)infutil.o \
		$(ZLIB_FOLDER)inffast.o
	
CORONA_OBJS = \
		$(CORONA_FOLDER)Corona.o \
		$(CORONA_FOLDER)Convert.o \
		$(CORONA_FOLDER)DefaultFileSystem.o \
		$(CORONA_FOLDER)MemoryFile.o \
		$(CORONA_FOLDER)OpenBMP.o \
		$(CORONA_FOLDER)OpenGIF.o \
		$(CORONA_FOLDER)OpenPCX.o \
		$(CORONA_FOLDER)OpenPNG.o \
		$(CORONA_FOLDER)OpenJPEG.o \
		$(CORONA_FOLDER)OpenTGA.o \
		$(CORONA_FOLDER)SavePNG.o \
		$(CORONA_FOLDER)SaveTGA.o
	 
	
LIBUNGIF_OBJS= \
	$(CORONA_FOLDER)libungif-4.1.0/dgif_lib.o \
	$(CORONA_FOLDER)libungif-4.1.0/gif_err.o \
	$(CORONA_FOLDER)libungif-4.1.0/gifalloc.o	
	
LIBPNG_OBJS=	\
	$(CORONA_FOLDER)libpng-1.2.1/png.o \
	$(CORONA_FOLDER)libpng-1.2.1/pngerror.o \
	$(CORONA_FOLDER)libpng-1.2.1/pnggccrd.o \
	$(CORONA_FOLDER)libpng-1.2.1/pngget.o \
	$(CORONA_FOLDER)libpng-1.2.1/pngmem.o \
	$(CORONA_FOLDER)libpng-1.2.1/pngpread.o \
	$(CORONA_FOLDER)libpng-1.2.1/pngread.o \
	$(CORONA_FOLDER)libpng-1.2.1/pngrio.o \
	$(CORONA_FOLDER)libpng-1.2.1/pngrtran.o \
	$(CORONA_FOLDER)libpng-1.2.1/pngrutil.o \
	$(CORONA_FOLDER)libpng-1.2.1/pngset.o \
	$(CORONA_FOLDER)libpng-1.2.1/pngtest.o \
	$(CORONA_FOLDER)libpng-1.2.1/pngtrans.o \
	$(CORONA_FOLDER)libpng-1.2.1/pngvcrd.o \
	$(CORONA_FOLDER)libpng-1.2.1/pngwio.o \
	$(CORONA_FOLDER)libpng-1.2.1/pngwrite.o \
	$(CORONA_FOLDER)libpng-1.2.1/pngwtran.o \
	$(CORONA_FOLDER)libpng-1.2.1/pngwutil.o
	
LIBJPEG_OBJS=	\
	$(CORONA_FOLDER)jpeg-6b/jcapimin.o \
	$(CORONA_FOLDER)jpeg-6b/jcapistd.o \
	$(CORONA_FOLDER)jpeg-6b/jccoefct.o \
	$(CORONA_FOLDER)jpeg-6b/jccolor.o \
	$(CORONA_FOLDER)jpeg-6b/jcdctmgr.o \
	$(CORONA_FOLDER)jpeg-6b/jchuff.o \
	$(CORONA_FOLDER)jpeg-6b/jcinit.o \
	$(CORONA_FOLDER)jpeg-6b/jcmainct.o \
	$(CORONA_FOLDER)jpeg-6b/jcmarker.o \
	$(CORONA_FOLDER)jpeg-6b/jcmaster.o \
	$(CORONA_FOLDER)jpeg-6b/jcomapi.o \
	$(CORONA_FOLDER)jpeg-6b/jcparam.o \
	$(CORONA_FOLDER)jpeg-6b/jcphuff.o \
	$(CORONA_FOLDER)jpeg-6b/jcprepct.o \
	$(CORONA_FOLDER)jpeg-6b/jcsample.o \
	$(CORONA_FOLDER)jpeg-6b/jctrans.o \
	$(CORONA_FOLDER)jpeg-6b/jdapimin.o \
	$(CORONA_FOLDER)jpeg-6b/jdapistd.o \
	$(CORONA_FOLDER)jpeg-6b/jdatadst.o \
	$(CORONA_FOLDER)jpeg-6b/jdatasrc.o \
	$(CORONA_FOLDER)jpeg-6b/jdcoefct.o \
	$(CORONA_FOLDER)jpeg-6b/jdcolor.o \
	$(CORONA_FOLDER)jpeg-6b/jddctmgr.o \
	$(CORONA_FOLDER)jpeg-6b/jdhuff.o \
	$(CORONA_FOLDER)jpeg-6b/jdinput.o \
	$(CORONA_FOLDER)jpeg-6b/jdmainct.o \
	$(CORONA_FOLDER)jpeg-6b/jdmarker.o \
	$(CORONA_FOLDER)jpeg-6b/jdmaster.o \
	$(CORONA_FOLDER)jpeg-6b/jdmerge.o \
	$(CORONA_FOLDER)jpeg-6b/jdphuff.o \
	$(CORONA_FOLDER)jpeg-6b/jdpostct.o \
	$(CORONA_FOLDER)jpeg-6b/jdsample.o \
	$(CORONA_FOLDER)jpeg-6b/jdtrans.o \
	$(CORONA_FOLDER)jpeg-6b/jerror.o \
	$(CORONA_FOLDER)jpeg-6b/jfdctflt.o \
	$(CORONA_FOLDER)jpeg-6b/jfdctfst.o \
	$(CORONA_FOLDER)jpeg-6b/jfdctint.o \
	$(CORONA_FOLDER)jpeg-6b/jidctflt.o \
	$(CORONA_FOLDER)jpeg-6b/jidctfst.o \
	$(CORONA_FOLDER)jpeg-6b/jidctint.o \
	$(CORONA_FOLDER)jpeg-6b/jidctred.o \
	$(CORONA_FOLDER)jpeg-6b/jmemmgr.o \
	$(CORONA_FOLDER)jpeg-6b/jmemnobs.o \
	$(CORONA_FOLDER)jpeg-6b/jquant1.o \
	$(CORONA_FOLDER)jpeg-6b/jquant2.o \
	$(CORONA_FOLDER)jpeg-6b/jutils.o

INCDIR = $(ZLIB_FOLDER) $(CORONA_FOLDER)jpeg-6b $(CORONA_FOLDER)libungif-4.1.0 $(CORONA_FOLDER)libpng-1.2.1 $(CORONA_FOLDER)SDL_CFLAGS = $(shell sdl-config --cflags)
CFLAGS = $(SDL_CFLAGS) -O3 -Wall
SDL_LDFLAGS = $(shell sdl-config --libs)
LIBDIR = $(CODE_FOLDER)
LDFLAGS = -lm -lc -lcorona -lzlib -lGL $(SDL_LDFLAGS) libfmodex.so
CXXFLAGS = $(CFLAGS) -I$(ZLIB_FOLDER) -I$(CORONA_FOLDER) -I$(CORONA_FOLDER)jpeg-6b -I$(CORONA_FOLDER)libungif-4.1.0/ -I$(CORONA_FOLDER)libpng-1.2.1/ -I$(SDL_FOLDER) -I$(FMOD_FOLDER)

Brockoly: $(CODE_FOLDER)libzlib.a $(CODE_FOLDER)libcorona.a $(BROCKOLY_OBJS)
	g++ $(CXXFLAGS) $(BROCKOLY_OBJS) -o Brockoly -L$(LIBDIR) $(LDFLAGS)

$(CODE_FOLDER)libzlib.a: $(ZLIB_OBJS)
	$(AR) r $@ $(ZLIB_OBJS)
	$(RANLIB) $@

$(CODE_FOLDER)libcorona.a: $(CORONA_OBJS) $(LIBJPEG_OBJS) $(LIBPNG_OBJS) $(LIBUNGIF_OBJS)
	$(AR) r $@  $(CORONA_OBJS) $(LIBJPEG_OBJS) $(LIBPNG_OBJS) $(LIBUNGIF_OBJS)
	$(RANLIB) $@ 

clean:
	rm -f $(ZLIB_OBJS) $(CORONA_OBS) $(LIBJPEG_OBJS) $(LIBPNG_OBJS) $(LIBUNGIF_OBJS) $(BROCKOLY_OBJS) $(CODE_FOLDER)libzlib.a $(CODE_FOLDER)libcorona.a Brockoly
brockclean:
	rm -f $(BROCKOLY_OBJS) Brockoly
