; ÐÑÐ·ÑÐºÐ°Ð»ÑÐ½Ð°Ñ ÐºÐ¾Ð¼Ð¿Ð¾Ð·Ð¸ÑÐ¸Ñ, ÑÐ³ÐµÐ½ÐµÑÐ¸ÑÐ¾Ð²Ð°Ð½Ð½Ð°Ñ Yamitracker
; Ð¤Ð¾ÑÐ¼Ð°Ñ: Pently-like Ñ Ð´Ð¾ÑÐ¾Ð¶ÐºÐ°Ð¼Ð¸
; ÐÑÐµÐ¼Ñ ÑÐ¾Ð·Ð´Ð°Ð½Ð¸Ñ: 2025-12-13 11:08:33
; ÐÐ¾Ð»Ð¸ÑÐµÑÑÐ²Ð¾ Ð´Ð¾ÑÐ¾Ð¶ÐµÐº: 2

.include "pentlyseq.inc"
.segment "SPCIMAGE"

; Ð¢Ð°Ð±Ð»Ð¸ÑÐ° Ð¸Ð½ÑÑÑÑÐ¼ÐµÐ½ÑÐ¾Ð²
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
PPDAT_track0_pattern0:
  .byt N_GHH|D_8
  .byt N_EHH|D_8
  .byt N_AHH|D_8
  .byt N_EHH|D_8
  .byt N_GHH|D_8
  .byt N_EHH|D_8
  .byt N_AHH|D_D8
  .byt N_EHH|D_8
  .byt N_GHH|D_8
  .byt N_EHH|D_8
  .byt N_AHH|D_8
  .byt N_EHH|D_8
  .byt N_GHH|D_8
  .byt N_EHH|D_8
  .byt N_AHH|D_8
  .byt N_EHH|D_8
  .byt N_GHH|D_D8
  .byt N_EHH|D_8
  .byt N_AHH|D_D8
  .byt N_EHH|D_D8
  .byt N_GHH|D_D8
  .byt PATEND
PPDAT_track0_pattern1:
  .byt N_CHH|D_2
  .byt PATEND
PPDAT_track0_pattern2:
  .byt N_CHH|D_4
  .byt N_CHH|D_8
  .byt N_CHH|D_8
  .byt PATEND
PPDAT_track0_pattern3:
  .byt N_A|D_2
  .byt N_DHH|D_2
  .byt N_EH|D_D4
  .byt N_BHH|D_D4
  .byt PATEND
PPDAT_track0_pattern4:
  .byt N_A|D_2
  .byt N_EH|D_D4
  .byt N_DHH|D_D4
  .byt N_BHH|D_D4
  .byt PATEND
PPDAT_track0_pattern5:
  .byt N_BHH|D_1
  .byt N_EH|D_1
  .byt N_A|D_1
  .byt N_DHH|D_1
  .byt N_G|D_D16
  .byt N_GS|D_D16
  .byt PATEND
PPDAT_track0_pattern6:
  .byt N_EH|D_8
  .byt N_DHH|D_D8
  .byt N_EH|D_D8
  .byt N_DHH|D_D8
  .byt N_BHH|D_8
  .byt N_EH|D_D8
  .byt N_DHH|D_D8
  .byt N_A|D_8
  .byt N_EH|D_8
  .byt N_BHH|D_8
  .byt N_DHH|D_8
  .byt N_EH|D_D8
  .byt N_A|D_8
  .byt N_EH|D_8
  .byt N_A|D_8
  .byt N_EH|D_D16
  .byt N_A|D_8
  .byt N_EH|D_D16
  .byt N_A|D_8
  .byt N_DHH|D_16
  .byt N_CHH|D_D32
  .byt N_A|D_8
  .byt N_BHH|D_D16
  .byt N_CHH|D_8
  .byt N_EH|D_D16
  .byt N_A|D_8
  .byt N_BHH|D_8
  .byt N_EH|D_8
  .byt N_CHH|D_D16
  .byt N_A|D_8
  .byt N_BHH|D_8
  .byt N_CHH|D_D8
  .byt N_EH|D_8
  .byt N_A|D_D8
  .byt N_BHH|D_8
  .byt PATEND
PPDAT_track1_pattern0:
  .byt N_CHH|D_D16
  .byt N_CHH|D_16
  .byt N_CHH|D_16
  .byt N_CHH|D_16
  .byt N_CHH|D_D32
  .byt N_CHH|D_D32
  .byt N_CHH|D_32
  .byt N_CHH|D_32
  .byt N_CHH|D_32
  .byt N_CHH|D_32
  .byt N_CHH|D_D32
  .byt N_CHH|D_D32
  .byt N_CHH|D_32
  .byt N_CHH|D_D32
  .byt N_CHH|D_D32
  .byt N_CHH|D_D32
  .byt N_CHH|D_D32
  .byt N_CHH|D_D32
  .byt N_CHH|D_D32
  .byt N_CHH|D_32
  .byt N_CHH|D_32
  .byt N_CHH|D_32
  .byt N_CHH|D_D32
  .byt N_CHH|D_16
  .byt N_CHH|D_D32
  .byt PATEND
PPDAT_track1_pattern1:
  .byt N_EH|D_8
  .byt N_CHH|D_D16
  .byt PATEND
PPDAT_track1_pattern2:
  .byt N_CHH|D_1
  .byt N_AH|D_D2
  .byt N_E|D_D2
  .byt N_D|D_D2
  .byt N_F|D_D2
  .byt N_C|D_2
  .byt PATEND
PPDAT_track1_pattern3:
  .byt N_AH|D_D4
  .byt N_BH|D_4
  .byt N_D|D_D4
  .byt N_C|D_4
  .byt N_E|D_4
  .byt PATEND
PPDAT_track1_pattern4:
  .byt N_CHH|D_D4
  .byt N_DHH|D_D4
  .byt N_C|D_4
  .byt N_BHH|D_4
  .byt PATEND
PPDAT_track1_pattern5:
  .byt N_FHH|D_4
  .byt N_GHH|D_8
  .byt N_EHH|D_32
  .byt PATEND

pently_songs:
.addr  PSDAT_composition

; ÐÑÐ½Ð¾Ð²Ð½Ð°Ñ ÐºÐ¾Ð¼Ð¿Ð¾Ð·Ð¸ÑÐ¸Ñ
PSDAT_composition:
  setTempo 300

  playPat 0, 0, 0, PLING_2
  playPat 0, 1, 0, PLING_2
  playPat 0, 2, 0, PLING_2
  playPat 0, 3, 0, PLING_2
  playPat 0, 4, 0, PLING_2
  playPat 0, 5, 0, PLING_2
  playPat 0, 6, 0, PLING_2
  waitRows 16
  playPat 1, 7, 0, PLING_2
  playPat 1, 8, 0, PLING_2
  playPat 1, 9, 0, PLING_2
  playPat 1, 10, 0, PLING_2
  playPat 1, 11, 0, PLING_2
  playPat 1, 12, 0, PLING_2

  ; ÐÐ°Ð²ÐµÑÑÐµÐ½Ð¸Ðµ ÐºÐ¾Ð¼Ð¿Ð¾Ð·Ð¸ÑÐ¸Ð¸
  waitRows 32
  fine
