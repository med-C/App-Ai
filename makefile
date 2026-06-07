otherh =  -I. -I./backends -I./httplib-openssl/include -I./nlohmann_json#-I./src
header = -Isdl2/include $(otherh)

libdir = -Lsdl2/lib
lib = -lmingw32 -lSDL2main -lSDL2 -lssl -lcrypto -lws2_32 -lcrypt32 -lgdi32

file = test.cpp

otherf = $(wildcard im*.cpp) $(file) backends/imgui_impl_sdl2.cpp backends/imgui_impl_sdlrenderer2.cpp

mongoi = -I C:/mongo-cxx-driver/include \
         -I C:/mongo-cxx-driver/include/mongocxx/v_noabi \
         -I C:/mongo-cxx-driver/include/bsoncxx/v_noabi

mongol = -L C:/mongo-cxx-driver/lib -lmongocxx -lbsoncxx -lWs2_32 -lcrypt32 -lBcrypt -lSecur32

s:
	g++ $(otherf) $(header) $(mongoi) $(libdir) $(lib) $(mongol) -mwindows -o sdl2/bin/myapp.exe

os:
	sdl2/bin/myapp.exe