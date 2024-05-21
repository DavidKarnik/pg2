# Počítačová Grafika 2

Předmět: PG2 2024 LS, Technická univerzita v Liberci

##

Wir haben die ganze verdammte Spiel-Engine erstellt

##

Vytvoření 3D hry pomocí OpenGL

# 3D Graphics Project

**START = 100 points**

## ESSENTIALS:
✅ 3D GL Core profile + shaders at least 4.5, GL debug
✅ High performance => at least 60 FPS
✅ Allow VSync control, fullscreen vs. windowed switching (restore window position & size)
✅ Event processing (camera, object, app behavior...): mouse (both axes, wheel), keyboard
✅ Multiple different independently moving 3D models, at least two loaded from file
✅ At least three different textures (or subtextures from texture atlas etc.)
✅ Lighting model, at least 3 lights (1x ambient, 1x directional, 1x reflector: at least one is moving; + possible other lights)
✅ Correct full scale transparency (at least one transparent object; NOT if(alpha<0.1) {discard;})

## EXTRAS:
✅ Height map textured by height & proper player height coords
✅ Working collisions
- [ ] Particles
- [ ] Scripting (useful)
✅ Audio (better than just background)
✅ Some other nice complicated effect...

## INSTAFAIL:
- Using GLUT, GL compatible profile

*Note: Hardware limitation might apply (e.g., no mouse wheel on notebook), in that case subtask can be ignored.*

## Points System:
- Each missing (non-functional) Essential = -25 points (partial functionality => partial decrement)
- Each working Extra = +10 points

### Points  
- **A = "1"**     91 - 100
- **B = "1-"**    81 - 90
- **C = "2"**     71 - 80
- **D = "2-"**    61 - 70
- **E = "3"**     51 - 60
- **F = "4"**     0 - 50

#----------------------------------------------------------------------------------------------------#
# Send full project and installation procedures in advance (use gitlab.tul.cz, github, gitlab, etc.) #
#----------------------------------------------------------------------------------------------------#
