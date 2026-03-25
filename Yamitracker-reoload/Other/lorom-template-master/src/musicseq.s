.include "pentlyseq.inc"
.segment "SPCIMAGE"

; Music data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; TODO: Make a version of pentlyas.py that can output this format

music_inst_table:
  ;    name         lv  rv frq smp att dec sus lvl
  INST KICK,        11, 10,  3,  6, 31, 17,  0,  8
  INST SNARE,       10, 15,  3,  7, 31, 17,  0,  8
  INST HAT,          3,  5,  3,  5, 31, 17,  0,  8
  INST PLING_8,      3, 10,  1,  0, 31, 19, 15,  2
  INST PLING_4,      7,  7,  1,  1, 31, 19, 15,  2
  INST PLING_2,      9,  6,  1,  2, 31, 17, 15,  2
  INST BASSGUITAR,  12, 12,  1,  4, 31, 17, 15,  8
  INST BASSCLAR,    12, 12,  1,  2, 21, 21, 27,  4
  INST HORNBLAT,     3,  9,  1,  0, 21, 21, 27,  4
  INST XYLSHORT,    10,  6,  1,  2, 31, 23, 20,  2
  INST XYLMED,      10,  6,  1,  2, 31, 19, 17,  2

pently_patterns:
  ; patterns 0-4: 2 AM
  .addr PPDAT_0200_sq1, PPDAT_0200_sq2, PPDAT_0200_bass, PPDAT_0200_drums
  ; pattern 4: cleared
  .addr PPDAT_round_clear_sq1
  ; patterns 5-9: 3 AM
  .addr PPDAT_0300_sq1, PPDAT_0300_sq2, PPDAT_0300_bass
  .addr PPDAT_0300_drums
  ; patterns 10-14: 4 AM
  .addr PPDAT_0400_sq1, PPDAT_0400_sq2, PPDAT_0400_drums, PPDAT_0400_drums_2
  ; patterns 15-18: 4 PM
  .addr PPDAT_1600__sq1, PPDAT_1600__sq2, PPDAT_1600__tri, PPDAT_1600__drums
  ; patterns 19-22: 5 AM
  .addr PPDAT_0500_sqloop, PPDAT_0500_triloop, PPDAT_0500_melody
  .addr PPDAT_restore_vol

pently_songs:
.addr  PSDAT_0300, PSDAT_0500, PSDAT_0200, PSDAT_0400, PSDAT_round_clear,  PSDAT_1600

;____________________________________________________________________
; 2am theme
; This is the famous first eight bars of the second movement of
; Beethoven's "Pathetique" done in 9/8 like ACWW 2am.

PSDAT_0200:
  setTempo 1000
  playPat 0, 0, 23, PLING_8
  waitRows 200
  stopPat 0
 playPat 1, 1, 18, 4

 ; playDrumPat 3, 3
  waitRows 144


      playPat 2, 2, 23, 4
       playPat 1, 1, 18, 4
         waitRows 255
        stopPat 1
          stopPat 2
  dalSegno

PPDAT_0200_sq1:
  .byt N_C|D_1, N_C|D_1, N_C|D_1
  .byt N_C|D_D2, N_C|D_D2, N_C|D_D2
  .byt N_D|D_2, N_D|D_2, N_D|D_2
  .byt N_D|D_D4, N_D|D_D4, N_D|D_D4
  .byt N_D|D_4, N_D|D_4, N_D|D_4
  .byt N_DS|D_4, N_DS|D_4, N_DS|D_4
  .byt N_DS|D_D8, N_DS|D_D8, N_DS|D_D8
  .byt N_DS|D_D8, N_DS|D_D8, N_DS|D_D8
  .byt N_E|D_D8, N_E|D_D8, N_E|D_D8
  .byt N_E|D_8, N_E|D_8, N_E|D_8
  .byt N_F|D_8, N_F|D_8, N_F|D_8
  .byt N_F|D_8, N_F|D_8, N_F|D_8
  .byt N_F|D_8, N_F|D_8, N_F|D_8



  .byt PATEND

