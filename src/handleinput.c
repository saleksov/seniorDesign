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
    allNotesOff(STRINGS_CHANNEL);
    allNotesOff(SONGPLAYER_CHANNEL);
    allNotesOff(METRONOME_CHANNEL);
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

    while(PressedKey != '#')
    {
      disp_keypad();
      PressedKey = get_keypress();
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

      //case 'D':                   // D -> volume settings
        //settingsVolume();
      //break;

      default:
        break;
      }
      // Check C
      // Check D
      // Get another Press
    }
    disp_harp_Menu1();
  }
}

// Numbers 0-9 -> song sellect
// * is confirm, # is back

static void settingsSongPlayer(void)
{
  char PressedKey = '!';
  //while (PressedKey != '#')
  //{
    disp_song_Set();
    PressedKey = get_keypress();
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
        skipToNextSong();
    break;
        
    default:

    if (PressedKey >= '0' && PressedKey <= '9')
      playsongFromKey(PressedKey);
    break;
    }
  }
//}

static void settingsMetronome(void)
{
  char PressedKey = '!';
  uint8_t UserNumber = 0;
  //while (PressedKey != '#')
  //{
    disp_metronome_Set();
    PressedKey = get_keypress();
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
      disp_signature_Set();
      PressedKey = get_keypress();
      switch (PressedKey)
      {
      case 'A':
        metroSig = 1; // 4/4
      break;
      case 'B':
        metroSig = 2; // 3/4
      break;
      case 'C':
        metroSig = 3; // 5/4
      break;
      case 'D':
        metroSig = 4; // 7/8
      break;

      default:
      break;
      }
    break;

    default:
    break;
    //}
  }
}

static void settingsInstrument(void)
{
  char PressedKey = '!';
  uint8_t UserNumber = 0;
  //while (PressedKey != '#')
  //{
  disp_instrument_Set();
  PressedKey = get_keypress();
  switch (PressedKey)
    {
    case 'A':                       // A -> change instrument song
    disp_changeInstrument();
    PressedKey = get_keypress();

    UserNumber = getUserNumber();
    if (UserNumber > 128)
      UserNumber = 128;
    else if (UserNumber < 1)
      UserNumber = 1;
    UserNumber--;

    switch (PressedKey)
    {
    case 'A':         // A -> Song Player Instrument
      changeInstrument(SONGPLAYER_CHANNEL, UserNumber);
    break;

    case 'B':         // B -> Metronome Instrument
      changeInstrument(METRONOME_CHANNEL, UserNumber);
    break;

    case 'C':         // C -> Strings Instrument
      changeInstrument(STRINGS_CHANNEL, UserNumber);
    break;
    
    default:
    break;
    }
    break;

    case 'B':                       // B -> Volunme Settings

    disp_changeInstrument();
    PressedKey = get_keypress();

    UserNumber = getUserNumber();
    if (UserNumber > 128)
      UserNumber = 128;
    else if (UserNumber < 1)
      UserNumber = 1;
    UserNumber--;

    switch (PressedKey)
    {
    case 'A':
      songPlayer_volume = UserNumber;
    break;

    case 'B':
      metronome_volume = UserNumber;
    break;

    case 'C':
      strings_volume = UserNumber;
    break;
    
    default:
    break;
    }

    break;
    
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
  }
//}

void strings(void)
{
  // String 0 - Output is B11
  if ((GPIOA->IDR) & (1 << 0))
    bufferString00 = 0;
  else if (bufferString00 == 0)
  {
    sendMIDI(STRINGS_CHANNEL, note00, strings_volume);
    bufferString00 = 1;
  }

  if ((GPIOA->IDR) & (1 << 1))
    bufferString01 = 0;
  else if (bufferString01 == 0)
  {
    sendMIDI(STRINGS_CHANNEL, note01, strings_volume);
    bufferString01 = 1;
  }

  if ((GPIOA->IDR) & (1 << 2))
    bufferString02 = 0;
  else if (bufferString02 == 0)
  {
    sendMIDI(STRINGS_CHANNEL, note02, strings_volume);
    bufferString02 = 1;
  }

  if ((GPIOA->IDR) & (1 << 3))
    bufferString03 = 0;
  else if (bufferString03 == 0)
  {
    sendMIDI(STRINGS_CHANNEL, note03, strings_volume);
    bufferString03 = 1;
  }

  if ((GPIOA->IDR) & (1 << 4))
    bufferString04 = 0;
  else if (bufferString04 == 0)
  {
    sendMIDI(STRINGS_CHANNEL, note04, strings_volume);
    bufferString04 = 1;
  }

  if ((GPIOA->IDR) & (1 << 5))
    bufferString05 = 0;
  else if (bufferString05 == 0)
  {
    sendMIDI(STRINGS_CHANNEL, note05, strings_volume);
    bufferString05 = 1;
  }

  if ((GPIOA->IDR) & (1 << 6))
    bufferString06 = 0;
  else if (bufferString06 == 0)
  {
    sendMIDI(STRINGS_CHANNEL, note06, strings_volume);
    bufferString06 = 1;
  }

  if ((GPIOA->IDR) & (1 << 7))
    bufferString07 = 0;
  else if (bufferString07 == 0)
  {
    sendMIDI(STRINGS_CHANNEL, note07, strings_volume);
    bufferString07 = 1;
  }

  if ((GPIOA->IDR) & (1 << 8))
    bufferString08 = 0;
  else if (bufferString08 == 0)
  {
    sendMIDI(STRINGS_CHANNEL, note08, strings_volume);
    bufferString08 = 1;
  }

  if ((GPIOA->IDR) & (1 << 9))
    bufferString09 = 0;
  else if (bufferString09 == 0)
  {
    sendMIDI(STRINGS_CHANNEL, note09, strings_volume);
    bufferString09 = 1;
  }

  if ((GPIOA->IDR) & (1 << 10))
    bufferString10 = 0;
  else if (bufferString10 == 0)
  {
    sendMIDI(STRINGS_CHANNEL, note10, strings_volume);
    bufferString10 = 1;
  }

  if ((GPIOA->IDR) & (1 << 11))
    bufferString11 = 0;
  else if (bufferString11 == 0)
  {
    sendMIDI(STRINGS_CHANNEL, note11, strings_volume);
    bufferString11 = 1;
  }
}

uint8_t getUserNumber(void)
{
  char PressedKey = '!';
  int userNumber = 0;
  uint8_t maxDigits = 0;
  
  disp_enterNumber();
  while (PressedKey != '#' && maxDigits < 3)
  {
    PressedKey = get_keypress();
    if (PressedKey >= '0' && PressedKey <= '9')
    {
      userNumber = userNumber * 10 + PressedKey - '0';
      maxDigits++;
      if (userNumber == 0)
        maxDigits--;
      else
        disp_number(userNumber % 10, maxDigits);
    }
  }

  if (userNumber > 240)
    userNumber = 240;
  return userNumber;
}