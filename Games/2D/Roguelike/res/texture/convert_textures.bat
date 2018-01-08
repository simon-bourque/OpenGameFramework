@echo off

conv t const.png const.tx 2d l ce ce
conv t font3.png font3.tx 2d l ce ce

conv t Floor.png Floor.tx 2da n ce ce auto 0 0 16 16
conv t Wall.png Wall.tx 2da n ce ce auto 0 0 16 16

conv t Rogue.png Rogue_walk_e.tx 2da n ce ce 0 32 16 16 16 32 16 16 32 32 16 16 48 32 16 16
conv t Rogue.png Rogue_walk_n.tx 2da n ce ce 0 48 16 16 16 48 16 16 32 48 16 16 48 48 16 16
conv t Rogue.png Rogue_walk_s.tx 2da n ce ce 0 0 16 16 16 0 16 16 32 0 16 16 48 0 16 16
conv t Rogue.png Rogue_walk_w.tx 2da n ce ce 0 16 16 16 16 16 16 16 32 16 16 16 48 16 16 16
conv t Rogue.png Rogue_idle_e.tx 2d n ce ce 0 32 16 16
conv t Rogue.png Rogue_idle_n.tx 2d n ce ce 0 48 16 16
conv t Rogue.png Rogue_idle_s.tx 2d n ce ce 0 0 16 16
conv t Rogue.png Rogue_idle_w.tx 2d n ce ce 0 16 16 16
conv t LongWep.png sword.tx 2d n ce ce 48 16 16 16

conv t GUI.png heart_full.tx 2d n ce ce 0 16 16 16
conv t GUI.png heart_empty.tx 2d n ce ce 16 16 16 16

conv t GUI.png ui_helm.tx 2d n ce ce 128 160 16 16
conv t GUI.png ui_chest.tx 2d n ce ce 128 176 16 16
conv t GUI.png ui_glove.tx 2d n ce ce 128 192 16 16
conv t GUI.png ui_empty.tx 2d n ce ce 128 208 16 16
conv t GUI.png ui_boot.tx 2d n ce ce 128 224 16 16
conv t GUI.png ui_amulet.tx 2d n ce ce 128 240 16 16
conv t GUI.png ui_bag.tx 2d n ce ce 128 256 16 16
conv t GUI.png ui_tl.tx 2d n ce ce 144 160 16 16
conv t GUI.png ui_t.tx 2d n ce ce 160 160 16 16
conv t GUI.png ui_tr.tx 2d n ce ce 176 160 16 16
conv t GUI.png ui_cl.tx 2d n ce ce 144 176 16 16
conv t GUI.png ui_c.tx 2d n ce ce 160 176 16 16
conv t GUI.png ui_cr.tx 2d n ce ce 176 176 16 16
conv t GUI.png ui_bl.tx 2d n ce ce 144 192 16 16
conv t GUI.png ui_b.tx 2d n ce ce 160 192 16 16
conv t GUI.png ui_br.tx 2d n ce ce 176 192 16 16

echo Done!
pause
