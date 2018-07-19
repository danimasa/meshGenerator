#!/bin/bash
rsync --exclude=build --delete -avzhe "ssh -p50683" . pi@192.168.0.108:/home/pi/Documents/meshGenerator
