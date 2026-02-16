# VOID DRIFTER: Terminal Bullet-Hell

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![C++](https://img.shields.io/badge/C++-17-orange.svg)
![Platform](https://img.shields.io/badge/Platform-Linux-lightgrey.svg)
![Build](https://img.shields.io/badge/Build-Optimized-green.svg)

**VOID DRIFTER** es un trepidante shooter de naves espacial desarrollado íntegramente en C++ para la terminal. Inspirado en los clásicos *bullet-hell*, el juego ofrece una experiencia frenética con gráficos ASCII dinámicos, física de partículas y un sistema de renderizado de alta velocidad optimizado para Linux.

---

## 🚀 Características Principales

* **Motor ASCII Dinámico:** El escenario y el HUD se adaptan automáticamente si cambias el tamaño de tu terminal en tiempo real.
* **Multilingüe:** Soporte completo en configuración para **Español, Inglés y Portugués**.
* **Personalización:** Elige entre 3 modelos de naves espaciales (`A`, `V`, `U`) y 3 niveles de dificultad.
* **Estética Retro-Moderna:** Fondo de estrellas con paralaje, explosiones de partículas y colores neón ANSI.
* **Zero-Lag:** Optimizado mediante un búfer de renderizado único para evitar el parpadeo (*flickering*) y consumo mínimo de CPU.
* **Sin Dependencias:** No requiere librerías externas como `ncurses`. Usa puramente las APIs estándar de Linux/Unix.

---

## 🛠️ Requisitos del Sistema

* **Sistema Operativo:** Cualquier distribución de Linux (Ubuntu, Arch, Fedora, Debian, etc.).
* **Compilador:** `g++` (GCC) con soporte para el estándar **C++17**.
* **Terminal:** Se recomienda una terminal con soporte para colores ANSI (Gnome Terminal, Konsole, Alacritty, Kitty, xterm).

---

## Estructura del Proyecto

El código está modularizado para una mayor limpieza y facilidad de mantenimiento:

- `main.cpp`: Punto de entrada y bucle principal.
- `Game.h/cpp`: Lógica del juego, colisiones, estados y renderizado.
- `Terminal.h/cpp`: Gestión de la consola, ocultación del cursor y modo raw de entrada.
- `Entity.h`: Definición de objetos físicos (jugador, enemigos, balas).
- `Vec2.h`: Estructura matemática para posiciones y vectores.

---

## Compilación e Instalación

Para obtener el máximo rendimiento y la mayor compatibilidad entre sistemas Linux, sigue estos pasos:

### 1. Clonar o descargar los archivos
Asegúrate de tener todos los archivos `.h` y `.cpp` en la misma carpeta.

### 2. Compilación Optimizada (Recomendada)
Este comando genera un binario estático que incluye todas las librerías necesarias, lo que lo hace compatible incluso en versiones antiguas de Linux:

```bash
g++ -O3 -std=c++17 -static -s main.cpp Game.cpp Terminal.cpp -o vdrifter
```

### 3. Ejecución

```bash
./vdrifter
```

## Instrucciones de Juego

* **Tecla--Accion:**

* "W, A, S, D",Mover la nave
* Espacio,Disparar láser
* Q,Volver al menú / Salir
* R,Reiniciar (en pantalla de Game Over)
* "1, 2, 3",Navegar por menús y ajustes