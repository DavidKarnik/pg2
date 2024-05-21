# Počítačová Grafika 2

Předmět: PG2 2024 LS, Technická univerzita v Liberci

##

Wir haben die ganze verdammte Spiel-Engine erstellt

##

Vytvoření 3D hry pomocí OpenGL

START = 100 points

ESSENTIALS:
* 3D GL Core profile + shaders at least 4.5, GL debug
* high performance => at least 60 FPS
* allow VSync control, fullscreen vs. windowed switching (restore window position & size)
* event processing (camera, object, app behaviour...): mouse (both axes, wheel), keyboard
* multiple different independently moving 3D models, at leats two loaded from file
* at least three different textures (or subtextures from texture atlas etc.)
* lighting model, at least 3 lights (1x ambient, 1x directional, 1x reflector: at least one is moving; +possible other lights)
* correct full scale transparency (at least one transparent object; NOT if(alpha<0.1) {discard;} )

EXTRAS:
* height map textured by height & proper player height coords
* working collisions
* particles
* scripting (useful)
* audio (better than just background)
* some other nice complicated effect...  

INSTAFAIL:
* using GLUT, GL compatible profile

NOTE: Hardware limitation might apply (eg. no mouse wheel on notebook), in that case subtask can be ignored.


Each missing (non-functional) Essential = -25 points (partial functionality => partial decrement)
Each working Extra = +10 points.

            Points  
A = "1"     91  100
B = "1-"    81  90
C = "2"     71  80
D = "2-"    61  70
E = "3"     51  60
F = "4"     0   50

#----------------------------------------------------------------------------------------------------#
# Send full project and installation procedures in advance (use gitlab.tul.cz, github, gitlab, etc.) #
#----------------------------------------------------------------------------------------------------#

