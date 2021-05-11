#set bitfile /afs/cern.ch/work/s/ssummers/vcu118/emp-vcu118-design/vcu118_heplnw017_pp_rl_ac_uk_190416_1841/top.bit
#set bitfile /afs/cern.ch/work/s/ssummers/p2fwk-work/proj/my-first-project/package/top.bit

open_hw
connect_hw_server
open_hw_target
current_hw_device [get_hw_devices xcvu9p_0]
refresh_hw_device -update_hw_probes false [lindex [get_hw_devices xcvu9p_0] 0]
set_property PROBES.FILE {} [get_hw_devices xcvu9p_0]
set_property FULL_PROBES.FILE {} [get_hw_devices xcvu9p_0]
set_property PROGRAM.FILE {/afs/cern.ch/work/t/thaarres/l1_hackathon/algo-work/proj/htmhtv2/package/src/htmhtv2.bit} [get_hw_devices xcvu9p_0]
program_hw_devices [get_hw_devices xcvu9p_0]
#refresh_hw_device [lindex [get_hw_devices xcvu9p_0] 0]
