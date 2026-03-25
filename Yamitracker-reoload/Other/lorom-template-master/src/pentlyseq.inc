.ifndef PENTLYSEQ_INC
PENTLYSEQ_INC = 1

; Instrument format
; $00: Left volume (bit 7-4) and right volume (bit 3-0),
;      used for balance and panning
; $01: Length of period in 16-sample units, or note 24 freq / 4186 Hz
; $02: Sample number
; $03: Attack and decay rates
; $04: Sustain threshold and rate
; $05-$07: Not used yet
.macro INST name, lvol, rvol, freqscale, samplenum, attack, decay, sustain, sustainlevel
name = (* - music_inst_table) / 8
.ident(.concat("PD_", .string(name))) = (* - music_inst_table)
  .assert (0 <= (lvol) && (lvol) <= 15), error, "left volume must be 0-15"
  .assert (0 <= (rvol) && (rvol) <= 15), error, "right volume must be 0-15"
  .assert (0 <= (rvol) && (rvol) <= 15), error, "right volume must be 0-15"
  .assert (1 <= (attack) && (attack) <= 31), error, "attack must be 1-31"
  .assert (attack & 1), error, "attack must be odd"
  .assert (16 <= (decay) && (decay) <= 31), error, "decay must be 17-31"
  .assert (0 <= (sustain) && (sustain) <= 31), error, "sustain must be 0-31"
  .assert (1 <= (sustainlevel) && (sustainlevel) <= 8), error, "sustainlevel must be 1-8"
  .byte ((lvol) << 4) | (rvol)
  .byte freqscale
  .byte samplenum
  .byte ((attack) >> 1) | (((decay) >> 1) << 4)
  .byte (sustain) | (((sustainlevel) - 1) << 5)
  .byte $00,$00,$00
.endmacro

CON_PLAYPAT = $00   ; next: pattern, transpose, instrument
CON_WAITROWS = $20  ; next: number of rows to wait minus 1
CON_FINE = $21      ; stop music now
CON_SEGNO = $22     ; set loop point
CON_DALSEGNO = $23  ; jump to loop point. if no point was set, jump to start of song.
CON_NOTEON = $28
CON_SETTEMPO = $30  ; low bits: bits 10-8 of tempo in rows/min; next: bits 7-0 of tempo

; Conductor macros
.macro playPat ch, patid, transpose, instrument
  .byt CON_PLAYPAT|ch, patid, transpose, instrument
.endmacro
.macro playDrumPat ch, patid
  .byt CON_PLAYPAT|ch, patid, $80, $80
.endmacro
.macro stopPat ch
  .byt CON_PLAYPAT|ch, 255, 0, 0
.endmacro
.macro waitRows n
  .byt CON_WAITROWS, (n)-1
.endmacro
.macro fine
  .byt CON_FINE
.endmacro
.macro segno
  .byt CON_SEGNO
.endmacro
.macro dalSegno
  .byt CON_DALSEGNO
.endmacro
.macro setTempo rowsPerMin
.local irpm
irpm = rowsPerMin
  .byt CON_SETTEMPO|>irpm, <irpm
.endmacro
.macro noteOn ch, notenum, instrument
  .byt CON_NOTEON|ch, notenum, instrument
.endmacro

; Pattern commands
N_C  =  0*8   ; До (C4 - средняя До) = 0
N_CS =  1*8   ; До-диез (C#4) = 8
N_D  =  2*8   ; Ре (D4) = 16
N_DS =  3*8   ; Ре-диез (D#4) = 24
N_E  =  4*8   ; Ми (E4) = 32
N_F  =  5*8   ; Фа (F4) = 40
N_FS =  6*8   ; Фа-диез (F#4) = 48
N_G  =  7*8   ; Соль (G4) = 56
N_GS =  8*8   ; Соль-диез (G#4) = 64
N_A  =  9*8   ; Ля (A4) = 72
N_AS = 10*8   ; Ля-диез (A#4) = 80
N_B  = 11*8   ; Си (B4) = 88
N_DB = N_CS   ; Ре-бемоль (Db4) = 8 (то же что C#)
N_EB = N_DS   ; Ми-бемоль (Eb4) = 24 (то же что D#)
N_GB = N_FS   ; Соль-бемоль (Gb4) = 48 (то же что F#)
N_AB = N_GS   ; Ля-бемоль (Ab4) = 64 (то же что G#)
N_BB = N_AS   ; Си-бемоль (Bb4) = 80 (то же что A#)
N_CH  = N_C  + 12*8  ; До второй октавы = 96
N_CSH = N_CS + 12*8  ; До# второй октавы = 104
N_DH  = N_D  + 12*8  ; Ре второй октавы = 112
N_DSH = N_DS + 12*8  ; Ре# второй октавы = 120
N_EH  = N_E  + 12*8  ; Ми второй октавы = 128
N_FH  = N_F  + 12*8  ; Фа второй октавы = 136
N_FSH = N_FS + 12*8  ; Фа# второй октавы = 144
N_GH  = N_G  + 12*8  ; Соль второй октавы = 152
N_GSH = N_GS + 12*8  ; Соль# второй октавы = 160
N_AH  = N_A  + 12*8  ; Ля второй октавы = 168
N_ASH = N_AS + 12*8  ; Ля# второй октавы = 176
N_BH  = N_B  + 12*8  ; Си второй октавы = 184
N_DBH = N_CSH ; Ре-бемоль второй октавы (Db5) = 104
N_EBH = N_DSH ; Ми-бемоль второй октавы (Eb5) = 120
N_GBH = N_FSH ; Соль-бемоль второй октавы (Gb5) = 144
N_ABH = N_GSH ; Ля-бемоль второй октавы (Ab5) = 160
N_BBH = N_ASH ; Си-бемоль второй октавы (Bb5) = 176
N_CHH = N_CH + 12*8  ; До третьей октавы (C6) = 192
N_TIE = 25*8  ; Связывание нот (продолжение предыдущей ноты) = 200
REST  = 26*8  ; Пауза (тишина) = 208
INSTRUMENT = $D8   ; Сменить инструмент
ARPEGGIO   = $D9   ; Арпеджио (быстрая последовательность нот аккорда)
LEGATO_OFF = $DA   ; Выключить легато (ноты отдельно)
LEGATO_ON  = $DB   ; Включить легато (ноты плавно перетекают)
TRANSPOSE  = $DC   ; Транспонировать (сдвинуть высоту всех нот)
GRACE      = $DD   ; Форшлаг (короткая нота перед основной)
VIBRATO    = $DE   ; Вибрато (периодическое изменение высоты)
CHVOLUME   = $DF   ; Установить громкость канала
PATEND = $FF  ; Конец паттерна (не нота, маркер)

; The default duration is one row (a sixteenth note in the tracker).
; OR the pitch with one of these constants.
D_8   = 1     ; Восьмая нота (1/8)
D_D8  = 2     ; Пунктирная восьмая (3/16)
D_4   = 3     ; Четвертная нота (1/4)
D_D4  = 4     ; Пунктирная четвертная (3/8)
D_2   = 5     ; Половинная нота (1/2)
D_D2  = 6     ; Пунктирная половинная (3/4)
D_1   = 7     ; Целая нота (1)

.global music_inst_table, pently_songs, pently_patterns
.endif
