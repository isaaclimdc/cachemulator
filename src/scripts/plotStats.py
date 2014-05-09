#!/usr/bin/python

import os
from mpl_toolkits.mplot3d import axes3d,  Axes3D
from collections import defaultdict
import matplotlib.pyplot as plt
STAT_FILE = os.path.abspath(__file__ + "/../../stats.report")

protocols = ['MSI', 'MESI', 'MESIF']
colors = ['r', 'c', 'b', 'y', 'c']
width=0.35

keys = [
"invalidations",
"hits",
"misses",
"evictions",
"BusRd",
"BusRdX",
"BusWr",
"BusUpg",
"Contented lines",

# start important metrics
"ticks",
"C2C",
]

MSI_vals = []
MESI_vals = []
MESIF_vals = []

vals = [MSI_vals, MESI_vals, MESIF_vals]

def main():
  stats = defaultdict(list)
  protId = -1
  with open(STAT_FILE) as f:
    fileContent = f.read()
    fileContentList = fileContent.split("\n")
    for item in fileContentList:
      print item
      if item.startswith("="):
        protId += 1
      else:
        try:
          key, value = item.split(" ")
          vals[protId].append(int(value))
        except:
          pass
  plot()

numImpor = 2

def plot():
  fig = plt.figure()
  ax = fig.add_subplot(111)
  length = len(keys) - numImpor
  ind = range(length)
  width = 0.2
  MSI_b = ax.bar(ind, MSI_vals[:length], width, color=colors[0], alpha=0.8)
  MESI_b = ax.bar(map(lambda x: x+width, ind), MESI_vals[:length], width, color=colors[1], alpha=0.8)
  MESIF_b = ax.bar(map(lambda x: x+2*width, ind), MESIF_vals[:length], width, color=colors[2], alpha=0.8)
  ax.set_xticks(map(lambda x: x+2*width, ind))
  ax.set_xticklabels(keys[:length], rotation=45, fontsize=10)
  ax.set_ylabel('Counts')
  ax.set_title('Performance stats of different coherence protocols (similar stats)')
  ax.legend( (MSI_b[0], MESI_b[0], MESIF_b[0]), ('MSI', 'MESI', 'MESIF') )
  fig.savefig("similar_stat.png")

  fig = plt.figure()
  idx = length
  ax = fig.add_subplot(111)
  length = len(keys) - numImpor
  ind = range(1)
  width = 0.2
  MSI_b = ax.bar(map(lambda x: x+width, ind), MSI_vals[idx:idx+1], width, color=colors[0], alpha=0.8)
  MESI_b = ax.bar(map(lambda x: x+2*width, ind), MESI_vals[idx:idx+1], width, color=colors[1], alpha=0.8)
  MESIF_b = ax.bar(map(lambda x: x+3*width, ind), MESIF_vals[idx:idx+1], width, color=colors[2], alpha=0.8)
  ax.set_xticks(map(lambda x: x+width, ind))
  ax.set_xticklabels(keys[idx:idx+1], rotation=45, fontsize=10)
  ax.set_ylabel('Tick Counts')
  ax.set_xlabel('Protocols')
  ax.set_title('Total Ticks')
  ax.set_xbound(-0.2 ,1.5)
  ax.legend( (MSI_b, MESI_b, MESIF_b), ('MSI', 'MESI', 'MESIF') )
  fig.savefig("ticks.png")

  fig = plt.figure()
  idx = idx+1
  ax = fig.add_subplot(111)
  length = len(keys) - numImpor
  ind = range(1)
  width = 0.2
  MSI_b = ax.bar(map(lambda x: x+width, ind), MSI_vals[idx:idx+1], width, color=colors[0], alpha=0.8)
  MESI_b = ax.bar(map(lambda x: x+2*width, ind), MESI_vals[idx:idx+1], width, color=colors[1], alpha=0.8)
  MESIF_b = ax.bar(map(lambda x: x+3*width, ind), MESIF_vals[idx:idx+1], width, color=colors[2], alpha=0.8)
  ax.set_ylabel('Counts')
  ax.set_xlabel('Protocols')
  ax.set_xbound(-0.2 ,1.5)
  ax.set_title('Cache to Cache Transfers')
  ax.legend( (MSI_b, MESI_b, MESIF_b), ('MSI', 'MESI', 'MESIF') )
  fig.savefig("C2C.png")

#  # plotting
#  fig = plt.figure()
#  # ax = Axes3D(fig)
#  ax = fig.add_subplot(111)
#  i = 0
#  #X, Y, Z = axes3d.get_test_data(0.05)
#  #cset = ax.contour(X, Y, Z, 16, extend3d=True)
#  #ax.clabel(cset, fontsize=9, inline=1)
#  ind = range(3)
#  for key, vals in stats.iteritems():
#    vals = map(int, vals)
#    if len(vals) != 3:
#      print vals
#    ax.bar(ind, vals, width, color=colors[i%len(colors)], alpha=0.8)
#    ax.set_xticks([0.5,1.5,2.5])
#    ax.set_xticklabels(protocols, fontsize=20, color='b')
#    i += 1
#    #fig.savefig(key + ".png")
#  plt.show()

if __name__ == '__main__':
  main()
