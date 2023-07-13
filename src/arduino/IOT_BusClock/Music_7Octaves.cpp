#include "Music_7Octaves.h"


Melody::Melody(MelodyNote m[],int c){
      note_count = c;
      for (int i; i < c; i++){
        //notes[i] = m[i];
      }
}

MusicGenerator::MusicGenerator(){
  // default code
  //Serial.println("Created Music Generator.");
}
void MusicGenerator::PlayMelody2(Melody melody, int pin) {
  // iterate over the notes of the melody:
  Serial.print("count: ");
  Serial.println(melody.note_count);
  for (int index = 0; index < melody.note_count; index++) {
    
    //Serial.println("count: ");
    //Serial.println(melody.note_count);
    //Serial.println("duration: ");
    //Serial.println(melody[index].duration);
    /*
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / melody.notes[index].duration;
    tone(pin, melody.notes[index].key, noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(pin);*/
  }
}

void MusicGenerator::PlayMelody1(MelodyNote melody[], int arr_size, MelodySpeed melodySpeed) {
  // iterate over the notes of the melody:
  for (int index = 0; index < arr_size; index++) {
    if (currentlyPlaying){
  
      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      uint16_t seconds = 1000;
      switch (melodySpeed) {
        case NORMAL_TIME:
          seconds = 1000;
          break;
        case HALF_TIME:
          seconds = 2000;
          break;
        case DOUBLE_TIME:
          seconds = 500;
          break;
      }
      int noteDuration = seconds / melody[index].duration;
      tone(D0, melody[index].key, noteDuration);
  
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(D0);
    } else {
      //killing melody playback
      break;
    }
  }
}
