# Game Boy PPU Notes

Various notes that to help me organize my thoughts and overall understanding of the PPU. If you happen to not be me and are reading this: don't take any of this as gospel. I'm merely standing on the shoulders of giants, looking down, and trying not to panic. :^)

## The OBJ "Penalty"

### Pandocs Explanation

The [Pandocs Rendering Section's](https://gbdev.io/pandocs/Rendering.html#obj-penalty-algorithm) explanation of the OBJ "penalty" is a strange one. I think it's more intended to allow homebrew devs to get the jist of what will happen if the sprite is in X position, but from an emudev perspective it doesn't make a whole lot of sense. To be fair, they do call it an "algorithm" and not documentation, but (based on other documentation I've read) it feels to me like it's painting over details that even homebrew devs may find useful.

In a nutshell, the algorithm tells you to determine which tile the leftmost pixel of a sprite lands in, count the number of that tile's pixels are to the **right** of the pixel, subtract 2, and incur a penalty of `max(0, count)`, then a 6-dot penalty from the tile fetch.

### My Interpretation

My read on this is that you keep writing pixels (so long as the BG FIFO still has pixels), and when you encounter a sprite you pause output and fetch the sprite. If the background fetcher is working on the next tile, you wait until it finishes before proceeding.

This aligns with the documented PPU timings. If you're two pixels into a tile, the BG fetcher would be on the first T-cycle of the low tile data fetch cycle. In three T-cycles, it would finish the fetch and push the BG pixels into the FIFO, freeing up the tile fetcher for the sprite (per the algorithm: 3 cycle penalty + 6 cycle fetch == 9 cycle delay. The math does, indeed, math). The timing would probably look something like this:

```
LCDX == 0, Plot, BG Get Tile T1
LCDX == 1, Plot, BG Get Tile T2
LCDX == 1, BG Get Data Low T1 (sprite X == 10 (left-most pixel at LCDX == 2), pause plotting)
LCDX == 1, BG Get Data Low T2
LCDX == 1, BG Get Data High T1
LCDX == 1, BG Get Data High T2, BG FIFO Push, OBJ Get Tile T1
LCDX == 1, OBJ Get Tile T2
LCDX == 1, OBJ Get Data Low T1
LCDX == 1, OBJ Get Data Low T2
LCDX == 1, OBJ Get Data High T1
LCDX == 1, OBJ Get Data High T2, OBJ FIFO Push
LCDX == 2, Plot
LCDX == 3, Plot
LCDX == 4, Plot
LCDX == 5, Plot
LCDX == 6, Plot
LCDX == 7, Plot
```

Assuming this is accurate, this is actually easier than trying to fake it.

#### Sprite Priority

Incoming sprites should be merged into the FIFO. The merge should start at the current position, and the winning pixel will depend on the priority. Color index `00` is treated as transparent, and will either show the pixel from a lower-priority sprite or the background/window (unless OAM flag 7--BG-over-OBJ--is set. See "Background Priority").

In monochrome mode, if another sprite overlaps with that one, we perform the fetch at that sprite's X coordinate, then merge its pixels into the FIFO. Priority is strictly left-to-right in DMG mode.

In color mode, sprites are prioritized by their order in OAM, so a sprite with a higher X position can overlap one drawn first. Make sure to store the OAM index in the FIFO pixel's `ObjPriority` field. If the incoming pixel has a **lower** priority value than the current one, it takes priority over the existing one.

From what I can gather, only strictly lower priorities win. If the priorities are equal, the first sprite drawn wins. This should allow me to run the same priority determination regardless of mode and have it Just Work™, as long as I ensure that DMG FIFO pixels are all assigned a priority of `0`.

#### Background Priority

In DMG mode, the background will take priority over the sprite if OAM flag 7 (BG-over-OBJ) is set.

In CGB mode, this behavior is more flexible. `LCDC.0` **must** be enabled for this behavior to function at all, and at least one of the following conditions must be true:

* OAM flag 7 (BG-over-OBJ) is set.
* BG map attribute bit 7 (priority) is set.

In either case, this behavior only occurs if the BG color index is 1-3, with index 0 being transparent.

Pandocs has a nice [relationship table](https://gbdev.io/pandocs/Tile_Maps.html#bg-to-obj-priority-in-cgb-mode), reproduced here:

| LCDC bit 0 | OAM attr bit 7 | BG attr bit 7 | Priority        |
|------------|----------------|---------------|-----------------|
| 0          | 0              | 0             | OBJ             |
| 0          | 0              | 1             | OBJ             |
| 0          | 1              | 0             | OBJ             |
| 0          | 1              | 1             | OBJ             |
| 1          | 0              | 0             | OBJ             |
| 1          | 0              | 1             | BG col 1-3, OBJ |
| 1          | 1              | 0             | BG col 1-3, OBJ |
| 1          | 1              | 1             | BG col 1-3, OBJ |