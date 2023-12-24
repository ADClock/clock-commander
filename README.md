# Code for the clock commander

The clock commander receives the instruction as serial input and translates it to signals for the 24 clocks.

## How2wire
TODO

## Instruction set
TODO 

x[0-9][0-9]* - Repeat every following instruction n times before processing the next instruction
r[0-9][0-9]* - Process the next instructions and loop for n times when reaching the next r instruction
d[0-9][0-9]* - Set step delay in us
h - Move hour hand backwards
H - Move hour hand forwards
m - Move minute hand backwards
M - Move minute hand forwards
b - Move both hands backwards
B - Move both hands forwards
o - Move both hands in the opposite direction. Hour hand backwards, Minute hand forwards
O - Move both hands in the opposite direction. Hour hand forwards, Minute hand backwards
s - Do not move
c - Calibrate clock
. - Repeats the given instruction for the following clocks

## Examples
Calibrate all clocks
> c.

The first working example
> d1100 r600 b. r500 s. r10 x1 d1500 x100 O. x1 d1100 x200 M. r4 x1 d1500 x100 b. M. r0

Arrow to the left
> r854 hssssh. r427 shsshs. shOOhs. r854 hhoohh. r1707 B. r1 x500 s. r3 x200 ssOOss. ssooss. r0 d3000 x300 ssOOss. x1 d900 x300 ssooss. r0

