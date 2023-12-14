# noinst = no install, on peut utiliser bin_PROGRAMS sinon
noinst_PROGRAMS = client server server2

INC_DIR = $(top_builddir)/include
SRC_DIR = $(top_builddir)/src

# Flags to compiler
AM_CPPFLAGS =  -I $(INC_DIR) -g -Wall -std=c++1y

# global LDFLAG  = Linker flags
AM_LDFLAGS = -pthread

# version locale a une cible
#cible_LDFLAGS = -pthread 

server_SOURCES = ServerSocket.h ServerSocket.cpp Socket.h Socket.cpp server.cpp

client_SOURCES = Socket.h Socket.cpp client.cpp
 
server2_SOURCES = TCPServer.h TCPServer.cpp ServerSocket.h ServerSocket.cpp Socket.h Socket.cpp server2.cpp







