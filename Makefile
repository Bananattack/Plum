UNAME_S := $(shell uname -s)

LUA_INC := source/lua/src
LUA_C := $(filter-out $(LUA_INC)/lua.c $(LUA_INC)/luac.c, $(wildcard $(LUA_INC)/*.c))
LUA_H := $(wildcard $(LUA_INC)/*.h)
LUA_OBJS := $(patsubst %.c, obj/%.o, $(LUA_C))
LUA := obj/liblua.a

JPEG_INC := source/corona/jpeg-6b
JPEG_C := $(wildcard $(JPEG_INC)/*.c)
JPEG_H := $(wildcard $(JPEG_INC)/*.h)
JPEG_OBJS := $(patsubst %.c, obj/%.o, $(JPEG_C))
JPEG := obj/libjpeg.a

PNG_INC := source/corona/libpng-1.2.1
PNG_C := $(wildcard $(PNG_INC)/*.c)
PNG_H := $(wildcard $(PNG_INC)/*.h)
PNG_OBJS := $(patsubst %.c, obj/%.o, $(PNG_C))
PNG := obj/libpng.a

ZLIB_INC := source/zlib
ZLIB_C := $(filter-out $(ZLIB_INC)/example.c, $(wildcard $(ZLIB_INC)/*.c))
ZLIB_H := $(wildcard $(ZLIB_INC)/*.h)
ZLIB_OBJS := $(patsubst %.c, obj/%.o, $(ZLIB_C))
ZLIB := obj/libzlib.a

UNGIF_INC := source/corona/libungif-4.1.0
UNGIF_C := $(wildcard $(UNGIF_INC)/*.c)
UNGIF_H := $(wildcard $(UNGIF_INC)/*.h)
UNGIF_OBJS := $(patsubst %.c, obj/%.o, $(UNGIF_C))
UNGIF := obj/libungif.a

CORONA_INC := source/corona
CORONA_CPP := $(wildcard $(CORONA_INC)/*.cpp)
CORONA_H := $(wildcard $(CORONA_INC)/*.h)
CORONA_OBJS := $(patsubst %.cpp, obj/%.o, $(CORONA_CPP))
CORONA := obj/libcorona.a

GLEW_SRC := source/glew/src
GLEW_INC := source/glew/include
GLEW_C := $(wildcard $(GLEW_SRC)/*.c)
GLEW_H := $(wildcard $(GLEW_INC)/*.h)
GLEW_OBJS := $(patsubst %.c, obj/%.o, $(GLEW_C))
GLEW := obj/libglew.a

GLFW_SRC := source/glfw/src
GLFW_INC := source/glfw/include
ifeq ($(UNAME_S),Linux)
	GLFW_OS_C := $(wildcard $(GLFW_SRC)/glx_*.c $(GLFW_SRC)/x11_*.c)
	GLFW_OS_M :=
endif
ifeq ($(UNAME_S),Darwin)
	GLFW_OS_C := $(wildcard $(GLFW_SRC)/nsgl_*.c $(GLFW_SRC)/cocoa_*.c)
	GLFW_OS_M := $(wildcard $(GLFW_SRC)/nsgl_*.m $(GLFW_SRC)/cocoa_*.m)	
endif
GLFW_C := $(filter-out $(wildcard $(GLFW_SRC)/*_*.c), $(wildcard $(GLFW_SRC)/*.c)) $(GLFW_OS_C)
GLFW_H := $(wildcard $(GLFW_INC)/*.h)
GLFW_M := $(GLFW_OS_M)
GLFW_C_OBJS := $(patsubst %.c, obj/%.o, $(GLFW_C))
GLFW_M_OBJS := $(patsubst %.m, obj/%.o, $(GLFW_M))
GLFW := obj/libglfw.a

$(info GLFW_OS_M = $(GLFW_OS_M))

PLAID_INC := source/plaidaudio
PLAID_SRC := $(PLAID_INC)/plaid
PLAID_CPP := $(wildcard $(PLAID_SRC)/*.cpp $(PLAID_SRC)/../imp-portaudio/*.cpp $(PLAID_SRC)/**/*.cpp $(PLAID_SRC)/**/**/*.cpp)
PLAID_H := $(wildcard $(PLAID_SRC)/*.h $(PLAID_SRC)/**/*.h $(PLAID_SRC)/**/**/*.h)
PLAID_OBJS := $(patsubst %.cpp, obj/%.o, $(PLAID_CPP))
PLAID := obj/libplaid.a

MODPLUG_INC := source/libmodplug/src
MODPLUG_CPP := $(wildcard $(MODPLUG_INC)/*.cpp)
MODPLUG_H := $(wildcard $(MODPLUG_INC)/*.h)
MODPLUG_OBJS := $(patsubst %.cpp, obj/%.o, $(MODPLUG_CPP))
MODPLUG := obj/libmodplug.a

PORTAUDIO_SRC := source/portaudio/src
PORTAUDIO_INC := source/portaudio/include
ifeq ($(UNAME_S),Linux)
	PORTAUDIO_OS_CPP := $(wildcard $(PORTAUDIO_SRC)/hostapi/alsa/*.cpp)
	PORTAUDIO_OS_C := $(wildcard $(PORTAUDIO_SRC)/hostapi/alsa/*.cpp)
endif
ifeq ($(UNAME_S),Darwin)
	PORTAUDIO_OS_CPP := $(wildcard $(PORTAUDIO_SRC)/hostapi/coreaudio/*.cpp)
	PORTAUDIO_OS_C := $(wildcard $(PORTAUDIO_SRC)/hostapi/coreaudio/*.cpp)
endif
PORTAUDIO_CPP := $(wildcard $(PORTAUDIO_SRC)/common/*.cpp $(PORTAUDIO_SRC)/os/unix/*.cpp) $(PORTAUDIO_OS_CPP)
PORTAUDIO_C := $(wildcard $(PORTAUDIO_SRC)/common/*.c $(PORTAUDIO_SRC)/os/unix/*.c $(PORTAUDIO_SRC)/hostapi/alsa/*.c) $(PORTAUDIO_OS_C)
PORTAUDIO_H := $(wildcard $(PORTAUDIO_INC)/*.h)
PORTAUDIO_C_OBJS := $(patsubst %.c, obj/%.o, $(PORTAUDIO_C))
PORTAUDIO_CPP_OBJS := $(patsubst %.cpp, obj/%.o, $(PORTAUDIO_CPP))
PORTAUDIO := obj/libportaudio.a

PLUM_CPP := $(wildcard source/plum/**/*.cpp source/plum/platform/**/*.cpp $(PLAID_SRC)/../codec_stb/*.cpp) source/plum/plum.cpp
PLUM_C := $(PLAID_SRC)/../codec_stb/stb_vorbis.c
PLUM_H := $(wildcard source/plum/**/*.h source/plum/platform/**/*.h)
PLUM_C_OBJS := $(patsubst %.c, obj/%.o, $(PLUM_C))
PLUM_CPP_OBJS := $(patsubst %.cpp, obj/%.o, $(PLUM_CPP))
PLUM_LIBS := lua corona jpeg png zlib ungif glew glfw plaid modplug portaudio
PLUM_DEPS := $(foreach i, $(PLUM_LIBS), obj/lib$(i).a)
PLUM := test/plum

ifeq ($(UNAME_S),Linux)
	CFLAGS += --std=gnu99 -Wall -DPA_USE_ALSA -DPA_ENABLE_DEBUG_OUTPUT -include pthread.h -include sys/time.h -O3
	CXXFLAGS += --std=c++0x -Wall -DPA_USE_ALSA -DHAVE_STDINT_H -DHAVE_SETENV -DHAVE_SINF -DPA_ENABLE_DEBUG_OUTPUT -O3
	LDFLAGS += -Lobj/ $(patsubst %, -l%, $(PLUM_LIBS)) -lm -lasound -lpthread -lXrandr -lXi -lXext -lXxf86vm -lX11 -lGL -lGLU
	INCLUDES := -I$(LUA_INC) -I$(JPEG_INC) -I$(PNG_INC) -I$(ZLIB_INC) -I$(UNGIF_INC) -I$(CORONA_INC) \
		-I$(GLEW_INC) -I$(GLFW_INC) -I$(GLFW_INC)/../plum -I$(PLAID_INC) -I$(MODPLUG_INC) -I$(MODPLUG_INC)/libmodplug \
		-I$(PORTAUDIO_INC) -I$(PORTAUDIO_SRC)/common -I$(PORTAUDIO_SRC)/os/unix
endif
ifeq ($(UNAME_S),Darwin)
	CFLAGS += --std=gnu99 -Wall -DPA_USE_COREAUDIO -include pthread.h -include sys/time.h -O3 -fno-common
	CXXFLAGS += --std=c++0x -Wall -DPA_USE_COREAUDIO -DHAVE_STDINT_H -DHAVE_SETENV -DHAVE_SINF -O3
	LDFLAGS += -framework Foundation -framework Cocoa -framework OpenGL -framework IOKit -Lobj/ $(patsubst %, -l%, $(PLUM_LIBS)) -lm -lpthread -lobjc
	INCLUDES := -I$(LUA_INC) -I$(JPEG_INC) -I$(PNG_INC) -I$(ZLIB_INC) -I$(UNGIF_INC) -I$(CORONA_INC) \
		-I$(GLEW_INC) -I$(GLFW_INC) -I$(GLFW_INC)/../plum -I$(PLAID_INC) -I$(MODPLUG_INC) -I$(MODPLUG_INC)/libmodplug \
		-I$(PORTAUDIO_INC) -I$(PORTAUDIO_SRC)/common -I$(PORTAUDIO_SRC)/os/unix
endif

.PHONY: clean all

define uniq
	$(eval seen :=)
	$(foreach _,$1,$(if $(filter $_,${seen}),,$(eval seen += $_)))
	${seen}
endef
DIRECTORIES := $(call uniq, $(dir \
	$(LUA_OBJS) $(JPEG_OBJS) $(PNG_OBJS) $(ZLIB_OBJS) $(UNGIF_OBJS) \
	$(CORONA_OBJS) $(GLEW_OBJS) $(PLAID_OBJS) $(MODPLUG_OBJS) \
	$(GLFW_C_OBJS) $(GLFW_M_OBJS) \
	$(PORTAUDIO_C_OBJS) $(PORTAUDIO_CPP_OBJS) \
	$(PLUM_C_OBJS) $(PLUM_CPP_OBJS) \
	))

all: $(DIRECTORIES) $(PLUM)
clean:
	rm -rf obj test/plum

$(LUA_OBJS): obj/%.o: %.c $(LUA_H)
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDES)
$(LUA): $(LUA_OBJS)
	$(AR) cr $@ $^
