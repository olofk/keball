# keball

Create an empty workspace directory and enter it

    mkdir workspace && cd workspace

Register keball as a new fusesoc library

    fusesoc library add keball https://github.com/olofk/keball

Run keball

    fusesoc run keball

From another terminal, run

    shmidcat build/keball_0/default/trace.vcd |gtkwave -v -I something.sav

to connect GTKWave in interactive mode to the VCD FIFO

Enjoy!
