import copy
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
from math import *
import random
import numpy as np

cameraTransX = 0.0
cameraTransY = 0.0
cameraTransZ = 0.0
cameraAngleX = 0.0
cameraAngleY = 0.0
cameraAngleZ = 0.0

particles = []

def is_pos(n):
    if n > 0:
        return 1
    else:
        return -1

class Particle:
    def __init__(self, pos, velocity, size, color, lifetime):
        self.pos = pos
        self.in_pos = copy.deepcopy(pos)
        self.velocity = velocity
        self.size = size
        self.color = color
        self.lifetime = lifetime
        self.in_lifetime = copy.deepcopy(lifetime)
        self.trail = None
        self.collided = False
        self.x = random.uniform(-0.01, 0.01)
        self.y = random.uniform(-0.01, 0.01)
        self.z = random.uniform(-0.01, 0.01)

    def is_dead(self):
        return self.lifetime <= 0

    def update(self, dtime):
        if self.trail == None:
            self.trail = ParticleTrail()
        self.trail.add_pos(self.pos)
        self.pos[0] += is_pos(self.velocity[0]) * dtime + self.x
        self.pos[1] += is_pos(self.velocity[1]) * dtime + self.y
        self.pos[2] += self.velocity[2] * dtime + self.z
        self.lifetime -= dtime
        self.color[3] -= dtime / self.in_lifetime
        init_distance = sqrt((self.pos[0] - self.in_pos[0]) ** 2 +
                             (self.pos[1] - self.in_pos[1]) ** 2 +
                             (self.pos[2] - self.in_pos[2]) ** 2)

    def draw(self):
        glColor4f(*self.color[:4])
        glPointSize(self.size)
        glBegin(GL_POINTS)
        glVertex3fv(self.pos)
        glEnd()

    def draw_trail(self):
        self.trail.draw(self.color)

    def check_collision_with_cube(self, cube):
        angle_rad = radians(cube.angle)

        rotated_x = (self.x * cos(angle_rad) -
                     self.y * sin(angle_rad))
        rotated_y = (self.x * sin(angle_rad) +
                     self.y * cos(angle_rad))

        x = rotated_x + cube.pos[0]
        y = rotated_y + cube.pos[1]
        z = self.z + cube.pos[2]

        if (
                abs(x - self.pos[0]) < cube.size / 2 + self.size / 2 and
                abs(y - self.pos[1]) < cube.size / 2 + self.size / 2 and
                abs(z - self.pos[2]) < cube.size / 2 + self.size / 2 and
                self.collided == False
        ):
            self.collided = True
            self.velocity[0] *= -1
            self.velocity[2] *= -1

class ParticleTrail:
    def __init__(self):
        self.length = random.randint(4, 6)
        self.poses = []

    def add_pos(self, pos):
        if len(self.poses) > self.length:
            self.poses.pop(0)
        self.poses.append(pos.copy())

    def draw(self, color):
        glColor4f(*color[:4])
        glBegin(GL_POINTS)
        for pos in self.poses:
            glVertex3fv(pos)
        glEnd()

class Cube:
    def __init__(self, size, pos, angle, color):
        self.size = size
        self.pos = pos
        self.angle = angle
        self.color = color

    def draw(self):
        glColor3f(*self.color[:3])
        glPushMatrix()
        glTranslatef(*self.pos[:3])
        glRotatef(self.angle, 0.0, 0.0, 1.0)
        glutSolidCube(self.size)
        glPopMatrix()

class Emitter:
    def __init__(self, size, pos, color):
        self.size = size
        self.pos = pos
        self.color = color

    def get_random_point(self, sphere_pos, radius):
        phi = random.uniform(0, pi)
        theta = random.uniform(0, pi * 2)

        x = sphere_pos[0] + radius * sin(phi) * cos(theta)
        y = sphere_pos[1] + radius * sin(phi) * sin(theta)
        z = sphere_pos[2] + radius * cos(phi)

        return [x, y, z]

    def get_direction_from_emitter(self, sphere_pos, radius):
        direction = [self.pos[0] - sphere_pos[0],
                     self.pos[1] - sphere_pos[1],
                     self.pos[2] - sphere_pos[2]]
        length = sqrt(direction[0] ** 2 + direction[1] ** 2 + direction[2] ** 2)
        direction = [d / length for d in direction]

        return direction

    def draw(self):
        glColor3f(*self.color[:3])
        glPushMatrix()
        glTranslatef(*self.pos[:3])
        glutSolidSphere(self.size, 20, 20)
        glPopMatrix()

