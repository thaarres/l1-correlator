##Project structure for L1 correlator work


Step 1) Produce Vivado project
The following steps assumes you have already designed a vivado project and for instance commited it to
https://gitlab.cern.ch/cms-cactus/phase2/firmware/correlator-common.
In this example, we are using JetMETs htmht
https://gitlab.cern.ch/thaarres/correlator-common/-/tree/seedcone-jets/jetmet/htmht


You need a computer with Vivado (eg Geonosis on lxplus)

### Prepare Vivado HLS firmware for vhdl

You need to link the firmware you made above to the rest of the board. You can prepare a folder structure with what vhdl needs, for instance by adding to your run_Synth.tcl file

make ipbb structure and .dep file

```python
file mkdir <proj>/firmware/hdl;
file mkdir <proj>/firmware/cfg;
set f [open <proj>/firmware/cfg/HT.dep "a"];
foreach filepath [glob -dir <proj>/solution1/syn/vhdl/ *] {
  set filename [file tail $filepath];
  file link ./J<proj>/firmware/hdl/$filename ../../solution1/syn/vhdl/$filename;
  puts $f "src $filename"
}
close $f
```

That should result in the following structure
```python
├── src/correlator-common/jetmet/htmht/htmht/firmware/cfg:
│   ├── htmht.dep

├── src/correlator-common/jetmet/htmht/htmht/firmware/hdl:
│   ├── algo_main_mac_muleOg.vhd
│   ├── algo_main_mac_mulg8j.vhd
│   ├── algo_main_mul_muldEe.vhd
│   ├── algo_main_mul_mulfYi.vhd
│   ├── algo_main.vhd
│   ├── ht_compute.vhd
│   ├── mht_compute_lut_ccud.vhd
│   ├── mht_compute_lut_sbkb.vhd
│   ├── mht_compute_lut.vhd
│   ├── sqrt_fixed_23_19_s.vhd

├── src/correlator-common/jetmet/htmht/htmht/solution1/syn/vhdl/:
│   ├── algo_main_mac_muleOg.vhd
│   ├── algo_main_mac_mulg8j.vhd
│   ├── algo_main_mul_muldEe.vhd
│   ├── algo_main_mul_mulfYi.vhd
│   ├── algo_main.vhd
│   ├── ht_compute.vhd
│   ├── mht_compute_lut_ccud.vhd
│   ├── mht_compute_lut_sbkb.vhd
│   ├── mht_compute_lut.vhd
│   ├── sqrt_fixed_23_19_s.vhd
```

For ipbb it always needs to end in firmware/hdl/file.vhd or firmware/cfg/file.dep
And then the *.dep contents is:
```python
│   ├── src ht_compute.vhd
│   ├── src sqrt_fixed_23_19_s.vhd
│   ├── src mht_compute_lut.vhd
│   ├── src algo_main.vhd
│   ├── src algo_main_mul_muldEe.vhd
│   ├── src algo_main_mac_muleOg.vhd
│   ├── src algo_main_mul_mulfYi.vhd
│   ├── src algo_main_mac_mulg8j.vhd
│   ├── src mht_compute_lut_sbkb.vhd
│   ├── src mht_compute_lut_ccud.vhd
```python


### Step 2) Producing a bit-file using emp-fwk


Setup ipbb

Use 2021b, latest version which work
```python
curl -L https://github.com/ipbus/ipbb/archive/dev/2021b.tar.gz | tar xvz
conda activate EMP
source ipbb-dev-2021b/env.sh
```
To run the ``ipbus gendecoders`` command later on
```python
export PATH=/opt/cactus/bin/uhal/tools:$PATH LD_LIBRARY_PATH=/opt/cactus/lib:$LD_LIBRARY_PATH
```
```python
ipbb init algo-work
cd algo-work
ipbb add git https://:@gitlab.cern.ch:8443/p2-xware/firmware/emp-fwk.git -b v0.5.6
ipbb add git https://gitlab.cern.ch/ttc/legacy_ttc.git -b v2.1
ipbb add git https://:@gitlab.cern.ch:8443/cms-tcds/cms-tcds2-firmware.git -b v0_1_1
ipbb add git https://gitlab.cern.ch/HPTD/tclink.git -r fda0bcf
ipbb add git https://github.com/ipbus/ipbus-firmware -b v1.9
```
```python
cd emp-fwk
git checkout -b v0.3.6 tags/v0.3.6
```
You also need the correlator repositories
https://gitlab.cern.ch/cms-cactus/phase2/firmware/correlator-common
https://gitlab.cern.ch/cms-cactus/phase2/firmware/correlator-layer2

