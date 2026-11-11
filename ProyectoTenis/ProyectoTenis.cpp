// Proyecto Final - Escenario Deportivo 3D Interactivo (Tenis)
// Computacion Grafica e Interaccion Humano-Computadora
// Facultad de Ingenieria, UNAM - Laboratorio, grupo 09
//
// Convenciones de la escena:
//   - 1 unidad = 1 metro. El eje Y es hacia arriba y el piso esta en y = 0.
//   - La red esta en z = 0 y la cancha se extiende a lo largo del eje Z (23.77 m de largo, 10.97 m de ancho).
//   - Las bancas y la silla del arbitro estan del lado +X; el publico, a ambos lados en X.
//   - Cada modelo (.obj) se dibuja con la matriz "model" que se arma antes de llamar a Draw().
//   - Todas las animaciones avanzan por cuadro (no por deltaTime), igual que en la practica 10.

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 1.7f, 15.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Positions of the point lights (los 4 reflectores, uno en cada torre)
glm::vec3 pointLightPositions[] = {
	glm::vec3(15.7f, 10.8f, 20.2f),
	glm::vec3(-15.7f, 10.8f, 20.2f),
	glm::vec3(15.7f, 10.8f, -20.2f),
	glm::vec3(-15.7f, 10.8f, -20.2f)
};

// Posiciones de las torres de reflectores
glm::vec3 torrePos[] = {
	glm::vec3(16.5f, 0.0f, 21.0f),
	glm::vec3(-16.5f, 0.0f, 21.0f),
	glm::vec3(16.5f, 0.0f, -21.0f),
	glm::vec3(-16.5f, 0.0f, -21.0f)
};

// Reflectores encendidos (tecla F)
bool lucesOn = false;

// Dia o noche (tecla N)
bool esDia = true;

// Puntos del jugador 1: 0, 15, 30, 40 y JUEGO (tecla M)
int puntos = 0;

// Linterna: luz spot que sale de la camara y apunta hacia donde mira (tecla O)
bool linternaOn = false;

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// Figura humana articulada (modelado jerarquico):
// cadera -> torso -> cabeza y brazos (hombro -> codo), cadera -> piernas (cadera -> rodilla)
struct Figura
{
	Model *torso;
	Model *cabeza;
	Model *brazo;
	Model *antebrazo;
	Model *muslo;
	Model *pantorrilla;
};

// Posicion y angulos (en grados) de cada articulacion de una figura
struct Pose
{
	float x, y, z, rotY;                        // posicion (pies sobre el piso) y orientacion
	float inclTorso, rotTorsoY;                 // torso
	float brDer, abDer, brIzq, abIzq;           // brazos: hombro y codo
	float pierDer, rodDer, pierIzq, rodIzq;     // piernas: cadera y rodilla
};

