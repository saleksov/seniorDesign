#include "handleinput.h"

static void settingsSongPlayer(void);
static void settingsMetronome(void);
static uint8_t getUserNumber(void);
static void settingsInstrument(void);

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

// D -> Octave Settings
// * -> confirm
// # -> go back 
// When Button 3 is pressed i need to halt the harp, save where it is at (at least halt the song)
// and later continue where we left off

void settings(void)
{
  if ((GPIOC->IDR) & (1 << 3))      // Button3 -> enter settings
    bufferButton3 = 0;
  else if (bufferButton3 == 0)
  {
    // In Settings Now
    bufferButton3 = 1;

    char PressedKey = '!';

    PressedKey = get_keypress();

    while(PressedKey != '#')
    {
      switch (PressedKey)
      {
      case 'A':                   // A -> Song Settings
        settingsSongPlayer();
      break;

      
      case 'B':                   // B -> Metronome Settigns
        settingsMetronome();
      break;
      
      case 'C':                   // C -> Instrument Settings
        settingsInstrument();
      break;

      default:
        break;
      }
      // Check C
      // Check D
      // Get another Press
      PressedKey = get_keypress();
    }
  }
}

// Numbers 0-9 -> song sellect
// * is confirm, # is back

static void settingsSongPlayer(void)
{
  char PressedKey = '!';
  PressedKey = get_keypress();
  while (PressedKey != '#')
  {
    switch (PressedKey)
    {
    case 'A':             // A is Stop / Start
      if (songPlaying)
        stopSong();
      else
        startSong();
    break;

    case 'B':             // B is Resume / Pause
      if (songPlaying)
        pauseSong();
      else
        resumeSong();
    break;

    case 'C':             // C is Play Next Song
        //  Implement play next song
    break;
    
    case 'D':             // D is sellect song 0 - 9;
        next_tick += 5000;
    break;
        // Implement Sellect song 0 -9;
    default:
      break;
    }
    PressedKey = get_keypress();
  }
}

static void settingsMetronome(void)
{
  char PressedKey = '!';
  uint8_t UserNumber = 0;
  PressedKey = get_keypress();
  while (PressedKey != '#')
  {
    switch (PressedKey)
    {
    case 'A':             // A -> ON / OFF
      if(Mstatus)
        Mstatus = 0;
      else
        Mstatus = 1;
    break;

    case 'B':             // B -> Set BPM
      UserNumber = getUserNumber();
      BPM = UserNumber;
      set_bpm();
    break;

    case 'C':             // C -> Metro Signature
      UserNumber = getUserNumber();
      if (UserNumber > 4)
        UserNumber = 4;
      else if (UserNumber < 1)
        UserNumber = 1;
      metroSig = UserNumber;
      metroSigCounter = 0;
    break;

    default:
    break;
    }
    PressedKey = get_keypress();
  }
}

static void settingsInstrument(void)
{
  char PressedKey = '!';
  uint8_t UserNumber = 0;
  PressedKey = get_keypress();
  while (PressedKey != '#')
  {
    switch (PressedKey)
    {
    case 'A':                       // A -> change instrument song
    UserNumber = getUserNumber();
    if (UserNumber > 128)
      UserNumber = 128;
    else if (UserNumber < 1)
      UserNumber = 1;
    UserNumber--;
    changeInstrument(0, UserNumber);
    break;

    // case 'B':                       // B -> change instrument strings

    // break;
    
    case 'C':         // C -> Octave Settings Set octave step (1-12), incriments all notes by octave step
    UserNumber = getUserNumber();
    if (UserNumber > 12)
      UserNumber = 12;
    else if (UserNumber < 1)
      UserNumber = 1;

    octaveStep = UserNumber;
    break;

    default:
      break;
    }
    PressedKey = get_keypress();
  }
}

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

uint8_t getUserNumber(void)
{
  char PressedKey = '!';
  int userNumber = 0;
  uint8_t maxDigits = 0;
  
  while (PressedKey != '#' && maxDigits < 3)
  {
    PressedKey = get_keypress();
    if (PressedKey >= '0' && PressedKey <= '9')
    {
      debugSendNote();
      userNumber = userNumber * 10 + PressedKey - '0';
      maxDigits++;
      if (userNumber == 0)
        maxDigits--;
    }
  }

  if (userNumber > 240)
    userNumber = 240;
  return userNumber;
}