#include <iostream>
#include "RtMIDInterface.h"

using namespace std;

class Handler
{
public:
    static void noteOnCallback(unsigned char channel,
                               unsigned char note,
                               unsigned char vel,
                               void *userData);
    int data;
};

void
Handler::noteOnCallback(unsigned char channel,
                             unsigned char note,
                             unsigned char vel,
                             void *userData) {
    Handler *h = (Handler *)userData;
    std::cout << h->data << std::endl;
    std::cout << "noteOn " << channel << " " << note << " " << vel << std::endl;
}

int main()
{
    Handler h;
    h.data = 12345;

    RtMIDIInterface rtmidi;
    rtmidi.openPort(0);
    rtmidi.setNoteOnCallback(&Handler::noteOnCallback, &h);

    int foo;
    cin >> foo;
    return 0;
}

