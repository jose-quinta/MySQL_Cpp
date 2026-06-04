CXX      = g++.exe
CXXFLAGS = -std=c++11 -Wall -I include -I"C:/Program Files/MariaDB 12.3/include/mysql"
LDFLAGS  = -L"C:/Program Files/MariaDB 12.3/lib" -lmariadb
RM       = del /Q

SRCDIR   = src
INCDIR   = include
BINDIR   = bin
TARGET   = $(BINDIR)/MySQL.exe

SRCS     = $(SRCDIR)/main.cpp \
           $(SRCDIR)/db/Database.cpp \
           $(SRCDIR)/model/Person.cpp \
           $(SRCDIR)/repository/PersonRepository.cpp \
           $(SRCDIR)/ui/Console.cpp

OBJS     = $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(BINDIR) $(TARGET)

$(BINDIR):
	if not exist "$(BINDIR)" mkdir "$(BINDIR)"

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(SRCDIR)/main.o: $(SRCDIR)/main.cpp $(INCDIR)/db/Database.h $(INCDIR)/ui/Console.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(SRCDIR)/db/Database.o: $(SRCDIR)/db/Database.cpp $(INCDIR)/db/Database.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(SRCDIR)/model/Person.o: $(SRCDIR)/model/Person.cpp $(INCDIR)/model/Person.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(SRCDIR)/repository/PersonRepository.o: $(SRCDIR)/repository/PersonRepository.cpp $(INCDIR)/repository/PersonRepository.h $(INCDIR)/db/Database.h $(INCDIR)/model/Person.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(SRCDIR)/ui/Console.o: $(SRCDIR)/ui/Console.cpp $(INCDIR)/ui/Console.h $(INCDIR)/repository/PersonRepository.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	$(RM) $(SRCDIR)\*.o 2>nul || ver>nul
	$(RM) $(SRCDIR)\db\*.o 2>nul || ver>nul
	$(RM) $(SRCDIR)\model\*.o 2>nul || ver>nul
	$(RM) $(SRCDIR)\repository\*.o 2>nul || ver>nul
	$(RM) $(SRCDIR)\ui\*.o 2>nul || ver>nul
	$(RM) $(BINDIR)\*.exe 2>nul || ver>nul
