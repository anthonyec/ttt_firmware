# TTT Firmware
## Requirements
### Hardware
- 2x OLED 0.96 inch display B/W
- 2x Arduino Nano
- 2x push buttons
- 2x Bluetooth modules

### Software

## Setup

## Circuit

## Concepts
### How undo works
An array keeps a list of which player "won" a given point. When the undo action is performed, a point in subtracted from the player who won the last point.

The orginal implentation to undo kept a list of both player scores in a 2D array. However, this used a lot more memory. With an array of who won the point, an array of 1 byte integars can be used instead of 2x 2byte integars.

#### Example

When the game starts, no player has won any points. The score is 0:0.

```fantastic psuedo c++
currentHistoryIndex = 0
history = []
```

Player 1 wins a point. Player 1 is index `0`, thus `0` is added to the history.

```fantastic psuedo c++
currentHistoryIndex = 0
history = [0]
```

Player 2 then wins a point. Player 2 is index `1`, thus `1` is added to the history.

```fantastic psuedo c++
currentHistoryIndex = 1
history = [0, 1]
```

This can continue, adding either player's index to the history.

```fantastic psuedo c++
currentHistoryIndex = 5
history = [0, 1, 0, 0, 0, 1]
```

If the last point was an mistake, then undo can be performed. This will subtract the `currentHistoryIndex` and then subtract a point from whatever player index is at the new `currentHistoryIndex`.

In this case, it will be Player 1 as player index `0` is at index `4` in the history.

```fantastic psuedo c++
currentHistoryIndex = 4
history = [0, 1, 0, 0, 0, 1]

subtractScoreFromPlayer(history[currentHistoryIndex])
```

Future points will overwrite.

```fantastic psuedo c++
currentHistoryIndex = 5
history = [0, 1, 0, 0, 0, 0]
```
