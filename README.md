*This project has been created as part of the 42 curriculum by yzhang2, weiyang.*

# cub3D

## Description
**cub3D** is a 42 School graphics project inspired by the world-famous *Wolfenstein 3D*. The primary goal of this project is to create a realistic first-person perspective inside a maze using **ray-casting** principles.

By leveraging the **miniLibX** graphical library, we developed a rendering engine that transforms a 2D grid map into a 3D navigable environment. The project emphasizes mathematical logic (trigonometry and DDA), strict file parsing, and efficient resource management in C.

### Core Features
- **Mandatory Goals**: 
    - Smooth 3D rendering with textures applied based on wall orientation (**NO / SO / WE / EA**).
    - Customizable **floor** and **ceiling** colors via RGB values.
    - Responsive event handling for movement (`W/A/S/D`), rotation (`left/right arrows`), and clean exits (`ESC` or window cross).
    - Comprehensive `.cub` scene file validation, ensuring maps are fully enclosed and spawn points are unique.
- **Bonus Build**:
    - Functional doors (toggle with `E`).
    - Minimap for real-time navigation.
    - Oriented/animated sprites for environmental depth.
    - Mouse-controlled camera rotation (Mouse Look) and wall collisions.

## Instructions

### 1) Requirements
This project is designed for **Linux/X11**. It uses the **minilibx-linux** version included in the repository. Ensure you have X11 development libraries installed on your system.

### 2) Compilation
The provided `Makefile` includes the following rules:
- `make`: Compiles the mandatory executable `cub3d`.
- `make bonus`: Compiles the bonus version with `-D BONUS` flag, creating `cub3d_bonus`.
- `make clean` / `make fclean`: Removes object files or the entire build.
- `make re`: Performs a full re-compilation.

### 3) Execution
Run the program by passing a valid `.cub` scene file as an argument:
```bash
# Mandatory
./cub3d maps/mandatory.cub

# Bonus
./cub3d_bonus maps/bonus.cub

```
### 4) Controls

| Action | Key (Mandatory) | Key (Bonus) |
| --- | --- | --- |
| **Move** | `W` `A` `S` `D` | `W` `A` `S` `D` |
| **Rotate View** | `←` / `→` | `←` / `→` or **Mouse** |
| **Toggle Door** | - | `E` |
| **Exit** | `ESC` / Red Cross | `ESC` / Red Cross |

## Technical Notes

### Ray-casting with DDA

The engine renders the scene column by column using the **DDA (Digital Differential Analyzer)** algorithm. For each ray, we calculate the distance to the nearest wall, determine which side was hit to apply the correct texture, and use perpendicular distance to prevent "fish-eye" distortion.

### Parsing & Validation

Our parser strictly validates the `.cub` configuration. It ensures:

* No missing or duplicate identifiers.
* Maps are **closed/surrounded by walls**, even when containing irregular spaces.
* Meaningful error messages are displayed (starting with `Error\n`) followed by an immediate clean exit upon detecting invalid data.

## Resources

### Classic References
* **Lode Vandevenne — Raycasting Tutorial**: [lodev.org/cgtutor/raycasting.html](https://lodev.org/cgtutor/raycasting.html) – The definitive resource for ray-casting logic and DDA implementation.
* **Cub3D Detailed Tutorial (HackMD)**: [hackmd.io/@nszl/H1LXByIE2](https://hackmd.io/@nszl/H1LXByIE2) – A comprehensive guide on implementation steps and map validation.
* **3D Ray-casting Game with Cub3D (Medium)**: [Article Link](https://devabdilah.medium.com/3d-ray-casting-game-with-cub3d-7a116376056a) – An overview of the rendering pipeline and project architecture.

### Video Materials
* **Linear Algebra & Mathematics**:
    * [Essence of Linear Algebra (YouTube)](https://www.youtube.com/watch?v=X9XMUY6RAzo&list=PLYvOmMwPgVxPfHD82QM07gKwaQ5FhPa07) / [Bilibili Version](https://www.bilibili.com/video/BV1ys411472E/) – Essential math for camera planes and vector rotations.
* **Ray-casting Visualized**:
    * [Super Fast Ray Casting (YouTube)](https://www.youtube.com/watch?v=NbSee-XM7WA)
    * [Make Your Own Raycaster (YouTube)](https://www.youtube.com/watch?v=G9i78WoBBIU&t=292s)
* **MiniLibX Introduction**:
    * [Getting Started with MLX (YouTube)](https://www.youtube.com/watch?v=bYS93r6U0zg) – Hooks, window management, and image handling.

### How AI was used (tasks + project parts)
AI tools were utilized as technical assistants to facilitate understanding, troubleshooting, and documentation. They served as a supplementary resource rather than an automated code generator.

Concretely, AI was used for the following tasks:
* **Mathematical Concepts**: Explaining the mathematical foundations of ray-casting, including the camera plane projection, DDA (Digital Differential Analyzer) stepping, and the calculation of perpendicular distance to prevent fish-eye distortion.
* **Logic Review**: Assisting in the identification of edge cases for `.cub` file parsing and verifying the robustness of map validation algorithms, particularly for closed-map and leak detection logic.
* **Debugging**: Troubleshooting complex compilation warnings and errors, specifically those related to strict compiler flags (`-Werror`).
* **Documentation Support**: Structuring and refining the technical documentation to ensure professional presentation and clarity for evaluators.

*All AI-suggested concepts and logic were independently reviewed, tested, and reworked by the team to ensure full technical mastery and compliance with evaluation standards.*