#!/usr/bin/python

from math import pow

ChildPerNode = 4
MaxNodes = 0x7FFFFFFF
currentNodeCount = 0
currentLevel = 0

while True:
    newNodes = pow(2, currentLevel) * 4
    if (currentNodeCount + newNodes > MaxNodes):
        break
    currentNodeCount = currentNodeCount + newNodes
    currentLevel += 1

print('Max depth: %i', currentLevel - 1)
print('Max nodes: %i', currentNodeCount)
print('Unused:    %i', (MaxNodes - currentNodeCount))
