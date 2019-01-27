# Mandelbrot Zoom

usage: Mandelbrot.exe gpu | cpu

Unter [Releases](https://github.com/addap/mandelbrot/releases) kann man sich die neueste Version runterladen
Im Shaders/ Ordner sind die shader des gpu modus definiert. Dort kann man zb die Farben anpassen, indem man die Zuweisung der fragColor Variable verändert. Ein Beispiel ist in der Datei escape-algorithm-colors.glsl, fragColor ist ein 4-Tupel aus RGBA wobei die Werte floats zwischen 0 und 1 sind.

gpu:
 * berechnet den escape time algorithmus auf der grafikkarte
 * mit linksclick neu zentrieren
 * mit pfeiltasten hoch/runter die geschwindigkeit anpassen

cpu:
 * berechnet den escape time algorithmus auf dem normalen prozessor
 * mit der Maus kann man ein Rechteck zeichnen um zu zoomen

