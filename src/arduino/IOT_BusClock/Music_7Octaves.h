#ifndef musicGen
#define musicGen


#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

// Start Music Notes
#define N_REST            0
#define N_C1             33
#define N_C1_SHARP       35
#define N_D1_FLAT        35
#define N_D1             37
#define N_D1_SHARP       39
#define N_E1_FLAT        39
#define N_E1             41
#define N_F1             44
#define N_F1_SHARP       46
#define N_G1_FLAT        46
#define N_G1             49    
#define N_G1_SHARP       52
#define N_A1_FLAT        52
#define N_A1             55
#define N_A1_SHARP       58
#define N_B1_FLAT        58
#define N_B1             62

#define N_C2             65
#define N_C2_SHARP       69
#define N_D2_FLAT        69
#define N_D2             73
#define N_D2_SHARP       78
#define N_E2_FLAT        78
#define N_E2             82
#define N_F2             87
#define N_F2_SHARP       93
#define N_G2_FLAT        93
#define N_G2             98    
#define N_G2_SHARP      104
#define N_A2_FLAT       104
#define N_A2            110
#define N_A2_SHARP      117
#define N_B2_FLAT       117
#define N_B2            123

#define N_C3            131
#define N_C3_SHARP      139
#define N_D3_FLAT       139
#define N_D3            147
#define N_D3_SHARP      156
#define N_E3_FLAT       156
#define N_E3            165
#define N_F3            175
#define N_F3_SHARP      185
#define N_G3_FLAT       185
#define N_G3            196    
#define N_G3_SHARP      208
#define N_A3_FLAT       208
#define N_A3            220
#define N_A3_SHARP      233
#define N_B3_FLAT       233
#define N_B3            247

#define N_C4              262
#define N_C4_SHARP    277
#define N_D4_FLAT       277
#define N_D4              294
#define N_D4_SHARP    311
#define N_E4_FLAT       311
#define N_E4              330
#define N_F4              349
#define N_F4_SHARP    370
#define N_G4_FLAT       370
#define N_G4              392    
#define N_G4_SHARP    415
#define N_A4_FLAT       415
#define N_A4              440
#define N_A4_SHARP    466
#define N_B4_FLAT       466
#define N_B4              494

#define N_C5              523
#define N_C5_SHARP    554
#define N_D5_FLAT       554
#define N_D5              587
#define N_D5_SHARP    622
#define N_E5_FLAT       622
#define N_E5              659
#define N_F5              698
#define N_F5_SHARP    740
#define N_G5_FLAT       740
#define N_G5              784    
#define N_G5_SHARP    831
#define N_A5_FLAT       831
#define N_A5              880
#define N_A5_SHARP    932
#define N_B5_FLAT       932
#define N_B5              988

#define N_C6              1047
#define N_C6_SHARP    1109
#define N_D6_FLAT       1109
#define N_D6              1175
#define N_D6_SHARP    1245
#define N_E6_FLAT       1245
#define N_E6              1319
#define N_F6              1397
#define N_F6_SHARP    1480
#define N_G6_FLAT       1480
#define N_G6              1568   
#define N_G6_SHARP    1661
#define N_A6_FLAT       1661
#define N_A6              1760
#define N_A6_SHARP    1865
#define N_B6_FLAT       1865
#define N_B6              1976

#define N_C7              2093
#define N_C7_SHARP    2217
#define N_D7_FLAT       2217
#define N_D7              2349
#define N_D7_SHARP    2489
#define N_E7_FLAT       2489
#define N_E7              2637
#define N_F7              2794
#define N_F7_SHARP    2960
#define N_G7_FLAT       2960
#define N_G7              3136   
#define N_G7_SHARP    3322
#define N_A7_FLAT       3322
#define N_A7              3520
#define N_A7_SHARP    3729
#define N_B7_FLAT     3729
#define N_B7              3951

#define N_C8              4186
#define N_C8_SHARP    4435
#define N_D8_FLAT       4435
#define N_D8              4699
#define N_D8_SHARP    4978
#define N_E8_FLAT       4978
#define N_E8              5274
#define N_F8              5588
#define N_F8_SHARP    5920
#define N_G8_FLAT     5920
#define N_G8            6272   
#define N_G8_SHARP    6645
#define N_A8_FLAT       6645
#define N_A8              7040
#define N_A8_SHARP    7459
#define N_B8_FLAT       7459
#define N_B8              7902

