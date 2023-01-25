# Robot reference game

This is the reference implementation for the board of the robots.
It contains both the game part and a `smart` robot.
There is also a copy of all other robots in the game.

## CLI arguments

- `--rounds 1000` - runs the game in silent mode for 1000 rounds
- `--smart` - enables the smart robot - else it's only used in 50% of the live games, or not at all in the silent mode
- `--standard` - uses the standard mode with no boni in the `radar` return and making a draw after 100 empty moves
- `--debug` - allows to debug some problems with the Game engine
- `--nostudents` - only puts the teacher's robots in the game

# CHANGELOG
- 2022-12-22 - update of robots and CLI arguments
- 2022-12-21 - update of robots and CLI arguments
- 2022-12-20 - update to latest robots