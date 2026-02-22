*This project has been created as part of the 42 curriculum by yzhang2, weiyang.*

# cub3D — My first RayCaster with miniLibX (42)

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

* **Lode Vandevenne — Raycasting Tutorial**: [lodev.org/cgtutor/raycasting.html](https://lodev.org/cgtutor/raycasting.html) (The industry standard for ray-casting logic).
* **3D Ray-casting Game with Cub3D (Medium)**: [Tutorial link](https://devabdilah.medium.com/3d-ray-casting-game-with-cub3d-7a116376056a).
* **Cub3D detailed tutorial (HackMD)**: [Tutorial link](https://hackmd.io/@nszl/H1LXByIE2).
* **Linear Algebra (3Blue1Brown)**: [YouTube Playlist](https://www.youtube.com/watch?v=X9XMUY6RAzo&list=PLYvOmMwPgVxPfHD82QM07gKwaQ5FhPa07).

### AI Usage Disclosure

AI tools (ChatGPT/Gemini) were utilized as technical assistants throughout this project to enhance development efficiency:

* **Task Specifics**: AI helped clarify the mathematical projection of the camera plane, refined the logic for detecting map leaks (flood-fill variants), and assisted in debugging complex X11/miniLibX memory leaks.
* **Documentation**: AI was used to polish the README for clarity and professional tone.
* **Verification**: All AI-generated suggestions were manually reviewed, tested, and integrated only after we fully understood the underlying logic to ensure compliance with the 42 evaluation standards.
