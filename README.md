# grzetris

## Rationale

We've all been there. You have a small break, like that between lectures,
and need something to play whilst you wait. Something quick to start, and
equally fast to put down.

That something is **grzetris**!

## Current features
- Randomised pieces
- Line clearing
- Full geometric rotation
- Next piece preview
- Quick drop

## Planned features
- Customisable key binds
- Scoring system
- Leaderboard
- Storing pieces
- Cool menu screen™

## Installation

Unfortunately, grzetris is not currently packaged for any system, as it's
not mature enough yet. In the meantime, you can build it locally, as described
below.

### Dependencies
- `>=ncurses-6.4` (will probably work on older versions)

### Building
> Requires `>=cmake-3.20`

1. Clone the repository:
```shell
git clone https://github.com/Grzegorzorzorz/grzetris.git && cd grzetris
```
2. Make a build directory:
```shell
mkdir build && cd build
```
3. Prepare the build:
```shell
cmake ../ -DCMAKE_BUILD_TYPE=Release
```
4. Build grzetris:
```shell
make -j25
```
5. Launch grzetris! :)
```shell
./grzetris
```

#### Quick build:
```shell
git clone https://github.com/Grzegorzorzorz/grzetris.git && cd grzetris
mkdir build && cd build
cmake ../ -DCMAKE_BUILD_TYPE=Release
make -j25
```

## Licence

**grzetris**, a *tris-like

Copyright (C) 2024 Grzegorz Ciołek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

### Third party
> See [LICENCE_THIRD_PARTY] for more information.

- [JSON for Modern C++](https://github.com/nlohmann/json), under the MIT Licence.
