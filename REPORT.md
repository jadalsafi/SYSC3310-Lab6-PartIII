# SYSC 3310 Laboratory 6, Part III

Jad Al Safi · November 27, 2026

## Environment checks

The examples and the lab models were run with Cadmium v2 on macOS arm64. The university's Traffic Light example ran for 23 seconds. Its trace emitted red at 10.2 s, green at 20.2 s and amber at 22.2 s. The original Blinky example produced internal outputs at 3, 6 and 9 s, processed its first input at 10 s, and then emitted outputs 0.75 s apart. The saved baseline trace and CSV are in [results](results/).

## Modified Blinky

The slow and fast delays were changed to 2.0 s and 0.5 s. The output, internal transition and external transition methods print when they execute and the state of `lightOn`.

The first output events occur at 2, 4, 6, 8 and 10 s. The trace shows that `output` sends the current state and `internalTransition` toggles it afterward. The first button input arrives at 10 s, at the same time as a scheduled internal event. The trace shows output first, the internal transition second, and the external transition third. The external transition changes the time advance to 0.5 s, so the following outputs occur at 10.5, 11 and 11.5 s. The next button event at 20 s restores the 2.0 s delay.

## Satellite

The model alternates between passive for 3 s and illuminate for 2 s. At the end of the passive state it emits beam ON. At the end of illumination it emits beam OFF. The simulation shows ON at 3, 8, 13 and 18 s and OFF at 5, 10 and 15 s. The first three complete passes each last 2 s and their starts are 5 s apart.

## Outdoor Unit

The ODU starts acquisition on beam ON. After 2 s it emits signal ON and enters its transmit/receive state. A beam OFF while transmitting starts a 1 s release timer. If the beam stays OFF, the model emits signal OFF and returns to passive.

| Test stimulus | Observed in simulation |
| --- | --- |
| Beam ON at 1 s and OFF at 1.5 s | Acquisition cancelled. No signal output. |
| Beam ON at 4 s, then another ON at 4.5 s | Signal turned ON at 6 s. The duplicate message did not restart the timer. |
| Beam OFF at 7 s, then ON at 7.5 s | Release cancelled. Signal stayed ON. |
| Beam OFF at 10 s, then another OFF at 10.25 s | Signal turned OFF at 11 s. The duplicate message did not postpone release. |

## Coupled model

The Satellite's `beam_out` is coupled directly to the ODU's `beam_in`. The first beam rises at 3 s and falls at 5 s. At 5 s the acquisition timer expires at the same simulation time as the falling beam. Cadmium emits the ODU's signal ON and processes the ODU's internal transition before applying the beam OFF input to the ODU. The ODU then starts release and emits signal OFF at 6 s. The same sequence repeats at 8/10/11 s and 13/15/16 s.

## Evidence

The live screen recordings show the terminal command and simulation output for [Modified Blinky](videos/Blinky_Live.mov), [Satellite](videos/Satellite_Live.mov), [Outdoor Unit](videos/ODU_Live.mov), and the [coupled model](videos/Coupled_Live.mov).

The CSVs and text traces contain the simulator output. The PNG snapshots and AVI replays are supplemental. All timing results are simulation timestamps. The MSP432 and the optional IDU extension were outside this run.
