import numpy as np
import bitstring
import os
from shutil import copyfile




def getEvents(infile):
  Events_ = []
  with open(infile) as f:
    for line in f.readlines():
      if line.startswith("Frame "):
        hexout = (line.split()[-1])[2:]
        frame =  line.split()[1] 
        if hexout!='0000000000000000':
          d = bitstring.pack('hex:64', hexout)
          ht = d[50:].uint / 4
          mht = d[50-14:50].uint / 4
          print('Frame {}: {}: HT = {} GeV MHT = {} GeV'.format(frame,hexout,ht,mht))
  
if __name__ == "__main__":
  print(" ---Test events from vcu118:---")
  datafile = 'data/tx_summary.txt'
  Events = getEvents(datafile)
  print("/n ---Reference events:---")
  print("TRUTH(HLS):  HT = 254.25(254.25) GeV , MHT = 91.75(94.50) GeV  ")
  print("TRUTH(HLS):  HT = 235.25(235.25) GeV , MHT = 115.50(117.50) GeV")
  print("TRUTH(HLS):  HT = 203.00(203.00) GeV , MHT = 89.75(87.50) GeV  ")
  print("TRUTH(HLS):  HT = 143.00(143.00) GeV , MHT = 37.00(34.75) GeV  ")
  print("TRUTH(HLS):  HT = 303.25(303.25) GeV , MHT = 27.25(23.25) GeV  ")
  print("TRUTH(HLS):  HT = 356.00(356.00) GeV , MHT = 115.00(117.00) GeV")
  print("TRUTH(HLS):  HT = 189.75(189.75) GeV , MHT = 121.75(123.25) GeV")
  print("TRUTH(HLS):  HT = 192.75(192.75) GeV , MHT = 123.00(121.00) GeV")
  print("TRUTH(HLS):  HT = 204.25(204.25) GeV , MHT = 76.00(73.75) GeV  ")
  print("TRUTH(HLS):  HT = 260.00(260.00) GeV , MHT = 152.00(151.75) GeV")
