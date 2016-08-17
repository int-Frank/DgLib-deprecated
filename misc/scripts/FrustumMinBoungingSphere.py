#!/usr/bin/python

import turtle
import math
import random
import tkinter as tk

def DrawCircle(x, y, radius):
    turtle.penup()
    turtle.goto(x + radius, y)
    turtle.setheading(90)
    turtle.pendown()
    turtle.circle(radius)
    turtle.penup

def Run():
    #bounds
    nearRange = [0, 50]
    farRange = [50, 200]
    frusHL = 100

    #Logic
    nearDist = random.uniform(nearRange[0], nearRange[1])
    farDist = random.uniform(farRange[0], farRange[1])
    d = frusHL * 2
    an = nearDist
    af = farDist
    b = (d*d + af*af - an*an) / (2 * d)
    radius = math.sqrt(b*b + an*an)
    originY = -frusHL + b

    #text.insert('end', 'Origin: %d\n' % originY)

    #Render
    turtle.clear()
    turtle.hideturtle()
    turtle.tracer(0, 0)
    turtle.penup()
    turtle.goto(-farDist, frusHL)
    turtle.pendown()
    turtle.goto(-nearDist, -frusHL)
    turtle.goto(nearDist, -frusHL)
    turtle.goto(farDist, frusHL)
    turtle.goto(-farDist, frusHL)
    turtle.penup()
    DrawCircle(0, originY, radius);
    turtle.update()


def onKeyPress(event):
    Run()

root = tk.Tk()
root.geometry('300x200')
text = tk.Text(root, background='black', foreground='white', font=('Comic Sans MS', 12))
text.pack()
root.bind('<KeyPress>', onKeyPress)
Run()
root.mainloop()
