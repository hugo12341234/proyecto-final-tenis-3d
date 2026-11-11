# Proyecto Final: Escenario Deportivo 3D Interactivo (Tenis)

Facultad de Ingeniería, UNAM · Laboratorio de Computación Gráfica e Interacción Humano-Computadora · Grupo 09 · Semestre 2027-1
Profesor: M.D.I. Edcel Fuerte Martínez

Escenario 3D interactivo de una cancha de tenis hecho con OpenGL moderno (3.3) en C++ (GLFW, GLEW, GLM, Assimp y SOIL2). Se desarrolló usando solo lo visto en las prácticas 0 a 10 del laboratorio: modelado jerárquico, carga de modelos, texturizado, iluminación, cámaras y animación.

## Equipo

- Aranda Marrón Fernando
- Gutiérrez Nolasco Emiliano
- Chacón Jaral Hugo Emmanuel

## Cómo compilar

1. Clonar el repositorio:
   ```
   git clone https://github.com/hugo12341234/proyecto-final-tenis-3d
   ```
2. Abrir la solución de la raíz: `ProyectoTenis.sln`.
3. En la barra superior elegir **Debug** y **x86** (las librerías son de 32 bits).
4. Presionar **F5** (o Ctrl + F5) para compilar y ejecutar.

**Requisitos:** Windows y Visual Studio con la carga de trabajo *Desarrollo para el escritorio con C++*. No hay que instalar ni configurar nada más: las librerías vienen en el repositorio.

## Controles

| Tecla | Acción |
|-------|--------|
| W A S D / flechas | Mover la cámara (primera persona) |
| Mouse | Mirar alrededor |
| C | Cambiar de cámara: primera persona, transmisión orbital y tercera persona |
| Q / E | Girar la cámara de transmisión alrededor de la cancha |
| Z / X | Acercar / alejar la cámara de transmisión |
| Y H G J | Mover al jugador (adelante, atrás, izquierda, derecha); camina con animación y no puede cruzar la red |
| L | Servicio del jugador (animación por keyframes). Al terminar suma un punto al marcador |
| F | Encender / apagar los 4 reflectores |
| N | Cambiar entre día y noche |
| O | Encender / apagar la linterna (luz spot que sigue a la cámara) |
| M | Sumar un punto en el marcador (0, 15, 30, 40, JUEGO) |
| Esc | Salir |

## Estructura del repositorio

```
External Libraries/    GLEW, GLFW, GLM, Assimp y SOIL2
ProyectoTenis.sln      solución de Visual Studio
ProyectoTenis/         proyecto: código, shaders, modelos, texturas y DLL
  ProyectoTenis.cpp    programa principal (todo el código del escenario)
  Shader.h Camera.h    clases de las prácticas (shaders y cámara)
  Model.h Mesh.h       carga de modelos con Assimp (de las prácticas)
  Shader/              lighting.vs/.frag (luces y texturas) y lamp.vs/.frag (lámparas)
  Models/              modelos .obj con sus .mtl y texturas
```

## Cómo está organizado el código

Todo el escenario está en `ProyectoTenis/ProyectoTenis.cpp`, con la misma estructura de las prácticas:

| Parte del código | Qué hace |
|------------------|----------|
| Variables globales | Cámara, luces, estado de las teclas, puntos del marcador, pose del jugador y keyframes |
| `main()` | Crea la ventana, carga shaders y modelos, y ejecuta el ciclo principal que dibuja cada cuadro |
| `DibujarFigura()` | Modelado jerárquico de una persona: cadera, torso, cabeza, brazos y piernas articulados |
| `Animation()` | Avanza los keyframes del servicio, la caminata y las animaciones por transformaciones |
| `interpolation()` y `cargarServicio()` | Keyframes del servicio (misma estructura de la práctica 10) |
| `DoMovement()` | Lee el teclado: cámara, jugador y cámara de transmisión |
| `KeyCallback()` y `MouseCallback()` | Teclas de un solo toque (F, N, M, C, L) y giro de la cámara con el mouse |

