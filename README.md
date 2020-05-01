# RTPlot

RTPlots plots tasks for you. You specify the task-graph using a basic file. It is really easy to use. 

### Input 

A text file with the following format is parsed: `{X, [(Y, Z), ...]}`. The arguments are:
**X**: This is just a number (in text) like 112235543. It is interpreted as a millisecond counter with type (`unsigned long long`) internally. 
**Y**: This is the name of the task. It can't be longer than 32 characters. It's just an unquoted string like A or Foo. 
**Z**: This is a text 0 or 1. 0 means to draw the task as "off", 1 means to draw it as "on". 

---

Here's an example of input:
```
{0, [(A, 1)]}
{20, [(B, 1), (C, 1)]}
{40, [(A, 0)]}
{60, [(B, 0)]}
{61, [(C, 0)]}
{65, [(D, 1),(E,1),(F,1)]}
{71, [(C,1)]}
{80, [(D,0), (E,0)]}
{85, [(F,0)]}
```
It produces the following graph:

![Plotted graph](https://i.imgur.com/pNWEl1j.png)

---

### Installation

Just clone this project and then open it in QTCreator. I did this all on Ubuntu 18.04 Eoan. 

**Q**: Where do I specify the input file? 
**A**: You can do that in the Project Settings. As you can see here, it's called "data.log". **The file you want to plot needs to be in the directory the program executes. In my version, it executed it NOT from rtplot folder, but in folder "build-rtplot-Desktop-Debug" which it created at the same level as it. 

![Settings](https://i.imgur.com/6J5TQcC.png)
