# aware_home_arduino

## Overview
This project forms a part of [The Aware Home](https://gvu.gatech.edu/research/labs/aware-home-research-initiative) project, carried out during the [2023 IPAT Summer Internship Program](https://research.gatech.edu/research-interns-selected-summer-2023). Detailed documentation regarding this project can be found on this [Notion](https://geehoon.notion.site/Machine-Learning-Approaches-for-Indoor-Location-Fingerprinting-Using-Apple-Watch-RSSI-fc6cbe1d59e44aa1a66004446faf7bb2?pvs=4) page.
The system scans Bluetooth signals emitted by an Apple Watch to determine its location. Various prediction models, including Random Forest, Support Vector Machine, and K-Nearest Neighbors, were utilized.

## Project Directory
1. **BLE_scan**: Contains file for bluetooth scan.
2. **Receiver**: Contains file for receiver.


## Setup

### Sender
open the file located at `./Sender/sender.ino` and update the `myData.id` on line 28 to align with the appropriate sensor ID. Once you've made these changes, proceed to upload the sketch to your ESP32.
Change `broadcastAddress` to align with the appropriate receiver's address if needed.


### Receiver
1. Open the file located at `./Receiver/receiver.ino` and upload the sketch to ESP32.