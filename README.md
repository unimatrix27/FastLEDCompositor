# FastLEDCompositor
Framework of classes to enable the composition of several animations using different methods of sequencing

The Composition framework is supposed to implement a collection of Classes that interact with each other
in order to simplify the sequencing of LED-effects on a 1-dimensional LED-Setup. Background idea of this project
was to have LED effects on an strip with 1200 WS2812 LEDs of the author running synced to music 
and to enable the user to predifine sequences of several patterns at the same time on this strip which is installed
going around the complete room which is mostly used by teenagers for partying.

The framework is supposed to hold a library of effects which can be easily extended and therefore uses abstraction and polymorphism
The framework can blend one effect over another and enables the creation of new blending patterns just by adding code
The framework can run several animations at the same time, both at parts of the strip as well as on the full strip and even move them around

Not yet implemented at all:
The framework shall enable the control of sequences through different interfaces:
- using a MIDI controller (software or hardware) talking to a Teensy3 sending notes, channel commands, etc and thus 
  infuencing the parameters of the effect, e.g. hue variations, size, brightness, randomness, etc. MIDI will also be
  used to sync the playback of animations. The MIDI setup will be useable both in a live mode (e.g. pressing keys on the keyboard
  immediately influencing the behavior of animations or in a sequencer mode, where a predefined "song" is played back on the strip 
  in sync to an actual 
- Another idea is to create an interface to  parametric configuration, e.g. using a JSON file as input. The Teensy could even hold
  a number of predefined programs. 