$(JPEG_OBJS): obj/%.o: %.c $(JPEG_H)
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDES)
$(JPEG): $(JPEG_OBJS)
	$(AR) cr $@ $^
$(PNG_OBJS): obj/%.o: %.c $(PNG_H)
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDES)
$(PNG): $(PNG_OBJS)
	$(AR) cr $@ $^
$(ZLIB_OBJS): obj/%.o: %.c $(ZLIB_H)
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDES)
$(ZLIB): $(ZLIB_OBJS)
	$(AR) cr $@ $^
$(UNGIF_OBJS): obj/%.o: %.c $(UNGIF_H)
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDES)
$(UNGIF): $(UNGIF_OBJS)
	$(AR) cr $@ $^
$(CORONA_OBJS): obj/%.o: %.cpp $(CORONA_H)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCLUDES)
$(CORONA): $(CORONA_OBJS)
	$(AR) cr $@ $^
$(GLEW_OBJS): obj/%.o: %.c $(GLEW_H)
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDES)
$(GLEW): $(GLEW_OBJS)
	$(AR) cr $@ $^
$(GLFW_C_OBJS): obj/%.o: %.c $(GLFW_H)
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDES)
$(GLFW_M_OBJS): obj/%.o: %.m $(GLFW_H)
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDES)
$(GLFW): $(GLFW_C_OBJS) $(GLFW_M_OBJS)
	$(AR) cr $@ $^
