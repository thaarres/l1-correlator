# open the project and reset
open_project -reset htmht
set_top algo_main
add_files firmware/ht.cpp -cflags -std=c++0x

# reset the solution
open_solution -reset "solution1"
##   VCU118 dev kit (VU9P), 320 MHz
set_part {xcvu9p-flga2104-2L-e}
create_clock -period 2.5

# synthethize the algorithm
csynth_design

# make ipbb structure and .dep file
file mkdir htmht/firmware/hdl;
file mkdir htmht/firmware/cfg;
set f [open htmht/firmware/cfg/htmht.dep "a"];

foreach filepath [glob -dir htmht/solution1/syn/vhdl/ *] {
 set filename [file tail $filepath];
 file link ./htmht/firmware/hdl/$filename ../../solution1/syn/vhdl/$filename;
 puts $f "src $filename"
}
close $f
exit
