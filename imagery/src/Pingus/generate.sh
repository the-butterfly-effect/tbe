#!/bin/bash

## This script generates our images from the original Pingus source imagery

montage -background '#FFFFFF00' -geometry +0+0 walker.png faller.png slider-left.png slider-right.png splat.png waiter.png exit.png dead.png sleeper.png dead.png -tile 1x10 pingus.png

convert -background '#FFFFFF00' -crop 32x32+0+0  walker.png pinguswalkleft.png
convert -background '#FFFFFF00' -crop 32x32+0+32 walker.png pinguswalkright.png
convert -background '#FFFFFF00' -crop 32x32+32+0 sleeper.png pingussleeper.png
