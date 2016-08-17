#!/usr/bin/python

array = [0] * 43

Top         = 1
Bottom      = 2
Far         = 4
Near        = 8
Left        = 16
Right       = 32

scopeName = 'FDataConstants::'

#Edge map
array[Top + Far]        = 'TopFar'
array[Top + Near]       = 'TopNear'
array[Top + Left]       = 'FTopLeft'
array[Top + Right]      = 'TopRight'
array[Bottom + Far]     = 'BottomFar'
array[Bottom + Near]    = 'BottomNear'
array[Bottom + Left]    = 'BottomLeft'
array[Bottom + Right]   = 'BottomRight'
array[Far + Left]       = 'FarLeft'
array[Far + Right]      = 'FarRight'
array[Near + Left]      = 'NearLeft'
array[Near+ Right]      = 'NearRight'

#Vertex map
array[Top + Far + Left]         = 'TopFarLeft'
array[Top + Far + Right]        = 'TopFarRight'
array[Top + Near + Left]        = 'TopNearLeft'
array[Top + Near + Right]       = 'TopNearRight'
array[Bottom + Far + Left]      = 'BottomFarLeft'
array[Bottom + Far + Right]     = 'BottomFarRight'
array[Bottom + Near + Left]     = 'BottomNearLeft'
array[Bottom + Near + Right]    = 'BottomNearRight'

for e in array:
    if e == 0:
        print(str(e), end=', '),
    else:
        print('\n' + scopeName + str(e), end=', '),
