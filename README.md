# Snake Game in C (raylib)

A classic Snake game written in C using the [raylib](https://www.raylib.com/) graphics library. The snake moves on a tile grid, but its movement is smoothly interpolated between tiles so it glides instead of jumping.

## Features

- Classic Snake gameplay: eat apples, grow longer, avoid walls and yourself
- Smooth interpolated movement between grid tiles
- WASD and arrow key controls
- Input buffering that prevents instant 180° turns into the snake's own body
- Apples never spawn on top of the snake
- Score display and a game over screen with quick restart

## Controls

| Key | Action |
|-----|--------|
| `W` / `↑` | Move up |
| `S` / `↓` | Move down |
| `A` / `←` | Move left |
| `D` / `→` | Move right |
| `SPACE` | Restart after game over |

## Requirements

- A C compiler (GCC, Clang, or MSVC)
- [raylib](https://github.com/raysan5/raylib) installed

## Building

Replace `main.c` with the name of your source file.

**Linux**
```bash
gcc main.c -o snake -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

**macOS**
```bash
clang main.c -o snake -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
```

**Windows (MinGW)**
```bash
gcc main.c -o snake.exe -lraylib -lopengl32 -lgdi32 -lwinmm
```

Then run it:
```bash
./snake
```

## Configuration

Gameplay can be tuned with the constants at the top of the source file:

| Constant | Default | Description |
|----------|---------|-------------|
| `GRID_WIDTH` | `44` | Grid width in tiles |
| `GRID_HEIGHT` | `22` | Grid height in tiles |
| `TILE_SIZE` | `32` | Size of each tile in pixels |
| `MOVE_INTERVAL` | `0.15f` | Seconds between snake steps (lower = faster) |
| `MAX_SNAKE_SIZE` | `256` | Maximum snake length |

The window size is `GRID_WIDTH * TILE_SIZE` by `GRID_HEIGHT * TILE_SIZE` (1408×704 with the defaults).

## How It Works

- The game logic runs on a fixed step timer (`MOVE_INTERVAL`), independent of the frame rate.
- Before each step, the previous body positions are saved. During rendering, each segment is drawn at a position interpolated between its previous and current tile, which produces the smooth motion.
- Each apple eaten adds 10 points and grows the snake by one segment.
- The game ends when the snake hits a wall or its own body.

## Project Structure

```
.
├── main.c      # Entire game (logic + rendering)
└── README.md
```

## License

MIT
