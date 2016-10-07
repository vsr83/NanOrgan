# NanOrgan

Originally one-day coding project to test RtMidi and Portaudio by a quick implementation of a software synthesizer for organ sounds. For sample audio, see [Little Fugue](http://vsr.name/smallfugue_simpledelay.mp3).

Interaction of the audio and MIDI threads is not currently appropriately handled with Mutexes since the use of pthreads (for example) would imply dependence on Unix-type systems. 
