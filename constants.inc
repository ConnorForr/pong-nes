; Descriptions provided by NES DEV Wiki
;
; PPU_CTRL
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 7  bit  0
; ---- ----
; VPHB SINN
; |||| ||||
; |||| ||++- Base nametable address
; |||| ||    (0 = $2000; 1 = $2400; 2 = $2800; 3 = $2C00)
; |||| |+--- VRAM address increment per CPU read/write of PPUDATA
; |||| |     (0: add 1, going across; 1: add 32, going down)
; |||| +---- Sprite pattern table address for 8x8 sprites
; ||||       (0: $0000; 1: $1000; ignored in 8x16 mode)
; |||+------ Background pattern table address (0: $0000; 1: $1000)
; ||+------- Sprite size (0: 8x8 pixels; 1: 8x16 pixels – see PPU OAM#Byte 1)
; |+-------- PPU master/slave select
; |          (0: read backdrop from EXT pins; 1: output color on EXT pins)
; +--------- Generate an NMI at the start of the
;            vertical blanking interval (0: off; 1: on)
PPU_CTRL = $2000

; PPU_MASK
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 7  bit  0
; ---- ----
; BGRs bMmG
; |||| ||||
; |||| |||+- Greyscale (0: normal color, 1: produce a greyscale display)
; |||| ||+-- 1: Show background in leftmost 8 pixels of screen, 0: Hide
; |||| |+--- 1: Show sprites in leftmost 8 pixels of screen, 0: Hide
; |||| +---- 1: Show background
; |||+------ 1: Show sprites
; ||+------- Emphasize red (green on PAL/Dendy)
; |+-------- Emphasize green (red on PAL/Dendy)
; +--------- Emphasize blue
PPU_MASK = $2001

; PPU_STATUS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 7  bit  0
; ---- ----
; VSO. ....
; |||| ||||
; |||+-++++- PPU open bus. Returns stale PPU bus contents.
; ||+------- Sprite overflow. The intent was for this flag to be set
; ||         whenever more than eight sprites appear on a scanline, but a
; ||         hardware bug causes the actual behavior to be more complicated
; ||         and generate false positives as well as false negatives; see
; ||         PPU sprite evaluation. This flag is set during sprite
; ||         evaluation and cleared at dot 1 (the second dot) of the
; ||         pre-render line.
; |+-------- Sprite 0 Hit.  Set when a nonzero pixel of sprite 0 overlaps
; |          a nonzero background pixel; cleared at dot 1 of the pre-render
; |          line.  Used for raster timing.
; +--------- Vertical blank has started (0: not in vblank; 1: in vblank).
;            Set at dot 1 of line 241 (the line *after* the post-render
;            line); cleared after reading $2002 and at dot 1 of the
;            pre-render line.
PPU_STATUS = $2002

; OAM_ADDR
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Location in OAM memory where you would like to write to. Write the 
; address of OAM you want to access here. Most games just write $00 here 
; and then use OAMDMA.
OAM_ADDR = $2003

; OAM_DATA
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Write OAM data here. Writes will increment OAMADDR after the write; 
; reads do not. Reads during vertical or forced blanking return the value 
; from OAM at that address. 
; Do not write directly to this register in most cases.
OAM_DATA = $2004

; PPU_SCROLL (Write twice)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This register is used to change the scroll position, that is, to tell 
; the PPU which pixel of the nametable selected through PPUCTRL should be 
; at the top left corner of the rendered screen. Typically, this register is 
; written to during vertical blanking, so that the next frame starts rendering 
; from the desired location, but it can also be modified during rendering 
; in order to split the screen. Changes made to the vertical scroll during 
; rendering will only take effect on the next frame.
; 
; After reading PPUSTATUS to reset the address latch, write the horizontal 
; and vertical scroll offsets here just before turning on the screen.
;
; Ex.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; bit PPUSTATUS
; ; possibly other code goes here
; lda cam_position_x
; sta PPUSCROLL
; lda cam_position_y
; sta PPUSCROLL
PPU_SCROLL = $2005

; PPU_ADDR (Write twice)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Because the CPU and the PPU are on separate buses, neither has 
; direct access to the other's memory. The CPU writes to VRAM through a 
; pair of registers on the PPU. First it loads an address into PPUADDR, 
; and then it writes repeatedly to PPUDATA to fill VRAM.
; Ex.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; lda #$21
; sta PPUADDR
; lda #$08
; sta PPUADDR
PPU_ADDR = $2006

; PPU_DATA
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; VRAM read/write data register. After access, the video memory address 
; will increment by an amount determined by bit 2 of $2000.
PPU_DATA = $2007

; OAM_DMA
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This port is located on the CPU. Writing $XX will upload 256 bytes of
; data from CPU page $XX00–$XXFF to the internal PPU OAM. This page is 
; typically located in internal RAM, commonly $0200–$02FF, but cartridge 
; RAM or ROM can be used as well.
OAM_DMA = $4014





