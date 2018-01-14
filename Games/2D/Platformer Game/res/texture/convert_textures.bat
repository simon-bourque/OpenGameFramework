@echo off

conv t uncolored_forest.png uncolored_forest.tx 2d n ce ce
conv t boxItem.png boxItem.tx 2d n ce ce
conv t boxItem_disabled.png boxItem_disabled.tx 2d n ce ce
conv t coinGold.png coinGold.tx 2d n ce ce
conv t font3.png font3.tx 2d l ce ce
conv t gemRed.png gemRed.tx 2d n ce ce

conv t p1_spritesheet.png player_duck.tx 2d n ce ce 365 98 69 71
conv t p1_spritesheet.png player_jump.tx 2d n ce ce 438 93 67 94
conv t p1_spritesheet.png player_stand.tx 2d n ce ce 67 196 66 92
conv t p1_spritesheet.png player_walk.tx 2da n ce ce 0 0 72 97 73 0 72 97 146 0 72 97 0 98 72 97 73 98 72 97 146 98 72 97 219 0 72 97 292 0 72 97 219 98 72 97 365 0 72 97 292 98 72 97

conv t tiles_spritesheet.png tiles_spritesheet.tx 2da n ce ce auto 0 2 70 70
conv t sheet1.png sheet1.tx 2da n ce ce auto 0 0 70 70

echo Done!
pause
