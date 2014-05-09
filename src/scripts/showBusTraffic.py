#!/usr/bin/python
import os, sys
import subprocess
import argparse
import matplotlib.pyplot as plt

TRAFFIC_FILE = os.path.abspath(__file__ + "/../../bustraffic.out")

stats = {
  'overall': [0],
  'busRd': [0],
  'busRdX': [0],
  'busWr': [0],
  'busUpg': [0],
}

groupSize = 500
def main():
  outfile = sys.argv[1]
  with open(TRAFFIC_FILE) as f:
    counter = groupSize
    for char in f.read():
      if counter <= 0:
        counter = groupSize
        for stat in stats:
          stats[stat].append(0)

      if char == 'R':
        stats['busRd'][-1] += 1
      elif char == 'X':
        stats['busRdX'][-1] += 1
      elif char == 'W':
        stats['busWr'][-1] += 1
      elif char == 'U':
        stats['busWr'][-1] += 1

      if char != "I":
        stats['overall'][-1] += 1

      counter -= 1

  for key in stats:
    stats[key] = [stats[key][i] / float(groupSize) for i in xrange(len(stats[key]))]

  x = range(len(stats['overall']))

  yTop = 1.5*max(stats['overall'])
  # general graph
  # gs = gridspec.GridSpec(2, 2, width_ratios=[3, 1]) 
  a = plt.subplot(221)

  plt.plot(x, stats['overall'])
  plt.fill_between(x, stats['overall'],0,color='0.2')
  plt.axis([0, 1.1*len(x), 0, yTop])
  plt.title('Overall Bus Traffic')

  # graph with busRd
  b = plt.subplot(222)

  plt.plot(x, stats['busRd'])
  plt.fill_between(x,stats['busRd'],0,color='0.2')
  plt.axis([0, 1.1*len(x), 0, yTop])
  plt.title('Bus Traffic from BusRd')

  # graph with busRdX
  c = plt.subplot(223)

  plt.plot(x, stats['busRdX'])
  plt.fill_between(x,stats['busRdX'],0,color='0.2')
  plt.axis([0, 1.1*len(x), 0, yTop])
  plt.title('Bus Traffic from BusRdX')

  # graph with busWr
  d = plt.subplot(224)

  plt.plot(x, stats['busWr'])
  plt.fill_between(x,stats['busWr'],0,color='0.2')
  plt.axis([0, 1.1*len(x), 0, yTop])
  plt.title('Bus Traffic from BusWr and BusUpg')

  plt.suptitle("Bus Traffic under %s protocol" % outfile,y=1.08, fontsize=20)

  plt.subplots_adjust(hspace=.3)
#  # graph with busUpg
#  e = plt.subplot(515)
#
#  plt.plot(x, stats['busUpg'])
#  plt.fill_between(x,stats['busUpg'],0,color='0.2')
#  plt.axis([0, 1.2*len(x), 0, yTop])
#  plt.title('Bus Traffic from BusUpg')
#
#  plt.show()
  plt.savefig(outfile + ".png")



if __name__ == '__main__':
  main()