struct MelodyNote{
  uint16_t key;
  uint8_t duration;
  MelodyNote(uint16_t k, uint8_t d) : key(k), duration(d){}
};

class Melody {
  public:
    int note_count;
    MelodyNote notes[];
    Melody(MelodyNote m[],int c);
};

// End Music Notes
class MusicGenerator {
  public:
    bool currentlyPlaying = false;
    
    MelodyNote CScale_Notes[21] = {
       MelodyNote(N_C5,2),
       MelodyNote(N_D5,4),
       MelodyNote(N_E5,4),
       MelodyNote(N_F5,4),
       MelodyNote(N_G5,4),
       MelodyNote(N_A5,4),
       MelodyNote(N_B5,4),
       MelodyNote(N_C6,2),
       MelodyNote(N_B5,4),
       MelodyNote(N_A5,4),
       MelodyNote(N_G5,4),
       MelodyNote(N_F5,4),
       MelodyNote(N_E5,4),
       MelodyNote(N_D5,4),
       MelodyNote(N_C5,2),
       MelodyNote(N_E5,4),
       MelodyNote(N_G5,4),
       MelodyNote(N_C6,2),
       MelodyNote(N_G5,4),
       MelodyNote(N_E5,4),
       MelodyNote(N_C5,2),
    };

    MelodyNote DrankinPatna_Notes[64]{
       MelodyNote(N_C4,4),
       MelodyNote(N_D4,4),
       MelodyNote(N_E4,4),
       MelodyNote(N_B4_FLAT,4),
       MelodyNote(N_A4_FLAT,4),
       MelodyNote(N_G4,4),
       MelodyNote(N_F4,4),
       MelodyNote(N_G4,4),

       MelodyNote(N_C4,4),
       MelodyNote(N_D4,4),
       MelodyNote(N_E4,4),
       MelodyNote(N_B4_FLAT,4),
       MelodyNote(N_A4_FLAT,4),
       MelodyNote(N_G4,4),
       MelodyNote(N_F4,4),
       MelodyNote(N_G4,4),

       MelodyNote(N_C4,4),
       MelodyNote(N_D4,4),
       MelodyNote(N_E4,4),
       MelodyNote(N_B4_FLAT,4),
       MelodyNote(N_A4_FLAT,4),
       MelodyNote(N_G4,4),
       MelodyNote(N_F4,4),
       MelodyNote(N_G4,4),

       MelodyNote(N_C4,4),
       MelodyNote(N_D4,4),
       MelodyNote(N_E4,4),
       MelodyNote(N_B4_FLAT,4),
       MelodyNote(N_A4_FLAT,4),
       MelodyNote(N_G4,4),
       MelodyNote(N_F4,4),
       MelodyNote(N_G4,4),

     // First 4 measures finished
       MelodyNote(N_F4,4),
       MelodyNote(N_G4,4),
       MelodyNote(N_A4_FLAT,4),
       MelodyNote(N_B4_FLAT,4),
       MelodyNote(N_G5,4),
       MelodyNote(N_F5,4),
       MelodyNote(N_E5_FLAT,4),
       MelodyNote(N_D5,4),

       MelodyNote(N_F4,4),
       MelodyNote(N_G4,4),
       MelodyNote(N_A4_FLAT,4),
       MelodyNote(N_B4_FLAT,4),
       MelodyNote(N_G5,4),
       MelodyNote(N_F5,4),
       MelodyNote(N_E5_FLAT,4),
       MelodyNote(N_D5,4),

       MelodyNote(N_F4,4),
       MelodyNote(N_G4,4),
       MelodyNote(N_A4_FLAT,4),
       MelodyNote(N_B4_FLAT,4),
       MelodyNote(N_G5,4),
       MelodyNote(N_F5,4),
       MelodyNote(N_E5_FLAT,4),
       MelodyNote(N_D5,4),

       MelodyNote(N_F4,4),
       MelodyNote(N_G4,4),
       MelodyNote(N_A4_FLAT,4),
       MelodyNote(N_B4_FLAT,4),
       MelodyNote(N_G5,4),
       MelodyNote(N_F5,4),
       MelodyNote(N_E5_FLAT,4),
       MelodyNote(N_D5,4)
    };
    
    Melody CScale_melody = Melody(CScale_Notes, 21);
    //CScale_melody.notes = CScale_Notes;
    MusicGenerator();
    void PlayMelody1(MelodyNote melody[], int arr_size);
    void PlayMelody2(Melody melody, int pin);

  private:
};

#endif
