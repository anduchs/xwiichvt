xwiichvt
========

Daemon that uses wiimotes to change and signal the current Linux VT using the wiimote
(more specifically libxwiimote)

- Buttons 1 and 2 are for switchting to VT number -=1 and +=1
- LEDS signal current VT: 1 to 4 directly. VT5-7 are 1+2, 2+3, 3+4, VT8-9 are 1+2+3, 2+3+4 and VT10 is 1+2+3+4
- Single-Device, Single-Threaded; loop until device is found, loop over device lifetime for events on 1+2 (hardcoded)


TODO
----
- Write service files (sysvinit, upstart, systemd)
- Write man-page (ronn-format)
- Change to be udev-event-activated (see service-files); Use -o for these cases.
- Use udev-device-info as parameter (see service-files); use as parameter to allow multiple wiimotes
- make this behaviour configurable ? Maybe udev-rules-tag (see service-files)
- Make buttons, leds configurable
- look for currently active vt even without keypress (forein VT-change); is there some kind of notifier ?
