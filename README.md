# Temperature-Sensitive Fan Controller

A fan control system written in C, simulating temperature-based fan speed control with manual and setpoint-driven modes. Built single-file, targeting Linux/GCC.

**Status:** In Progress — actively being extended (see Roadmap below)

## What it does

Reads a temperature input and controls fan behavior accordingly. Supports:
- Manual control mode (direct fan speed control)
- Setpoint-based auto mode with diff-based fan scaling (0–5 levels)
- Standby state, separate from control mode
- Alert signaling on high-temperature conditions

## Development History (short version)

**Stage 1 (Apr 1–20)** — First working version. Simple binary logic: fan either fully ON or fully OFF above a fixed 40°C threshold. Considered complete at the time.

**Stage 2 (May 5 – ongoing)** — Outside review exposed real design flaws, so I rebuilt the control logic from the blueprint up:
- Recalibrated thresholds to match a real room's temperature range (25–32°C), not an arbitrary 40°C
- Replaced ON/OFF with graduated `FanSpeed` (0–5) for proportional response
- Fixed manual mode so it actually sets fan speed directly, instead of still asking for a temperature
- Moved mode selection inside the main loop so modes can switch without restarting
- Added a STANDBY state, separating *how* the fan is controlled from *whether* the system runs at all
- Replaced a CPU-dependent busy-wait delay with `sleep()`
- Fixed inverted alert logic, missing input validation, a manual-mode routing bug, and a display state bug
- Added setpoint control mode with diff-based fan scaling
- Currently implementing **hysteresis** to stop rapid on/off cycling near a threshold

Full write-up with the reasoning behind each change: see [DEVLOG.md](./DEVLOG.md), or the [devlog page on my portfolio site](#).

## Roadmap
- [ ] Hysteresis — in progress
- [ ] Rate-of-change response
- [ ] Gradual fan speed ramping
- [ ] Logging
- [ ] Move to hardware (sensor, DC fan, MOSFET driver, buttons, OLED)
- [ ] PWM-based fan speed control
- [ ] IoT layer (ESP32, Wi-Fi, remote monitoring/control)

## Why single-file (for now)

Kept deliberately as one file rather than split into modules — priority has been building real understanding of the control logic before restructuring for its own sake.

## Build & Run

```bash
gcc fan_controller.c -o fan_controller
./fan_controller
```

---
*Part of my embedded systems / IoT portfolio, built while completing SIWES training in Computer Engineering.*
