# Beep Device with NtDeviceIoControlFile @Pyramidyon

**PoC with sound:**

https://github.com/pyramidyon/Beep-Device-with-Native-API/assets/88564775/3c23bd84-d754-4924-90bd-e639413f8ad4

## Overview
The **Native Beep Device with NT API** is a Proof of Concept (PoC) that demonstrates direct interactions with the beep device using NT APIs from user mode (ring3). This approach showcases how to bypass certain standard Win32/NT APIs to manage device operations effectively. This is meant to showcase what malware could possible do to bypass certain APIs.

## Features
- Directly open and control the beep device using `NtOpenFile` and `NtDeviceIoControlFile`.
- Adjust beep frequency and duration parameters
- Demonstrate potential for system-level interactions beyond standard API constraints.

