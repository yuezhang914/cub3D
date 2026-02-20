# cub3D — RayCaster with miniLibX (42)

A 42 School graphics project inspired by **Wolfenstein 3D**. We built a first-person engine using **ray-casting** and **miniLibX**, with strict parsing/validation of `.cub` scene files (textures, colors, and map).

---

## ✅ What we implemented

### Mandatory (`cub3d`)

* **Ray-casting renderer (Wolf3D style)**
* 3D projection from a 2D grid map.
* **4 wall textures** depending on hit direction: `NO / SO / WE / EA`.
* **Floor / ceiling colors**: `F` and `C` with strict RGB validation.


* **Window & input (subject compliant)**
* Move: `W A S D`.
* Rotate: `← / →`.
* Quit: `ESC` or window close (red cross) → clean exit.


* **Robust .cub parsing**
* Detects missing/duplicate identifiers, invalid RGB format/range, invalid map characters, etc.


* **Map validation**
* Exactly **one** spawn (`N/S/E/W`).
* Map must be **closed/surrounded by walls** (no leaks/open walls).
* Spaces are treated as real map content and handled safely during validation.



### Bonus (`cub3d_bonus`)

*Enabled by compiling with -D BONUS.*

* **Wall collisions**: Prevent walking through walls or closed doors.
* **Minimap**: A real-time top-down overlay.
* **Doors**: Interactive doors (map token `D`) that can be toggled open/closed.
* **Sprites**: Static, animated, or oriented sprites (e.g., `T/B/C/M`).
* **Mouse look**: Rotate the camera smoothly with mouse movement.

---

## 🧠 Methods / How it works

### 1) Ray-casting with DDA

For each vertical screen column:

1. Cast a ray from the player position and direction.
2. Step through grid cells using **DDA (Digital Differential Analyzer)**.
3. Stop at the first hit (wall/door).
4. Compute perpendicular distance to avoid **fish-eye distortion**.
5. Choose the correct texture and draw a textured vertical stripe.

### 2) Event input + per-frame update

* Key press/release hooks only set **input states**.
* Each frame updates movement/rotation, then renders the next frame.
* Movement uses axis-splitting (X then Y) to reduce sliding glitches and wall-sticking.

### 3) Clean resource lifecycle

* Centralized error handling and cleanup.
* All images, textures, and sprites are tracked and destroyed properly to ensure **0 memory leaks**.

---

## 🕹️ Build & Run

### Compile

* **Mandatory**: `make`
* **Bonus**: `make bonus`
* **Full Clean**: `make fclean`

### Run

```bash
./cub3d path/to/map.cub
# OR
./cub3d_bonus path/to/map.cub

```

---

## 🎮 Controls

* **Move**: `W A S D`
* **Rotate**: `← / →`
* **Quit**: `ESC` or Red Cross
* **Door**: `E` (Bonus)
* **Mouse Look**: Mouse movement (Bonus)

---

## 🗂️ Repository Structure

* `src/parse/`: `.cub` parsing & map validation (including open-wall checks).
* `src/raycasting/`: Ray setup, DDA stepping, hit calculations.
* `src/render/`: Drawing pixels/columns and the render loop.
* `src/game/`: Input state, movement, and main loop.
* `src/setup/`: MLX initialization, texture loading, and hook bindings.
* `src/cleanup/`: Error handling and resource freeing.
* `src/bonus/`: Implementation of doors, sprites, and minimap.

---

## 👥 Contributors

* **Wei Yang**
* **Yue Zhang**

---

---
# cub3D  中文版说明

本项目是 42学校的图形学入门项目：使用 **miniLibX** 和 **Ray-Casting（光线投射）** 原理，从第一人称视角在迷宫里渲染出“近似 3D”的画面（Wolfenstein 3D 风格），并对 `.cub` 场景文件进行严格解析与校验。

---

## ✅ 我们实现了什么

### Mandatory（必做）

* **Ray-casting 3D 渲染**
* 从 2D 网格地图投射光线，逐列渲染墙面。
* 根据朝向应用 4 张墙面贴图：`NO / SO / WE / EA`。
* 支持地板与天花板纯色：`F / C`（包含严格的 RGB 解析与校验）。


* **窗口与输入控制**
* `W A S D`：移动视角。
* `← / →`：左右旋转视角。
* `ESC` 或点击窗口红叉：干净退出。


* **文件解析（严格校验）**
* 读取并校验贴图路径、RGB 颜色、地图内容。
* 防止：缺字段、重复字段、RGB 越界/格式错误、非法字符等。


* **地图合法性校验**
* **必须被墙封闭**：通过算法确保地图没有“漏气”或开口。
* **唯一出生点**：出生点必须且只能有一个（N/S/E/W）。



### Bonus（加分项）

*通过 make bonus 编译开启。*

* **门 (Doors)**：地图字符 `D`，支持按键切换开/关。
* **精灵 (Sprites)**：支持在地图中放置静态、动画或具备方向感的精灵（如 `T/B/C/M`）。
* **小地图 (Minimap)**：实时显示玩家位置与周边地形。
* **鼠标控制**：通过鼠标移动实现平滑的视角旋转。
* **碰撞处理**：墙壁、空格、关闭的门均会产生物理阻挡。

---

## 🧠 关键方法

### 1) DDA（Digital Differential Analyzer）步进找墙

针对屏幕每一列：

1. 计算射线的发射方向。
2. 在网格中用 **DDA** 算法逐格推进。
3. 找到首次命中的墙壁（或门）。
4. 计算垂直距离以消除“鱼眼畸变”。
5. 采样纹理并绘制该列像素带。

### 2) 输入事件处理

* 采用“按键状态记录法”，`on_key_down` 只修改状态，不直接移动。
* 在主循环中根据状态统一更新位置，采用“坐标拆分”移动方式，减少卡墙抖动。

### 3) 资源生命周期管理

* 所有图片、纹理和内存资源统一记录。
* 程序退出时执行 `graceful_exit`，确保 **0 内存泄漏**。

---

## 🕹️ 编译与运行

### 编译

* **必做部分**: `make`
* **加分部分**: `make bonus`
* **彻底清理**: `make fclean`

### 运行

```bash
./cub3d path/to/map.cub
或
./cub3d_bonus path/to/map.cub
```

---

## 🎮 操作说明

* **移动**: `W A S D`
* **旋转视角**: `← / →`
* **退出**: `ESC` 或窗口红叉
* **开关门**: `E` (仅限 Bonus)
* **鼠标移动**: 旋转视角 (仅限 Bonus)

---

## 📁 项目结构

* `src/parse/`：`.cub` 解析、地图封闭性检查、出生点抽取。
* `src/raycasting/`：射线计算、DDA 步进、命中与距离计算。
* `src/render/`：像素绘制、贴图采样、渲染循环。
* `src/game/`：输入状态维护、移动逻辑。
* `src/setup/`：MLX 初始化、纹理加载、Hook 绑定。
* `src/cleanup/`：错误处理与统一清理。
* `src/bonus/`：门、精灵、小地图等功能的具体实现。

---

## 👥 贡献者

* **Wei Yang**
* **Yue Zhang**

