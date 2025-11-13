#include "handleinput.h"

void strings(void)
{
  // String 0 - Output is B11
  if ((GPIOA->IDR) & (1 << 0))
    bufferString00 = 0;
  else if (bufferString00 == 0)
  {
    sendMIDI(0, note00, 100);
    bufferString00 = 1;
  }

  if ((GPIOA->IDR) & (1 << 1))
    bufferString01 = 0;
  else if (bufferString01 == 0)
  {
    sendMIDI(0, note01, 100);
    bufferString01 = 1;
  }

  if ((GPIOA->IDR) & (1 << 2))
    bufferString02 = 0;
  else if (bufferString02 == 0)
  {
    sendMIDI(0, note02, 100);
    bufferString02 = 1;
  }

  if ((GPIOA->IDR) & (1 << 3))
    bufferString03 = 0;
  else if (bufferString03 == 0)
  {
    sendMIDI(0, note03, 100);
    bufferString03 = 1;
  }

  if ((GPIOA->IDR) & (1 << 4))
    bufferString04 = 0;
  else if (bufferString04 == 0)
  {
    sendMIDI(0, note04, 100);
    bufferString04 = 1;
  }

  if ((GPIOA->IDR) & (1 << 5))
    bufferString05 = 0;
  else if (bufferString05 == 0)
  {
    sendMIDI(0, note05, 100);
    bufferString05 = 1;
  }

  if ((GPIOA->IDR) & (1 << 6))
    bufferString06 = 0;
  else if (bufferString06 == 0)
  {
    sendMIDI(0, note06, 100);
    bufferString06 = 1;
  }

  if ((GPIOA->IDR) & (1 << 7))
    bufferString07 = 0;
  else if (bufferString07 == 0)
  {
    sendMIDI(0, note07, 100);
    bufferString07 = 1;
  }

  if ((GPIOA->IDR) & (1 << 8))
    bufferString08 = 0;
  else if (bufferString08 == 0)
  {
    sendMIDI(0, note08, 100);
    bufferString08 = 1;
  }

  if ((GPIOA->IDR) & (1 << 9))
    bufferString09 = 0;
  else if (bufferString09 == 0)
  {
    sendMIDI(0, note09, 100);
    bufferString09 = 1;
  }

  if ((GPIOA->IDR) & (1 << 10))
    bufferString10 = 0;
  else if (bufferString10 == 0)
  {
    sendMIDI(0, note10, 100);
    bufferString10 = 1;
  }

  if ((GPIOA->IDR) & (1 << 11))
    bufferString11 = 0;
  else if (bufferString11 == 0)
  {
    sendMIDI(0, note11, 100);
    bufferString11 = 1;
  }
}

void buttons()
{
  // Octave Down
  if ((GPIOC->IDR) & (1 << 0))
    bufferButton0 = 0;
  else if (bufferButton0 == 0)
  {
    bufferButton0 = 1;
    if (octave > 35 )
    {
      octave -= octaveStep;
      setOctave();

      //dispUI(octaves[octave], metros[metroidx], modes[modeidx]);
    }
  }

  // Octave Up
  if ((GPIOC->IDR) & (1 << 1))
    bufferButton1 = 0;
  else if (bufferButton1 == 0)
  {
    bufferButton1 = 1;
    if (octave < 97) // min is 1
    {
      octave += octaveStep;
      setOctave();

      //dispUI(octaves[octave], metros[metroidx], modes[modeidx]);
    }
  }

  // Clear String Sound
  if ((GPIOC->IDR) & (1 << 2))
    bufferButton2 = 0;
  else if (bufferButton2 == 0)
  {
    bufferButton2 = 1;
    allNotesOff(0);
  }
}

void settings(void)
{
  if ((GPIOC->IDR) & (1<<3))
  {
    bufferButton3 = 1;
  }
}

// {
//   bufferSettings0 = 1;
//   while ((GPIOC->IDR) & (1))
//     bufferSettings0 = 1;

//   int loop = 1;
//   while (loop)
//   {
//     if ((GPIOC->IDR) & (1)){
//       bufferSettings0 = 0;
//     }
//     else if (bufferSettings0 == 0)
//     {
//       bufferSettings0 = 1;
//       loop = 0;
//     }

//     if ((GPIOC->IDR) & (1 << 2))
//       bufferOctave0 = 0;
//     else if (bufferOctave0 == 0)
//     {
//       bufferOctave0 = 1;
//       BPM += 10;
//       if ((BPM != 0) && (Mstatus == 0))
//       {
//         Mstatus = 1;
//         toggleMetronome();
//       }
//       if (BPM > 220)
//         BPM = 220; 
//       set_bpm();
//     }

//     if ((GPIOC->IDR) & (1 << 3))
//       bufferOctave1 = 0;
//     else if (bufferOctave1 == 0)
//     {
//       bufferOctave1 = 1;
//       if (BPM != 0)
//         BPM -= 10;
//       if (BPM == 0)
//       {
//         Mstatus = 0;
//         toggleMetronome();
//       }
//       set_bpm();
//     }
//   }

//   bufferOctave0 = 0;
//   bufferOctave1 = 0;
// }
