# Sound-Relay
Application for audio transmission in real time within a local network to multiple devices with ESP32 as an authenticating and monitering device

Here the available ESP32 devices are shown for making the connection to establish a platform for sound relay.
<img width="1037" height="845" alt="image" src="https://github.com/user-attachments/assets/5e4a62df-a2ae-4621-abde-550e358ac845" />

Upon selecting the ESP32 device, users enter the password which is combined with the session key received from ESP32. The QT application computes the MD5 hash of (key + password) and sends it for verification. The ESP32 compares this against its pre-computed MD5 hash of (key + master_password). Matching hashes grant access to the audio channel, ensuring only authorized devices can join the sender's network.
<img width="1031" height="846" alt="image" src="https://github.com/user-attachments/assets/36eb4ae7-cba8-454f-8ec6-ea4a29961f94" />

The sender's QT application interface during active transmission, displaying current channel number (e.g., Channel 8),list of connected receiver devices with their IP addresses. The ESP32 monitoring data populates this view, allowing the sender to track exactly which authenticated devices are receiving the audio broadcast on this channel 
<img width="1037" height="845" alt="image" src="https://github.com/user-attachments/assets/1ddd601c-42ad-4f42-ab04-912692396868" />

The receiver's QT application displays all existing audio channels available on the network, managed by active ESP32 gateways. Each channel (e.g., Channel #1, #2) represents one sender broadcasting audio to multiple authenticated receivers. Users select a channel to join, and begin receiving the sender's audio stream in real-time.
<img width="1037" height="845" alt="image" src="https://github.com/user-attachments/assets/4c6e6ccf-dcfa-4bfd-9391-34aa1853647d" />