void DibujarFigura(Figura &f, Shader &shader, Pose &p, Model *raqueta)
{
	GLint modelLoc = glGetUniformLocation(shader.Program, "model");
	glm::vec3 X(1.0f, 0.0f, 0.0f), Y(0.0f, 1.0f, 0.0f);
	glm::mat4 base, cadera, articulacion, model;

	// Origen del cuerpo: los pies sobre el piso y mirando hacia +Z antes de rotar
	base = glm::translate(glm::mat4(1), glm::vec3(p.x, p.y, p.z));
	base = glm::rotate(base, glm::radians(p.rotY), Y);

	// Pierna derecha: muslo y pantorrilla
	articulacion = glm::translate(base, glm::vec3(-0.1f, 0.9f, 0.0f));
	articulacion = glm::rotate(articulacion, glm::radians(p.pierDer), X);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(articulacion));
	f.muslo->Draw(shader);
	articulacion = glm::translate(articulacion, glm::vec3(0.0f, -0.42f, 0.0f));
	articulacion = glm::rotate(articulacion, glm::radians(p.rodDer), X);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(articulacion));
	f.pantorrilla->Draw(shader);

	// Pierna izquierda
	articulacion = glm::translate(base, glm::vec3(0.1f, 0.9f, 0.0f));
	articulacion = glm::rotate(articulacion, glm::radians(p.pierIzq), X);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(articulacion));
	f.muslo->Draw(shader);
	articulacion = glm::translate(articulacion, glm::vec3(0.0f, -0.42f, 0.0f));
	articulacion = glm::rotate(articulacion, glm::radians(p.rodIzq), X);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(articulacion));
	f.pantorrilla->Draw(shader);

	// Torso (de aqui cuelgan la cabeza y los brazos)
	cadera = glm::translate(base, glm::vec3(0.0f, 0.9f, 0.0f));
	cadera = glm::rotate(cadera, glm::radians(p.inclTorso), X);
	cadera = glm::rotate(cadera, glm::radians(p.rotTorsoY), Y);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cadera));
	f.torso->Draw(shader);

	// Cabeza
	model = glm::translate(cadera, glm::vec3(0.0f, 0.6f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	f.cabeza->Draw(shader);

	// Brazo derecho: brazo y antebrazo
	articulacion = glm::translate(cadera, glm::vec3(-0.26f, 0.54f, 0.0f));
	articulacion = glm::rotate(articulacion, glm::radians(p.brDer), X);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(articulacion));
	f.brazo->Draw(shader);
	articulacion = glm::translate(articulacion, glm::vec3(0.0f, -0.3f, 0.0f));
	articulacion = glm::rotate(articulacion, glm::radians(p.abDer), X);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(articulacion));
	f.antebrazo->Draw(shader);

	// Brazo izquierdo
	model = glm::translate(cadera, glm::vec3(0.26f, 0.54f, 0.0f));
	model = glm::rotate(model, glm::radians(p.brIzq), X);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	f.brazo->Draw(shader);
	model = glm::translate(model, glm::vec3(0.0f, -0.3f, 0.0f));
	model = glm::rotate(model, glm::radians(p.abIzq), X);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	f.antebrazo->Draw(shader);

	// Raqueta en la mano derecha
	if (raqueta != nullptr)
	{
		model = glm::translate(articulacion, glm::vec3(0.0f, -0.31f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		raqueta->Draw(shader);
	}
}

// Jugador 1 (el que saca): empieza detras de la linea de fondo, mirando hacia -Z
Pose pJug = { 1.8f, 0.0f, 12.6f, 180.0f,   0.0f, 0.0f,   0.0f, 0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f, 0.0f };

// Pelota (empieza en la mano izquierda del jugador)
float pelX = 1.54f, pelY = 0.83f, pelZ = 12.6f;

// Oponente (del otro lado de la red, mirando hacia +Z, con la raqueta lista)
Pose pOpp = { -1.5f, 0.0f, -13.2f, 0.0f,   10.0f, 0.0f,   -35.0f, -15.0f, -20.0f, -10.0f,   0.0f, 0.0f, 0.0f, 0.0f };

// Arbitro sentado en la silla (muslos al frente y rodillas dobladas)
Pose pArb = { 8.4f, 1.05f, 0.0f, -90.0f,   0.0f, 0.0f,   -40.0f, -45.0f, -40.0f, -45.0f,   -90.0f, 90.0f, -90.0f, 90.0f };

// KeyFrames: animacion del servicio (misma estructura que la practica 10)
#define MAX_FRAMES 9
int i_max_steps = 70;
int i_curr_steps = 0;
typedef struct _frame {
	// Valores en el frame
	float x, y, z;                              // posicion del jugador
	float brDer, abDer, brIzq, inclTorso;       // brazos y torso del jugador
	float pelX, pelY, pelZ;                     // posicion de la pelota
	// Incrementos por paso
	float incX, incY, incZ;
	float incBrDer, incAbDer, incBrIzq, incInclTorso;
	float incPelX, incPelY, incPelZ;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = MAX_FRAMES;
bool play = false;
int playIndex = 0;

// Guarda los valores de un frame
void frame(int i, float x, float y, float z, float brDer, float abDer, float brIzq, float inclTorso, float pelX, float pelY, float pelZ)
{
	KeyFrame[i].x = x;
	KeyFrame[i].y = y;
	KeyFrame[i].z = z;
	KeyFrame[i].brDer = brDer;
	KeyFrame[i].abDer = abDer;
	KeyFrame[i].brIzq = brIzq;
	KeyFrame[i].inclTorso = inclTorso;
	KeyFrame[i].pelX = pelX;
	KeyFrame[i].pelY = pelY;
	KeyFrame[i].pelZ = pelZ;
}

// Los 9 frames del servicio: preparacion, lanzamiento de la pelota, golpe, seguimiento y bote en el cuadro de saque
void cargarServicio(void)
{
	//     frame   x       y       z       brDer     abDer     brIzq     inclTorso   pelota (x, y, z)
	frame(0, 1.8f, 0.0f, 12.6f,   0.0f, 0.0f, 0.0f, 0.0f,   1.54f, 0.83f, 12.6f);
	frame(1, 1.8f, 0.0f, 12.6f,   25.0f, 15.0f, -70.0f, -5.0f,   1.54f, 1.28f, 12.058f);
	frame(2, 1.8f, 0.0f, 12.5f,   -50.0f, 80.0f, -165.0f, -12.0f,   1.54f, 2.587f, 12.58f);
	frame(3, 1.8f, 0.0f, 12.4f,   -100.0f, 105.0f, -150.0f, -15.0f,   2.06f, 3.074f, 11.923f);
	frame(4, 1.8f, 0.15f, 12.3f,   -170.0f, 0.0f, -60.0f, 8.0f,   2.06f, 2.574f, 11.903f);
	frame(5, 1.8f, 0.0f, 11.9f,   -40.0f, 20.0f, 10.0f, 25.0f,   -0.66f, 1.45f, 0.0f);
	frame(6, 1.8f, 0.0f, 11.9f,   -10.0f, 10.0f, 5.0f, 10.0f,   -2.0f, 0.08f, -5.9f);
	frame(7, 1.8f, 0.0f, 11.9f,   0.0f, 0.0f, 0.0f, 0.0f,   -2.5f, 1.0f, -9.0f);
	frame(8, 1.8f, 0.0f, 11.9f,   0.0f, 0.0f, 0.0f, 0.0f,   -2.9f, 0.08f, -12.8f);
}

void resetElements(void)
{
	pJug.x = KeyFrame[0].x;
	pJug.y = KeyFrame[0].y;
	pJug.z = KeyFrame[0].z;
	pJug.rotY = 180.0f;
	pJug.brDer = KeyFrame[0].brDer;
	pJug.abDer = KeyFrame[0].abDer;
	pJug.brIzq = KeyFrame[0].brIzq;
	pJug.inclTorso = KeyFrame[0].inclTorso;
	// Piernas rectas al iniciar el servicio (por si el jugador venia caminando)
	pJug.pierDer = 0.0f;
	pJug.pierIzq = 0.0f;
	pJug.rodDer = 0.0f;
	pJug.rodIzq = 0.0f;
	pelX = KeyFrame[0].pelX;
	pelY = KeyFrame[0].pelY;
	pelZ = KeyFrame[0].pelZ;
}

void interpolation(void)
{
	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].x - KeyFrame[playIndex].x) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].y - KeyFrame[playIndex].y) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].z - KeyFrame[playIndex].z) / i_max_steps;
	KeyFrame[playIndex].incBrDer = (KeyFrame[playIndex + 1].brDer - KeyFrame[playIndex].brDer) / i_max_steps;
	KeyFrame[playIndex].incAbDer = (KeyFrame[playIndex + 1].abDer - KeyFrame[playIndex].abDer) / i_max_steps;
	KeyFrame[playIndex].incBrIzq = (KeyFrame[playIndex + 1].brIzq - KeyFrame[playIndex].brIzq) / i_max_steps;
	KeyFrame[playIndex].incInclTorso = (KeyFrame[playIndex + 1].inclTorso - KeyFrame[playIndex].inclTorso) / i_max_steps;
	KeyFrame[playIndex].incPelX = (KeyFrame[playIndex + 1].pelX - KeyFrame[playIndex].pelX) / i_max_steps;
	KeyFrame[playIndex].incPelY = (KeyFrame[playIndex + 1].pelY - KeyFrame[playIndex].pelY) / i_max_steps;
	KeyFrame[playIndex].incPelZ = (KeyFrame[playIndex + 1].pelZ - KeyFrame[playIndex].pelZ) / i_max_steps;
}