Your folder structure should look like
```python
├── src
│   ├── cms-tcds2-firmware
│   ├── correlator-common
│   ├── correlator-layer1
│   ├── correlator-layer2
│   ├── emp-fwk
│   ├── ipbus-firmware
│   ├── legacy_ttc
│   └── tclink
```

### Prepare implementation of full board setup

Make repositories
```python
mkdir correlator-layer2/ht/firmware/{hdl,cfg}`
```python
and copy
```python
emp-fwk/components/payload/firmware/hdl/emp_payload.vhd
```python
into
```python
`correlatory-layer2/ht/firmware/hdl`
```
Do
```python
ipbb proj create vivado htmhtv2 correlator-layer2:htmht
```
Copy
```python
emp-fwk/projects/examples/vcu118/firmware/hdl/emp_project_decl_full`
```
into
```python
correlator-layer2/htmht/firmware/cfg/
```
Edit the file to some unique 
```python
`constant PAYLOAD_REV : std_logic_vector(31 downto 0) := X"12345678" #e.g
`constant PAYLOAD_REV : std_logic_vector(31 downto 0) := X"7EA00000"
```

Edit
```python
correlator-layer2/htmht/firmware/cfg/top.dep
```
to include `emp_project_decl_full`

Edit 
```python
correlator-layer2/htmht/firmware/hdl/emp_payload.vhd
```

to have the right input and outputs
!! Important remember to set `q(0).strobe <= '1';` in the end!!

Make a top.dep file
top.dep:
```python
include -c correlator-common:jetmet/htmht/htmht htmht.dep
include -c emp-fwk:boards/vcu118 vcu118.dep
src vcu118_decl_full.vhd
src emp_payload.vhd
```

Synthesise and implement!
```python
ipbb ipbus gendecoders
ipbb vivado generate-project
ipbb vivado synth -j4 impl -j4
ipbb vivado package
```



### Step 3) Run on the board!

Move this file: .k5login before attempting to log in!
```python
ssh cmd-phase2-trg-ctrlhub

source /home/xilinx/Vivado_Lab/2018.3/settings64.sh
export PATH=$PATH:/home/ssummers/emp-sw-containers/0.5.2/

cp /afs/cern.ch/work/s/ssummers/public/program-bit.tcl .
```

edit `program-bit.tcl` and change `PROGRAM.FILE` into `proj/htmht/package/src/<myproject>.bit`
```python
vivado_lab -mode batch -source program-bit.tcl 

sudo /afs/cern.ch/work/s/ssummers/public/pcie_reconnect_xilinx.sh
sudo chmod 777 /dev/xdma0_c2h_0 /dev/xdma0_h2c_0
c=/afs/cern.ch/work/s/ssummers/public/c-v0.3.6.xml
```
pass input
`file=source.txt`

Reset the firmware (just clears some registers)
`empbutler -c $c do vcu118 reset internal`

Configure the rx (receiver) buffers of links 0-9 in 'PlayOnce' mode.
The file $file is written (over IPBus over PCIe) into buffers (memories) next to each receiver link.
`empbutler -c $c do vcu118 buffers rx PlayOnce -c 0-9 --inject file://$file`

Configure the tx (transmit) buffer of link 0 to Capture. The algorithm only outputs to one link
`empbutler -c $c do vcu118 buffers tx Capture -c 0`

Play the data from rx buffer, through algorithm, to tx buffer
`empbutler -c $c do vcu118 capture --rx 0-9 --tx 0`


The following versions of libraries are used in this work:

1. emp-fwk: v0.5.5 5bc4343 v0.5.5 release: Increment version numbers
2. legacy_ttc: v2.1
3. cms-tcds2-firmware: v0_1_1
4. tclink: detached from fda0bcf??
5. ipbus-firmware: v1.8

