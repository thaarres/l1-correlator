import numpy as np
import bitstring
import os
from shutil import copyfile

ETAPHI_LSB = np.pi / 720
   
            
class Jet:
    
    def __init__(self, pt, eta, phi, hexdata=None):
        if hexdata is not None:
            if isinstance(hexdata, str):
                d = bitstring.pack('hex:64', hexdata)
            elif isinstance(hexdata, int):
                d = bitstring.pack('int:64', hexdata)
            pt = d[48:].uint / 4
            eta = d[38:48].int / 100
            phi = d[28:38].int / 100
        self.pt = pt
        self.eta = eta
        self.phi = phi
        
    def __str__(self):
        return "Jet({}, {}, {})".format(self.pt, self.eta, self.phi)
    
    def __repr__(self):
        return self.__str__()
    
    def __eq__(self, other):
        eq = True
        eq = eq and (self.pt == other.pt)
        eq = eq and (self.eta == other.eta)
        eq = eq and (self.phi == other.phi)
        return eq
        #if isinstance(other, self.__class__):
        #    return self.__dict__ == other.__dict__
        #else:
        #    return False
    
    def iRegion(self, regions):
        return regions.iRegion(self)
        
    def fromUproot(pt_jagged, eta_jagged, phi_jagged, ptcut=None, doround=False, pclass=None):
        if pclass is None:
            pclass = Jet
        if doround:
            eta_jagged = round(eta_jagged / ETAPHI_LSB) * ETAPHI_LSB
            phi_jagged = round(phi_jagged / ETAPHI_LSB) * ETAPHI_LSB
        events = []
        for pti, etai, phii in zip(pt_jagged, eta_jagged, phi_jagged):
            event = []
            for ptij, etaij, phiij in zip(pti, etai, phii):
                particle = pclass(ptij, etaij, phiij)
                if ptcut is None:
                    event.append(particle)
                elif particle.pt > ptcut:
                    event.append(particle)
            events.append(event)
        return events
    
    def pack(self):
        eta = np.floor(self.eta / ETAPHI_LSB)
        phi = np.floor(self.phi / ETAPHI_LSB)
        pt = np.floor(self.pt * 4)
        #v = int(pt > 0)
        bs = bitstring.pack('uint:27,int:11,int:12,uint:14',0,phi,eta,pt)
        return bs.hex
    
    def toVHex(self, valid, apx=False, sideband=False):
        if apx:
            start = '0x{:02x} '.format(int(valid)) if sideband else ''
            return start + '0x' + self.pack()
        else:
            return str(int(valid)) + 'v' + self.pack()
    
    
            
def write_event_hr(f, pups, jets):
    f.write("Event with {} candidates, {} jets in the selected region\n".format(len(pups), len(jets)))
    for pup in PuppiCand.pack_event_hr(pups):
        f.write(pup)
    for jet in Jet.pack_event_hr(jets):
        f.write(jet)
    f.write("\n")
    
def header(nlinks, board='HTMHT', link_map=None, apx=False):
    if apx:
        return header_apx(nlinks, board, link_map)
    txt = 'Board {}\n'.format(board)
    txt += 'Quad/Chan :'
    for i in range(nlinks):
        j = i if link_map is None else link_map[i]
        quadstr = '        q{:02d}c{}      '.format(int(j/4), int(j%4))
        txt += quadstr
    txt += '\n      Link :'
    for i in range(nlinks):
        j = i if link_map is None else link_map[i]
        txt += '         {:03d}       '.format(j)
    txt += '\n'
    return txt
 

def frame(vhexdata, iframe, nlinks):

    txt = 'Frame {:04d} :'.format(iframe)

    for d in vhexdata:
        txt += ' ' + d
        
    txt += '\n'
    return txt

def empty_frames(n, istart, nlinks, apx=False, sideband=False):
    ''' Make n empty frames for nlinks with starting frame number istart '''
    empty_data = '0v0000000000000000'
    empty_frame = [empty_data] * nlinks
    iframe = istart
    frames = []
    for i in range(n):
        frames.append(frame(empty_frame, iframe, nlinks))
        iframe += 1
    return frames

def pack_event(jetarray,startframe):
  hex_jets = []
  for i, j in enumerate(jetarray):
    jet = j.toVHex(True)
    hex_jets.append(jet)
  return frame(hex_jets, startframe, 10)
 
def write_pattern_file(events, mux=False, link_map=None, apx=False):
    f = open('source.txt', 'w')
    iframe = 0
    # nLinks = 36 if mux else 6*16
    nLinks = 10
    f.write(header(nLinks, link_map=link_map))
    for d in empty_frames(6, iframe, nLinks):
        f.write(d)
    iframe += 6
    for event in events:
      print(event)
      row = pack_event(event, startframe=iframe)
      print(row)
      f.write(row)
      iframe += 1
    f.close()

def getEvents(infile):
  Events_ = []
  with open(infile) as f:
    events = f.read().split("Event")
    for c in events:
      event = []
      for line in c.splitlines():
        if line.startswith("   jet pt"):
          ptEtaPhi = []
          for t in line.split():
            try:
              ptEtaPhi.append(float(t))
            except ValueError:
              pass
          ptEtaPhi.pop()
          event.append(ptEtaPhi)
      if len(event)>0:
        shape = np.shape(event)
        padded_array = np.zeros((10, 3))
        padded_array[:shape[0],:shape[1]] = event
        Events_.append(padded_array)
  return Events_  

def convertToJets(events):
  events_jets = []
  for event in events:
    event_jets = []
    for j in event:
      jet = Jet(j[0],j[1],j[2])
      event_jets.append(jet)
    events_jets.append(event_jets)   
  return events_jets 
  
if __name__ == "__main__":
  
  jetFile = 'src/correlator-common/jetmet/htmht/seededcone_jets_ttbar.txt'
  Events = getEvents(jetFile)
  Events_jets = convertToJets(Events)
  write_pattern_file(Events_jets)

  