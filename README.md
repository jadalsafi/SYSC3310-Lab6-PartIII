# SYSC 3310 Lab 6 Part III

This repository contains the modified Blinky example, the standalone Satellite and Outdoor Unit models, and the coupled Satellite–ODU model.

## Reproduce the simulations

Use a C++17 compiler, Git and Cadmium v2. The runs here used Cadmium commit `aee78e681fd07865493fb1c774a44b6df016f5f3` from `dev-rt`.

```sh
git clone --branch dev-rt https://github.com/SimulationEverywhere/cadmium_v2.git ../cadmium_v2
git -C ../cadmium_v2 checkout aee78e681fd07865493fb1c774a44b6df016f5f3
CADMIUM="$(cd ../cadmium_v2 && pwd)" sh ./run.sh
```

On the Mac used for these runs, the default MacOSX27 SDK was incompatible with its installed linker. The builds succeeded with `SDKROOT=/Library/Developer/CommandLineTools/SDKs/MacOSX15.4.sdk`.

## Record the model runs

After building with `sh ./run.sh`, start a screen recording of a selected area around your Terminal window (on macOS, press Shift–Command–5). Run the following commands one at a time so each model has a separate recording.

```sh
sh ./demo.sh blinky
sh ./demo.sh satellite
sh ./demo.sh odu
sh ./demo.sh coupled
```

Each command runs a fresh simulation and displays its trace. Save the four live recordings in `videos/`.

## Results

- [Report](REPORT.md)
- [Simulation CSVs and text traces](results/)
- [Output snapshots](screenshots/)
- [Simulator output replay videos](videos/)
- [Cadmium v2 installation manual](https://devssim.carleton.ca/manuals/installation/Cadmium_Documentation_Embedded_10.pdf)
- [Official Traffic Light example](https://github.com/Sasisekhar/DEVS_Traffic_Light/tree/54333cbd96c9d32651e9411d45e92adc564bcd72)
