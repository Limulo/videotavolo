#! /bin/bash

# pd -jack -d 4 -verbose -stderr pd_OSC/master.pd;

# reindirizzamento dello stdout verso un file di testo, 
# quindi reindirizzamento dello stderr allo stesso file con il comando 2>&1

#pd -jack -d 4 -stderr data/PD_OSC/master.pd > PD_out.txt 2>&1 & 
#./videotavolo > VT_out.txt 2>&1 &
#cd /home/nicola/Documents/repos/reacTIVision-1.5.1/linux/; ./reacTIVision &


pd -jack  data/PD_OSC_and_LIGHT/main.pd &
./videotavolo_debug &
cd /home/nicola/Documents/repos/reacTIVision-1.5.1/linux/; ./reacTIVision &