PPDAT_0200_sq2:

  .byt N_C|D_8, N_CS|D_1, N_C|D_1
  .byt N_CS|D_8, N_C|D_1, N_C|D_1
  .byt N_C|D_8, N_CS|D_1, N_C|D_1
  .byt N_CS|D_8, N_C|D_1, N_C|D_1
  .byt N_C|D_8, N_CS|D_1, N_C|D_1

PPDAT_0200_bass:
  .byt N_DS|D_2, N_DS|D_2, N_DS|D_2
  .byt PATEND

PPDAT_0200_drums:
  .byt PD_KICK|D_D8, PD_HAT|D_8, PD_HAT, PD_HAT|D_D8, PD_SNARE|D_D8, PD_HAT|D_D4
  .byt PATEND


;____________________________________________________________________
; Dry Hands - C418 (Minecraft)

PSDAT_0300:
  setTempo 120
  playPat 0, 5, 32, PLING_8
  playPat 1, 6, 12, PLING_4
  playPat 2, 7, 0, PLING_4
  waitRows 32
  playDrumPat 3, 8
  segno
  waitRows 96
  dalSegno

PPDAT_0300_sq1:
  ; Основная мелодия Dry Hands - правая рука
  ; Первый мотив (A-E-F-G)
  .byt N_A|D_4, N_E|D_4, N_F|D_4, N_G|D_4
  .byt N_F|D_4, N_E|D_4, N_D|D_4, N_E|D_4
  ; Повтор с вариацией
  .byt N_A|D_4, N_E|D_4, N_F|D_4, N_G|D_4
  .byt N_F|D_4, N_E|D_4, N_D|D_4, N_C|D_4
  ; Второй мотив (C-D-E-F)
  .byt N_C|D_4, N_D|D_4, N_E|D_4, N_F|D_4
  .byt N_E|D_4, N_D|D_4, N_C|D_4, N_D|D_4
  ; Возврат к первому мотиву
  .byt N_A|D_4, N_E|D_4, N_F|D_4, N_G|D_4
  .byt N_F|D_4, N_E|D_4, N_D|D_4, N_E|D_4
  .byt PATEND