$(PLAID_OBJS): obj/%.o: %.cpp $(PLAID_H)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCLUDES)
$(PLAID): $(PLAID_OBJS)
	$(AR) cr $@ $^
$(MODPLUG_OBJS): obj/%.o: %.cpp $(MODPLUG_H)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCLUDES)
$(MODPLUG): $(MODPLUG_OBJS)
	$(AR) cr $@ $^
$(PORTAUDIO_C_OBJS): obj/%.o: %.c $(PORTAUDIO_H)
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDES)
$(PORTAUDIO_CPP_OBJS): obj/%.o: %.cpp $(PORTAUDIO_H)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCLUDES)
$(PORTAUDIO): $(PORTAUDIO_C_OBJS) $(PORTAUDIO_CPP_OBJS)
	$(AR) cr $@ $^	

$(PLUM_C_OBJS): obj/%.o: %.c $(PLUM_H)
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDES)
$(PLUM_CPP_OBJS): obj/%.o: %.cpp $(PLUM_H)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCLUDES)
$(PLUM): $(DIRECTORIES) $(PLUM_C_OBJS) $(PLUM_CPP_OBJS) $(PLUM_H) $(PLUM_DEPS)
	$(CXX) $(CXXFLAGS) $(PLUM_C_OBJS) $(PLUM_CPP_OBJS) $(LDFLAGS) -o $@

define makedir
$(1):
	mkdir -p $(1)
endef
$(foreach d, $(DIRECTORIES), $(eval $(call makedir, $(d))))