// Caminata del jugador
bool caminando = false;
float swing = 0.0f;         // angulo de las piernas al caminar
bool pasoAdelante = true;

// Camara activa: 0 primera persona, 1 transmision (orbital), 2 tercera persona (tecla C)
int modoCamara = 0;
float orbitaAng = 0.0f;
float orbitaDist = 32.0f;
float orbitaAlt = 14.0f;

// Animaciones basicas por transformaciones
float rotPelota = 0.0f;     // rotacion de la pelota
float swayOpp = 0.0f;       // balanceo del oponente
bool swayDir = true;
float pulso = 1.0f;         // escala del marcador al sumar un punto

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final - Tenis 3D", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	// Models (se cargan una sola vez, antes del ciclo principal)
	// Piso: pasto de 200 x 200 m. Cancha: modelo completo con red, postes, bancas y silla del arbitro
	Model Piso((char*)"Models/piso.obj");
	Model Cancha((char*)"Models/cancha_tenis.obj");
	Model Gradas((char*)"Models/gradas.obj");
	Model Tablero1((char*)"Models/tablero_1.obj");
	Model Tablero2((char*)"Models/tablero_2.obj");
	Model Tablero3((char*)"Models/tablero_3.obj");
	Model *Tableros[3] = { &Tablero1, &Tablero2, &Tablero3 };
	Model Torre((char*)"Models/torre.obj");

	// Marcadores (uno por cada puntuacion)
	Model Marcador0((char*)"Models/marcador_0.obj");
	Model Marcador1((char*)"Models/marcador_1.obj");
	Model Marcador2((char*)"Models/marcador_2.obj");
	Model Marcador3((char*)"Models/marcador_3.obj");
	Model Marcador4((char*)"Models/marcador_4.obj");
	Model *Marcadores[5] = { &Marcador0, &Marcador1, &Marcador2, &Marcador3, &Marcador4 };

	// Jugador 1 (figura articulada)
	Model J1Torso((char*)"Models/j1_torso.obj");
	Model J1Cabeza((char*)"Models/j1_cabeza.obj");
	Model J1Brazo((char*)"Models/j1_brazo.obj");
	Model J1Antebrazo((char*)"Models/j1_antebrazo.obj");
	Model J1Muslo((char*)"Models/j1_muslo.obj");
	Model J1Pantorrilla((char*)"Models/j1_pantorrilla.obj");
	Figura J1 = { &J1Torso, &J1Cabeza, &J1Brazo, &J1Antebrazo, &J1Muslo, &J1Pantorrilla };

	// Raqueta y pelota
	Model Raqueta((char*)"Models/raqueta.obj");
	Model Pelota((char*)"Models/pelota.obj");

	// Oponente
	Model J2Torso((char*)"Models/j2_torso.obj");
	Model J2Cabeza((char*)"Models/j2_cabeza.obj");
	Model J2Brazo((char*)"Models/j2_brazo.obj");
	Model J2Antebrazo((char*)"Models/j2_antebrazo.obj");
	Model J2Muslo((char*)"Models/j2_muslo.obj");
	Model J2Pantorrilla((char*)"Models/j2_pantorrilla.obj");
	Figura J2 = { &J2Torso, &J2Cabeza, &J2Brazo, &J2Antebrazo, &J2Muslo, &J2Pantorrilla };

	// Arbitro
	Model ArbTorso((char*)"Models/arb_torso.obj");
	Model ArbCabeza((char*)"Models/arb_cabeza.obj");
	Model ArbBrazo((char*)"Models/arb_brazo.obj");
	Model ArbAntebrazo((char*)"Models/arb_antebrazo.obj");
	Model ArbMuslo((char*)"Models/arb_muslo.obj");
	Model ArbPantorrilla((char*)"Models/arb_pantorrilla.obj");
	Figura Arb = { &ArbTorso, &ArbCabeza, &ArbBrazo, &ArbAntebrazo, &ArbMuslo, &ArbPantorrilla };

	// Cubo para dibujar las lamparas (VAO y VBO como en las practicas)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 300.0f);

	// Frames de la animacion del servicio
	cargarServicio();

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		if (esDia)
			glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
		else
			glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		// Camara activa: 0 primera persona, 1 transmision (orbital), 2 tercera persona
		glm::mat4 view;
		glm::vec3 posCam = camera.GetPosition();
		glm::vec3 frenteCam = camera.GetFront();     // hacia donde apunta la linterna
		if (modoCamara == 0)
		{
			view = camera.GetViewMatrix();
		}
		else if (modoCamara == 1)
		{
			posCam = glm::vec3(orbitaDist * sin(glm::radians(orbitaAng)), orbitaAlt, orbitaDist * cos(glm::radians(orbitaAng)));
			view = glm::lookAt(posCam, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			frenteCam = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f) - posCam);
		}
		else
		{
			glm::vec3 adelante(sin(glm::radians(pJug.rotY)), 0.0f, cos(glm::radians(pJug.rotY)));
			glm::vec3 jugador(pJug.x, pJug.y, pJug.z);
			posCam = jugador - adelante * 4.5f + glm::vec3(0.0f, 2.4f, 0.0f);
			view = glm::lookAt(posCam, jugador + glm::vec3(0.0f, 1.2f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			frenteCam = glm::normalize(jugador + glm::vec3(0.0f, 1.2f, 0.0f) - posCam);
		}

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, posCam.x, posCam.y, posCam.z);

		// Directional light (luz del dia; de noche casi no hay luz)
		float luzDia = esDia ? 1.0f : 0.08f;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f * luzDia, 0.6f * luzDia, 0.6f * luzDia);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f * luzDia, 0.6f * luzDia, 0.6f * luzDia);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f * luzDia, 0.3f * luzDia, 0.3f * luzDia);

		// Point lights: los 4 reflectores de las torres (tecla F los enciende y apaga)
		float luz = lucesOn ? 1.0f : 0.0f;
		for (int i = 0; i < 4; i++)
		{
			std::string luzI = "pointLights[" + std::to_string(i) + "]";
			glUniform3f(glGetUniformLocation(lightingShader.Program, (luzI + ".position").c_str()), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (luzI + ".ambient").c_str()), 0.15f * luz, 0.15f * luz, 0.13f * luz);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (luzI + ".diffuse").c_str()), 0.9f * luz, 0.9f * luz, 0.8f * luz);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (luzI + ".specular").c_str()), 0.5f * luz, 0.5f * luz, 0.45f * luz);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (luzI + ".constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (luzI + ".linear").c_str()), 0.022f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (luzI + ".quadratic").c_str()), 0.0019f);
		}

		// SpotLight: linterna de la camara (tecla O la enciende y la apaga)
		float luzLinterna = linternaOn ? 1.0f : 0.0f;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), posCam.x, posCam.y, posCam.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), frenteCam.x, frenteCam.y, frenteCam.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f * luzLinterna, 0.95f * luzLinterna, 0.8f * luzLinterna);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f * luzLinterna, 1.0f * luzLinterna, 1.0f * luzLinterna);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations (view se calculo arriba segun la camara activa)

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Piso y cancha (la cancha incluye red, postes, bancas y silla del arbitro)
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);
		Cancha.Draw(lightingShader);

		// Zona de espectadores: cada modulo mide 13.1 m de largo, asi que dos modulos cubren 26 m por lado.
		// El lado -X es el mismo modulo girado 180 grados sobre Y.
		for (int i = 0; i < 2; i++)
		{
			float z = -6.55f + 13.1f * i;

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(13.3f, 0.0f, z));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Gradas.Draw(lightingShader);

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-13.3f, 0.0f, -z));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Gradas.Draw(lightingShader);
		}

		// Carteles de publicidad: 9 en cada fondo y 9 en cada costado, rotados para mirar hacia la cancha.
		// Se alternan los tres anuncios (tablero_1, tablero_2 y tablero_3) con el operador % 3.
		for (int i = 0; i < 9; i++)
		{
			float x = -10.0f + 2.5f * i;

			// Fondo lejano
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(x, 0.0f, -17.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Tableros[i % 3]->Draw(lightingShader);

			// Fondo cercano
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(x, 0.0f, 17.2f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Tableros[(i + 1) % 3]->Draw(lightingShader);
		}
		for (int i = 0; i < 9; i++)
		{
			float z = -15.0f + 3.75f * i;

			// Lado izquierdo
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-11.9f, 0.0f, z));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Tableros[(i + 2) % 3]->Draw(lightingShader);

			// Lado derecho
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(11.9f, 0.0f, z));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Tableros[i % 3]->Draw(lightingShader);
		}

		// Torres de reflectores en las cuatro esquinas (miran hacia el centro de la cancha).
		// atan2 calcula el angulo que hay que girar cada torre sobre Y para encarar el origen.
		for (int i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, torrePos[i]);
			model = glm::rotate(model, atan2(-torrePos[i].x, -torrePos[i].z), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Torre.Draw(lightingShader);
		}

		// Marcador (muestra los puntos del jugador 1)
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 4.5f, -19.6f));
		model = glm::scale(model, glm::vec3(pulso));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Marcadores[puntos]->Draw(lightingShader);

		// Jugador 1 (modelado jerarquico)
		DibujarFigura(J1, lightingShader, pJug, &Raqueta);

		// Pelota
		model = glm::translate(glm::mat4(1), glm::vec3(pelX, pelY, pelZ));
		model = glm::rotate(model, glm::radians(rotPelota), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pelota.Draw(lightingShader);

		// Oponente y arbitro
		DibujarFigura(J2, lightingShader, pOpp, &Raqueta);
		DibujarFigura(Arb, lightingShader, pArb, nullptr);

		// Utileria: tres pelotas en el piso y una raqueta sobre una banca
		for (int i = 0; i < 3; i++)
		{
			model = glm::translate(glm::mat4(1), glm::vec3(-3.0f + 1.5f * i, 0.08f, 13.5f - 0.7f * i));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Pelota.Draw(lightingShader);
		}
		model = glm::translate(glm::mat4(1), glm::vec3(10.5f, 0.75f, 5.3f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Raqueta.Draw(lightingShader);

		// Dibujar las lamparas de los reflectores
		lampShader.Use();
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(VAO);
		for (int i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Controles del jugador (mismas teclas que el perro de la practica 10)
	caminando = false;
	if (!play)
	{
		if (keys[GLFW_KEY_Y])
		{
			pJug.z -= 0.05f;
			pJug.rotY = 180.0f;
			caminando = true;
		}
		if (keys[GLFW_KEY_H])
		{
			pJug.z += 0.05f;
			pJug.rotY = 0.0f;
			caminando = true;
		}
		if (keys[GLFW_KEY_G])
		{
			pJug.x -= 0.05f;
			pJug.rotY = -90.0f;
			caminando = true;
		}
		if (keys[GLFW_KEY_J])
		{
			pJug.x += 0.05f;
			pJug.rotY = 90.0f;
			caminando = true;
		}

		// Limites del area de juego
		if (pJug.x > 7.4f) pJug.x = 7.4f;
		if (pJug.x < -11.0f) pJug.x = -11.0f;
		if (pJug.z > 16.0f) pJug.z = 16.0f;
		if (pJug.z < 1.0f) pJug.z = 1.0f;       // la red esta en z = 0: el jugador no puede cruzarla
	}

	// Camara de transmision: Q y E la giran alrededor de la cancha, Z y X la acercan o la alejan
	if (modoCamara == 1)
	{
		if (keys[GLFW_KEY_Q])
			orbitaAng -= 0.8f;
		if (keys[GLFW_KEY_E])
			orbitaAng += 0.8f;
		if (keys[GLFW_KEY_Z] && orbitaDist > 12.0f)
			orbitaDist -= 0.2f;
		if (keys[GLFW_KEY_X] && orbitaDist < 60.0f)
			orbitaDist += 0.2f;
	}

	// Camera controls
	if (modoCamara == 0 && (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]))
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (modoCamara == 0 && (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]))
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (modoCamara == 0 && (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]))
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (modoCamara == 0 && (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]))
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	// La camara en primera persona no atraviesa el piso ni la red, y no sale del estadio
	if (modoCamara == 0)
	{
		glm::vec3 pc = camera.GetPosition();
		if (pc.y < 0.4f) pc.y = 0.4f;
		if (pc.y > 25.0f) pc.y = 25.0f;
		if (pc.x > 30.0f) pc.x = 30.0f;
		if (pc.x < -30.0f) pc.x = -30.0f;
		if (pc.z > 30.0f) pc.z = 30.0f;
		if (pc.z < -30.0f) pc.z = -30.0f;
		// Red: mide 1.04 m de alto y llega hasta los postes (|x| < 6.8)
		if (fabs(pc.z) < 0.3f && fabs(pc.x) < 6.8f && pc.y < 1.15f)
			pc.z = (pc.z >= 0.0f) ? 0.3f : -0.3f;
		camera.SetPosition(pc);
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// F: enciende o apaga los 4 reflectores de las torres
	if (GLFW_KEY_F == key && GLFW_PRESS == action)
	{
		lucesOn = !lucesOn;
	}

	// N: cambia entre dia y noche (luz direccional y color del fondo)
	if (GLFW_KEY_N == key && GLFW_PRESS == action)
	{
		esDia = !esDia;
	}

	// O: enciende o apaga la linterna (luz spot)
	if (GLFW_KEY_O == key && GLFW_PRESS == action)
	{
		linternaOn = !linternaOn;
	}

	// M: suma un punto al jugador 1 (0, 15, 30, 40 y JUEGO) y hace crecer el marcador un instante
	if (GLFW_KEY_M == key && GLFW_PRESS == action)
	{
		puntos = (puntos + 1) % 5;
		pulso = 1.25f;
	}

	// L: reproduce la animacion del servicio (como en la practica 10)
	if (GLFW_KEY_L == key && GLFW_PRESS == action)
	{
		if (play == false)
		{
			resetElements();
			//First Interpolation
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}
	}

	// C: cambia de camara (primera persona, transmision orbital y tercera persona)
	if (GLFW_KEY_C == key && GLFW_PRESS == action)
	{
		modoCamara = (modoCamara + 1) % 3;
		if (modoCamara == 0)
			std::cout << "Camara: primera persona (W A S D y mouse)" << std::endl;
		if (modoCamara == 1)
			std::cout << "Camara: transmision orbital (Q E giran, Z X acercan)" << std::endl;
		if (modoCamara == 2)
			std::cout << "Camara: tercera persona (sigue al jugador)" << std::endl;
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

void Animation()
{
	// Animaciones basicas: rotacion de la pelota, balanceo del oponente y escala del marcador
	rotPelota += 0.8f;
	if (swayDir)
	{
		swayOpp += 0.15f;
		if (swayOpp > 12.0f)
			swayDir = false;
	}
	else
	{
		swayOpp -= 0.15f;
		if (swayOpp < -12.0f)
			swayDir = true;
	}
	pOpp.rotTorsoY = swayOpp;
	if (pulso > 1.0f)
	{
		pulso -= 0.004f;
		if (pulso < 1.0f)
			pulso = 1.0f;
	}

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)    //end of total animation?
			{
				std::cout << "termina anim" << std::endl;
				playIndex = 0;
				play = false;

				// El servicio llego al otro lado de la red: es un punto para el jugador 1
				puntos = (puntos + 1) % 5;
				pulso = 1.25f;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			pJug.x += KeyFrame[playIndex].incX;
			pJug.y += KeyFrame[playIndex].incY;
			pJug.z += KeyFrame[playIndex].incZ;
			pJug.brDer += KeyFrame[playIndex].incBrDer;
			pJug.abDer += KeyFrame[playIndex].incAbDer;
			pJug.brIzq += KeyFrame[playIndex].incBrIzq;
			pJug.inclTorso += KeyFrame[playIndex].incInclTorso;
			pelX += KeyFrame[playIndex].incPelX;
			pelY += KeyFrame[playIndex].incPelY;
			pelZ += KeyFrame[playIndex].incPelZ;
			i_curr_steps++;
		}
	}
	else
	{
		// Antes de sacar, la pelota va en la mano izquierda del jugador
		float r = glm::radians(pJug.rotY);
		pelX = pJug.x + 0.26f * cos(r);
		pelY = pJug.y + 0.83f;
		pelZ = pJug.z - 0.26f * sin(r);

		// Caminata: las piernas y los brazos se balancean mientras el jugador avanza
		if (caminando)
		{
			if (pasoAdelante)
			{
				swing += 2.0f;
				if (swing > 30.0f)
					pasoAdelante = false;
			}
			else
			{
				swing -= 2.0f;
				if (swing < -30.0f)
					pasoAdelante = true;
			}
		}
		else
		{
			swing = 0.0f;
		}
		pJug.pierDer = swing;
		pJug.pierIzq = -swing;
		pJug.rodDer = (swing > 0.0f) ? swing * 1.2f : 0.0f;
		pJug.rodIzq = (swing < 0.0f) ? -swing * 1.2f : 0.0f;
		pJug.brDer = -swing * 0.8f;
		pJug.brIzq = swing * 0.8f;
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}
