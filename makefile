otherh =  -I. -I./backends -I./httplib-openssl/include -I./nlohmann_json#-I./src
header = -Isdl2/include $(otherh)

libdir = -Lsdl2/lib
lib = -lmingw32 -lSDL2main -lSDL2 -lssl -lcrypto -lws2_32 -lcrypt32 -lgdi32

otherf = $(wildcard im*.cpp) $(file) backends/imgui_impl_sdl2.cpp backends/imgui_impl_sdlrenderer2.cpp
file = test.cpp #$(wildcard src/*.cpp) #change de file
#compile all cpp in src dossier

s: #sdl compil
	g++ $(otherf) $(header) $(libdir) $(lib) -mwindows -o sdl2/bin/test
os:
	sdl2/bin/test



imgui: #imgui compil but not correct we should use header of sdl2 and cpp
	g++ test.cpp imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp -o test_imgui
oi:
	test_imgui