PPDAT_0300_sq2:
  ; Аккомпанемент - левая рука (аккорды Am, G, F, E)
  ; Am аккорд (A-C-E)

  .byt N_A|D_2, N_C|D_2, N_E|D_2, N_A|D_2
  ; G аккорд (G-B-D)
  .byt N_G|D_2, N_B|D_2, N_D|D_2, N_G|D_2
  ; F аккорд (F-A-C)
  .byt N_F|D_2, N_A|D_2, N_C|D_2, N_F|D_2
  ; E аккорд (E-G#-B) - в оригинале E major, но для простоты E5
  .byt N_E|D_2, N_G|D_2, N_B|D_2, N_E|D_2
  ; Повтор
  .byt N_A|D_2, N_C|D_2, N_E|D_2, N_A|D_2
  .byt N_G|D_2, N_B|D_2, N_D|D_2, N_G|D_2
  .byt N_F|D_2, N_A|D_2, N_C|D_2, N_F|D_2
  .byt N_E|D_2, N_G|D_2, N_B|D_2, N_E|D_2
  .byt PATEND

PPDAT_0300_bass:
  ; Басовая линия - простые долгие ноты
  ; Тон Am

  .byt N_A|D_1, REST|D_2
  .byt N_A|D_1, REST|D_2
  ; Тон G
  .byt N_G|D_1, REST|D_2
  .byt N_G|D_1, REST|D_2
  ; Тон F
  .byt N_F|D_1, REST|D_2
  .byt N_F|D_1, REST|D_2
  ; Тон E
  .byt N_E|D_1, REST|D_2
  .byt N_E|D_1, REST|D_2
  .byt PATEND

PPDAT_0300_drums:
  ; Очень мягкие ударные для атмосферы (не в оригинале, но для заполнения)
  .byt PD_HAT|D_D8, REST|D_D8, PD_HAT|D_D8, REST|D_D8
  .byt PD_HAT|D_D8, REST|D_D8, PD_HAT|D_D8, REST|D_D8
  .byt PD_HAT|D_D8, REST|D_D8, PD_HAT|D_D8, REST|D_D8
  .byt PD_HAT|D_D8, REST|D_D8, PD_HAT|D_D8, REST|D_D8
  .byt PATEND

;____________________________________________________________________
; 4am theme

PSDAT_0400:
  setTempo 300
  playPat 1, 11, 12, XYLSHORT
  playPat 2, 12, 12, XYLSHORT
  waitRows 6
  playDrumPat 0, 13
  waitRows 84
  playDrumPat 0, 14
  segno
  waitRows 192
  dalSegno

PPDAT_0400_sq1:
  .byt INSTRUMENT, XYLSHORT, N_FH|D_8, N_EH|D_4, N_DH|D_D8
  .byt INSTRUMENT, HORNBLAT, N_DH, REST|D_2, REST|D_D4
  .byt INSTRUMENT, XYLSHORT, N_EH|D_8, N_DH|D_4, N_CH|D_D8
  .byt INSTRUMENT, HORNBLAT, N_CH, REST|D_2, REST|D_D4
  .byt INSTRUMENT, XYLSHORT, N_DH|D_8, N_CH|D_4, N_BB|D_D8
  .byt INSTRUMENT, HORNBLAT, N_BB, REST|D_2, REST|D_D4
  .byt INSTRUMENT, XYLSHORT, N_BB|D_8, N_A|D_4, N_G|D_D8
  .byt INSTRUMENT, HORNBLAT, N_DH, REST|D_2, REST|D_D4
  .byt $FF
PPDAT_0400_sq2:
  .byt INSTRUMENT, XYLSHORT, N_AH|D_D8, N_GH|D_D8, N_FH|D_D8
  .byt INSTRUMENT, HORNBLAT, N_FH, REST|D_2, REST|D_D4
  .byt INSTRUMENT, XYLSHORT, N_GH|D_D8, N_FH|D_D8, N_EH|D_D8
  .byt INSTRUMENT, HORNBLAT, N_EH, REST|D_2, REST|D_D4
  .byt INSTRUMENT, XYLSHORT, N_FH|D_D8, N_EH|D_D8, N_DH|D_D8
  .byt INSTRUMENT, HORNBLAT, N_DH, REST|D_2, REST|D_D4
  .byt INSTRUMENT, XYLSHORT, N_DH|D_D8, N_CH|D_D8, N_B|D_D8
  .byt INSTRUMENT, HORNBLAT, N_GH, REST|D_2, REST|D_D4
  .byt $FF
PPDAT_0400_drums_2:
  .byt PD_KICK|D_D8, PD_HAT|D_D8, PD_SNARE|D_D8, PD_HAT|D_8, PD_KICK
PPDAT_0400_drums:
  .byt PD_KICK|D_D8, PD_HAT|D_D4, PD_HAT|D_8, PD_KICK
  .byt $FF

;____________________________________________________________________
; 5am theme
;
; This is melodies from the third movement of Beethoven's
; "Pathetique" combined with rhythms from ACPG 5am.
; At the time I transcribed this, there were about 400 bytes left
; in the RODATA segment of the PRG ROM.

PSDAT_0500:
  setTempo 300
  playDrumPat 3, 2
    playPat 3, 13, 13, PLING_4
  playPat 2, 5, 24, PLING_4
    playDrumPat 1, 4
  playPat 1, 10, 19, PLING_4
  waitRows 96
  playPat 4, 21, 24, XYLMED
  playPat 3, 20, 0, BASSCLAR
  waitRows 190
  playPat 2, 22, 24, PLING_4
  playPat 1, 22, 19, PLING_4
  waitRows 2
  stopPat 3
  stopPat 4
  dalSegno

PPDAT_restore_vol:
  .byt CHVOLUME, 4, REST|D_D2
  .byt PATEND

PPDAT_0500_sqloop:
  .byt INSTRUMENT, PLING_4
  .byt REST|D_8, N_E, N_A|D_8, N_B, N_CSH|D_8, REST, N_DH|D_8, N_B|D_8
  .byt INSTRUMENT, PLING_8
  .byt REST, N_E, N_A|D_8, N_B, N_CSH|D_8, REST, N_DH|D_8, N_B|D_8
  .byt REST, REST|D_D4, REST|D_1
  .byt INSTRUMENT, PLING_4
  .byt REST|D_8, N_E, N_A|D_8, N_B, N_CSH|D_8, REST, N_B|D_8, N_G|D_8
  .byt INSTRUMENT, PLING_8
  .byt REST, N_E, N_A|D_8, N_B, N_CSH|D_8, REST, N_B|D_8, N_G|D_8
  .byt REST, REST|D_D4, CHVOLUME, 3, REST|D_1
  .byt PATEND
PPDAT_0500_triloop:
  .byt N_A|D_8, REST, N_G|D_8, N_FS|D_8, REST, N_G|D_8, REST|D_2
  .byt N_FS|D_8, REST, N_F|D_8, N_E|D_8
  .byt REST|D_D8, REST|D_4, REST|D_1
  .byt PATEND
PPDAT_0500_melody:
  .byt REST, N_CSH, N_DH, N_CSH, N_B, N_CSH, N_DH, N_EH|D_8, N_EH|D_8, N_EH|D_D8
  .byt REST|D_8, REST|D_1, REST|D_1
  .byt N_EH|D_8, N_CSH, N_DH|D_8, N_EH|D_D8, N_DH, N_CSH|D_8, N_FSH|D_D8
  .byt REST|D_8, REST|D_1, REST|D_1
  .byt N_CSH|D_8, N_A, N_B|D_8, N_CSH|D_D8, N_B, N_A|D_8, N_DH|D_D8
  .byt REST|D_8, REST|D_1, REST|D_1
  .byt N_E|D_D8, N_E|D_D8, N_E|D_D8, N_E|D_8, N_A|D_D8
  .byt REST|D_8, REST|D_1, REST|D_1
  .byt PATEND

;____________________________________________________________________
; round cleared theme

PSDAT_round_clear:
  setTempo 60
  playPat 0, 4, 12, PLING_2
  playPat 1, 4, 21, PLING_2
  waitRows 5
  fine

PPDAT_round_clear_sq1:
  .byt N_F, N_A, N_G, N_C|D_8
  .byt PATEND

;____________________________________________________________________
; 4pm theme

PSDAT_1600:
  setTempo 300
  playPat 1, 15, 12, XYLSHORT
  playPat 2, 16, 12, XYLSHORT
  playPat 3, 17, 0, BASSGUITAR
  playDrumPat 0, 18
  waitRows 48
  dalSegno

PPDAT_1600__sq2:
  .byt N_FH|D_D8, N_CH|D_D8, N_FH|D_D8, N_CH|D_8, N_FH
  .byt N_EBH|D_D8, N_BB|D_D8, N_EBH|D_D8, N_BB|D_8, N_EBH
  .byt N_FH|D_D8, N_DH|D_D8, N_FH|D_D8, N_DH|D_8, N_FH
  .byt N_GH|D_D8, N_DH|D_D8, N_GH|D_D8, N_CH|D_8, N_GH
  .byt PATEND
PPDAT_1600__sq1:
  .byt N_AH|D_D4, N_AH|D_D4
  .byt N_AH|D_D4, N_AH|D_D4
  .byt N_AH|D_D4, N_AH|D_4, N_TIE, N_AH
  .byt N_BBH|D_D4, N_BBH|D_4, N_TIE, N_BBH
  .byt PATEND
PPDAT_1600__tri:
  .byt N_FH|D_8, REST|D_D4, N_CH|D_8, REST|D_8
  .byt N_EBH|D_8, REST|D_D4, N_BB|D_8, REST|D_8
  .byt N_DH|D_8, REST|D_D4, N_A|D_8, REST|D_8
  .byt N_GH|D_8, REST|D_D4, N_CH|D_8, REST|D_8
  .byt PATEND
PPDAT_1600__drums:
  .byt PD_KICK|D_D8, PD_SNARE|D_D8, PD_HAT|D_8, PD_KICK, PD_SNARE|D_D8
  .byt PATEND
