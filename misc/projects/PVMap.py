#!/usr/bin/python

array = [0] * 43

Top         = 1
Bottom      = 2
Far         = 4
Near        = 8
Left        = 16
Right       = 32

#Edge map
array[Top + Far]        = 0
array[Top + Near]       = 1
array[Top + Left]       = 2
array[Top + Right]      = 3
array[Bottom + Far]     = 4
array[Bottom + Near]    = 5
array[Bottom + Left]    = 6
array[Bottom + Right]   = 7
array[Far + Left]       = 8
array[Far + Right]      = 9
array[Near + Left]      = 10
array[Near+ Right]      = 11

#Vertex map
array[Top + Far + Left]         = 0
array[Top + Far + Right]        = 1
array[Top + Near + Left]        = 2
array[Top + Near + Right]       = 3
array[Bottom + Far + Left]      = 4
array[Bottom + Far + Right]     = 5
array[Bottom + Near + Left]     = 6
array[Bottom + Near + Right]    = 7

print(array)
