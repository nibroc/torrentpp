CXXFLAGS+=-Wall -Werror -Wextra -Weffc++ \
	    -Wstrict-aliasing -pedantic -std=c++11
LDLIBS+=

CXX=clang++

SRCS=torrent.cpp bencode.cpp bdecode.cpp
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
EXE=torrent++
DEPS=.depend

release: CXXFLAGS+=-O2
release: build

debug: CXXFLAGS+=-O0 -g
debug: LDFLAGS+=-g
debug: build

build: depend $(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(LDFLAGS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(CXX) $(CXXFLAGS) -MM $(SRCS) > ./.depend

clean:
	$(RM) $(RMFLAGS) $(EXE) $(TOK) $(OBJS) retrieve .depend

-include $(DEPS)