#!/bin/bash
make -C build && sudo picotool load build/*.uf2 -fx
