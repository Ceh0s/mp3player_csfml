#!/bin/sh
##
## EPITECH PROJECT, 2025
## mp3player
## File description:
## builds assets/atlas.png: the 25 first icons come from base.png (the
## atlas drawn by hand), the others from icons/<name>.svg in the order of
## icons.txt (same order as enum icon_e after ICON_BACK). Each icon is put
## once in outline (ring + symbol), then once more as a filled disc with
## the symbol cut out, NB_ICONS cells later.
## Needs ImageMagick (magick) and rsvg-convert. Run from the root:
##     sh tools/atlas/make_atlas.sh
##

DIR=tools/atlas
CELL=256
COLS=10
BASE_NB=25
TMP=$(mktemp -d)

NEW_NB=$(grep -c . "$DIR/icons.txt")
NB=$((BASE_NB + NEW_NB))

# the old icons: outline i at cell i, filled at cell i + BASE_NB
cell_of_base() {
    x=$(($1 % COLS * CELL))
    y=$(($1 / COLS * CELL))
    magick "$DIR/base.png" -crop "${CELL}x${CELL}+$x+$y" +repage "$2"
}

i=0
while [ $i -lt $BASE_NB ]; do
    cell_of_base $i "$TMP/$(printf %03d $i).png"
    cell_of_base $((i + BASE_NB)) "$TMP/$(printf %03d $((i + NB))).png"
    i=$((i + 1))
done

# the content of an svg, without its first and last lines (<svg> tags)
symbol() {
    sed '1d;$d' "$DIR/icons/$1.svg"
}

i=$BASE_NB
while read -r name; do
    [ -z "$name" ] && continue
    {
        echo '<svg xmlns="http://www.w3.org/2000/svg" width="256" height="256" viewBox="0 0 256 256" color="white">'
        echo '<circle cx="128" cy="128" r="110" fill="none" stroke="white" stroke-width="16"/>'
        symbol "$name"
        echo '</svg>'
    } > "$TMP/outline.svg"
    {
        echo '<svg xmlns="http://www.w3.org/2000/svg" width="256" height="256" viewBox="0 0 256 256">'
        echo '<mask id="cut"><circle cx="128" cy="128" r="120.5" fill="white"/>'
        echo '<g color="black">'
        symbol "$name"
        echo '</g></mask>'
        echo '<circle cx="128" cy="128" r="120.5" fill="white" mask="url(#cut)"/>'
        echo '</svg>'
    } > "$TMP/filled.svg"
    rsvg-convert "$TMP/outline.svg" -o "$TMP/$(printf %03d $i).png"
    rsvg-convert "$TMP/filled.svg" -o "$TMP/$(printf %03d $((i + NB))).png"
    i=$((i + 1))
done < "$DIR/icons.txt"

ROWS=$(((2 * NB + COLS - 1) / COLS))
magick montage "$TMP"/*.png -background none -tile "${COLS}x$ROWS" \
    -geometry "${CELL}x${CELL}+0+0" PNG32:assets/atlas.png
rm -rf "$TMP"
echo "assets/atlas.png: $NB icons, ${COLS}x$ROWS cells"