def reshape(width, height):
    glViewport(0, 0, width, height)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(90, (width / height), 0.1, 100.0)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()
    gluLookAt(0, 15, 50,
              0, 0, 0,
              0, 1, 0)

def display(cube, emitter, particles_to_emit, FPS):
    dtime = 1 / FPS
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    glPushMatrix()
    glTranslatef(cameraTransX, cameraTransY, -cameraTransZ)
    glRotatef(cameraAngleX, 0.0, 1.0, 0.0)
    glRotatef(cameraAngleY, 1.0, 0.0, 0.0)
    glRotatef(cameraAngleZ, 0.0, 0.0, 1.0)

    for particle in particles:
        particle.check_collision_with_cube(cube)
        particle.update(dtime)
        if particle.is_dead():
            particles.remove(particle)

    if len(particles) < particles_to_emit:
        for _ in range(particles_to_emit):
            pos = emitter.get_random_point(emitter.pos, emitter.size)
            direction = emitter.get_direction_from_emitter(pos, emitter.size)
            velocity = [direction[0], direction[1], direction[2]]
            size = 5
            color = [random.uniform(0, 1), random.uniform(0, 1), random.uniform(0, 1), 1.0]
            lifetime = random.uniform(30, 60)

            particle = Particle(pos, velocity, size, color, lifetime)
            particles.append(particle)

    cube.draw()
    emitter.draw()

    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    for particle in particles:
        particle.draw()
        if particle.trail is None:
            continue
        particle.draw_trail()
    glDisable(GL_BLEND)

    glPopMatrix()
    glutSwapBuffers()
    glutPostRedisplay()

def keyboard(key, x, y):
    global cameraTransX, cameraTransY, cameraTransZ, cameraAngleX, cameraAngleY, cameraAngleZ

    # camera translation
    if key == b'1':
        cameraTransX += 1.0
    if key == b'2':
        cameraTransX -= 1.0
    if key == b'3':
        cameraTransY -= 1.0
    if key == b'4':
        cameraTransY += 1.0
    if key == b'5':
        cameraTransZ -= 1.0
    if key == b'6':
        cameraTransZ += 1.0

    # camera rotation
    if key in (b'Q', b'q'):
        cameraAngleX += 1.0
    if key in (b'W', b'w'):
        cameraAngleX -= 1.0
    if key in (b'E', b'e'):
        cameraAngleY -= 1.0
    if key in (b'R', b'r'):
        cameraAngleY += 1.0
    if key in (b'T', b't'):
        cameraAngleZ -= 1.0
    if key in (b'Y', b'y'):
        cameraAngleZ += 1.0

def main():
    particles_to_emit = 50

    cube_size = 20.0
    sphere_radius = 10.0
    FPS = 60

    cube = Cube(cube_size, [-20.0, 0.0, 0.0], 45, [0.0, 0.0, 1.0])
    emitter = Emitter(sphere_radius, [20.0, 0.0, 0.0], [0.0, 1.0, 0.0])

    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH)
    glutCreateWindow(b"Coursework 51")
    glutInitWindowPosition(50, 100)
    glutReshapeWindow(800, 600)
    glEnable(GL_DEPTH_TEST)
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glClearColor(0.0, 0.0, 0.0, 1.0)

    glutDisplayFunc(lambda: display(cube, emitter, particles_to_emit, FPS))
    glutKeyboardFunc(keyboard)
    glutReshapeFunc(reshape)
    glutMainLoop()

if __name__ == "__main__":
    main()