#!/usr/bin/python
import os, sys
import subprocess
import argparse

outFiles = ["hitsmisses.out", "busshouts.out"]

def report(test, sol):
  if test == sol:
    print "TEST PASSED!"
  else:
    print "TEST FAILED..."


def checkHitsMisses(parsed, traceFile):
  sol = []
  if traceFile == "traces/easy1.trace":
    sol = ['M','M','M','H','H','H','M','H','H','H']
  elif traceFile == "traces/evict1.trace":
    sol = ['M','M','H','H','E','E']
  elif traceFile == "traces/coherent1.trace":
    sol = ['M','M','M','H','H','H']

  report(parsed[0], sol)


def checkBusShouts(parsed, traceFile):
  sol = []
  if traceFile == "traces/easy1.trace":
    sol = ['R','R','R','U','R']
  elif traceFile == "traces/evict1.trace":
    sol = ['R','R','R','R']
  elif traceFile == "traces/coherent1.trace":
    sol = ['R','R','R','U','X']

  report(parsed[1], sol)


def parse():
  parsedFiles = []
  for outFile in outFiles:
    with open(outFile) as f:
      content = f.readlines()
      bare = map (lambda r: r.strip(), content)
      parsedFiles.append(bare)
  return parsedFiles


def run(traceFile, protocol):
  subprocess.call(["./cache", "-t", traceFile, "-p", protocol])


def clean():
  for outFile in outFiles:
    subprocess.call(["rm", outFile])


def main():
  ap = argparse.ArgumentParser()
  ap.add_argument('-t', help="Tracefile path")
  ap.add_argument('-p', help="Coherence protocol")
  opts = ap.parse_args()

  traceFile = opts.t
  protocol = opts.p
  run(traceFile, protocol)

  print
  print "~ Verifying output"

  parsed = parse()

  print "~ Checking Hits/Misses/Evictions"
  checkHitsMisses(parsed, traceFile)

  print "~ Checking Bus Shouts"
  checkBusShouts(parsed, traceFile)

  clean()


if __name__ == '__main__':
  main()
