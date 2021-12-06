#!/bin/sh -xe

set -e

arduino-cli compile --fqbn aceduino:avr:m8xt8m bore

# arduino-cli upload -P avrispmk2 --fqbn aceduino:avr:m8xt8m bore

arduino-cli upload --verify --verbose -p /dev/ttyUSB0 --fqbn aceduino:avr:m8xt8m bore
