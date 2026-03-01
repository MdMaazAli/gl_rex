# gl_rex 🦖

A lightweight, 3D platformer prototype built from scratch using **C++** and **OpenGL**. This project focuses on implementing core game engine physics and custom rendering pipelines without relying on high-level game engines.

---

## 🚀 Features
* **Custom Renderer:** Utilizes OpenGL for real-time 3D rendering of primitives (spheres, cubes, pyramids).
* **Dynamic Lighting:** Implementation of Phong lighting for depth and volume.
* **Physics Foundation:** Basic gravity and jump mechanics for the player character ("Rexxy").
* **Procedural Geometry:** Objects rendered using custom vertex data and GLSL shaders.

## 🛠️ Tech Stack
* **Language:** C++
* **Graphics API:** OpenGL
* **Libraries:** * **GLFW** (Windowing and Input)
  * **GLAD** (OpenGL Loader)
  * **GLM** (OpenGL Mathematics)

## 🚧 Current Development: Collision System
The project is currently in the prototype phase. Current focus areas include:
* **AABB (Axis-Aligned Bounding Box):** Implementing bounding boxes for static obstacles like the cubes and pyramids.
* **Sphere-to-Box Collision:** Handling interactions between the player (sphere) and the environment.
* **Collision Response:** Fine-tuning the physics to prevent clipping and handle bounces accurately.

## Demo
The current prototype features "Rexxy" navigating a dark environment with red-lit flooring and green obstacles.

![Project Preview](https://github.com/MdMaazAli/gl_rex/blob/main/rexxy_prototype1%20(1).gif)

## ⚙️ Setup and Build
1. **Clone the repository:**
   ```bash
   git clone [https://github.com/MdMaazAli/gl_rex.git](https://github.com/MdMaazAli/gl_rex.git)
