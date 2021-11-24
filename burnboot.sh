#!/bin/sh -xe

set -e

arduino-cli burn-bootloader -P avrispmk2 -b aceduino:avr:m8xt8m -t