Convenciones de la escena: 1 unidad es 1 metro, el eje Y es hacia arriba, la red está en z = 0 y la cancha mide 23.77 × 10.97 m a lo largo del eje Z.

## Contenido del escenario

- **Terreno de juego:** cancha con las líneas reglamentarias, red y postes (modelo de Poly Pizza escalado a medidas reglamentarias).
- **Zona de bancas:** bancas de jugadores y silla del juez (con el árbitro sentado) incluidas en el modelo de la cancha.
- **Zona de espectadores:** gradas a ambos lados y carteles de publicidad alrededor de la cancha.
- **Ambientación:** cuatro torres de reflectores, marcador y utilería (pelotas y raqueta).
- **Personajes:** jugador, oponente y árbitro con modelado jerárquico (torso, cabeza, brazos y piernas articulados).

## Requisitos del proyecto

| Requisito | Dónde está |
|-----------|-----------|
| Modelado jerárquico | `DibujarFigura` en `ProyectoTenis.cpp` |
| Carga de modelos | Carpeta `Models/` (.obj, .mtl y texturas) |
| Perspectiva y 2 cámaras | Primera persona y transmisión orbital / tercera persona |
| Texturizado | Pasto, cancha, gradas, carteles, marcador y personajes |
| Iluminación | Luz direccional (día/noche), 4 luces puntuales (reflectores) y una luz spot (linterna) |
| Transformaciones | Caminata, rotación de la pelota, balanceo del oponente, escala del marcador |
| Keyframes | Servicio del jugador (tecla L) |
| Interacciones | Reflectores, día/noche, marcador, servicio |

## Problemas comunes

- **Marca que falta `MSVCP140D.dll`, `VCRUNTIME140D.dll` o `ucrtbased.dll`:** `assimp` usa las bibliotecas de depuración de Visual C++. Se instalan con la carga de trabajo *Desarrollo para el escritorio con C++* de Visual Studio.
- **Error al vincular `glew32.lib` o `glfw3.lib`:** revisar que la plataforma sea **x86**, no x64.
- **Pantalla negra o no encuentra shaders y modelos:** ejecutar desde Visual Studio con F5. Los shaders y modelos se buscan en la carpeta del proyecto.
- **Los reflectores no iluminan:** presionar **F** para encenderlos; de noche (tecla **N**) se nota más.

## Créditos de modelos

- Tennis racket by Poly by Google [CC-BY] via Poly Pizza (https://poly.pizza/m/72XCHXgHlg7)
- Tennis ball by Poly by Google [CC-BY] via Poly Pizza (https://poly.pizza/m/0aPpwgcBbk1)
- Tennis court by Poly by Google [CC-BY] via Poly Pizza (https://poly.pizza/m/7cVH1Xt-LYy)
- Large Support for Lighting by Peter Simcoe [CC-BY] via Poly Pizza (https://poly.pizza/m/7EmgSLnxWDE)
- Ad Board by Uykucukoala [CC0, dominio público] via Poly Pizza (https://poly.pizza/m/8G0qJ84VDS)
- Stadium Seats by Zsky [CC-BY] via Poly Pizza (https://poly.pizza/m/3eKUbGjEPv)
- Texturas Grass 005 (pasto), Concrete 042 A (concreto de las gradas) y Metal 032 (torres y postes del marcador) de ambientCG [CC0, dominio público] (https://ambientcg.com/view?id=Grass005, https://ambientcg.com/view?id=Concrete042A, https://ambientcg.com/view?id=Metal032)
- El resto de los modelos (piso, marcador y personajes) y las demás texturas fueron generados por el equipo con un script y no tienen licencia externa.

## Librerías de terceros

GLEW, GLFW, GLM, Assimp y SOIL2 se incluyen tal como se usaron en el laboratorio, cada una con su propia licencia.

## Metodología

El equipo trabaja con Scrum, con iteraciones de una semana. El detalle está en el acta del proyecto.
