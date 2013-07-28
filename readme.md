__sw__ is a very simple stopwatch that work in commandline
Note that doesnt suport interactive mode

README
======

Compilation
-----------

`cc -Wall -Wextra -O2 sw.c -o sw -lncurses`

Usage
-----

sw [-t_time passed_] [-s_task passed_]
sw [-t _time passed_] [-s _task passed_]
sw [--tiempo=_time passed_] [--tarea=_task passed_]
sw [--tiempo _time passed_] [--tarea _task passed_]
sw [default 20 minutes]
