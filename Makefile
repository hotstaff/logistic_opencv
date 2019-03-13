CXX           = g++
CXXFLAGS      = -Ofast -Wall -I/usr/local/include
LDFLAGS       = -L/usr/local/lib
LIBS          = -lm
OBJS          = logistic_opencv.o 
PROGRAM       = logistic_opencv

LDFLAGS  += `pkg-config opencv --libs`
CFLAGS   += `pkg-config opencv --cflags`
CXXFLAGS += `pkg-config opencv --cflags`

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)
	chmod 755 $(PROGRAM)

clean:
	rm -f *.o *~ $(PROGRAM)
