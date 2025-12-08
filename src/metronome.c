#include "metronome.h"

void init_tim6_metronome(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

  TIM6->PSC = 48000 - 1;
  TIM6->ARR = (60000 / BPM) - 1;

  TIM6->DIER |= TIM_DIER_UIE;
  TIM6->CR1  |= TIM_CR1_CEN;

  NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

void TIM6_DAC_IRQHandler(void) {
  TIM6->SR &= ~TIM_SR_UIF; // clear interrupt flag
  
  if (Mstatus)
    metroSignature();     
}

// void toggleMetronome(void)
// {
//   if (Mstatus)
//     Mstatus = 1;
//   else
//     Mstatus = 0;
//   // if (Mstatus)
//   // {
//   //   TIM6->CNT = 0;              // reset counter
//   //   TIM6->SR &= ~TIM_SR_UIF;    // clear flag
//   //   TIM6->CR1 |= TIM_CR1_CEN;   // enable counter
//   //   dispUI(octaves[octave],'Y', modes[modeidx]);
//   // }
//   // else{
//   //   TIM6->CR1 &= ~TIM_CR1_CEN;
//   //   dispUI(octaves[octave],'N', modes[modeidx]);
//   // }
// }

void set_bpm(void) {
  TIM6->CR1 &= ~TIM_CR1_CEN;
  if (BPM > 0)
    TIM6->ARR = (60000 / BPM) - 1;
  else
  {
    BPM = 60;
    TIM6->ARR = (60000 / BPM) - 1;
    Mstatus = 0;
  }
    
  TIM6->CNT = 0;
  TIM6->SR &= ~TIM_SR_UIF;
  TIM6->CR1 |= TIM_CR1_CEN;
  metroSigCounter = 0;
}

void metroSignature(void)
{
  switch (metroSig)
  {
  case 1: // 4/4
    if (metroSigCounter)
      sendMIDI(METRONOME_CHANNEL, 84, metronome_volume); // Note On 
    else
      sendMIDI(METRONOME_CHANNEL, 72, metronome_volume);
    metroSigCounter++;
    if (metroSigCounter > 3)
      metroSigCounter = 0;
  break;
  
  case 2: // 3/4
    if (metroSigCounter)
      sendMIDI(METRONOME_CHANNEL, 84, metronome_volume); // Note On 
    else
      sendMIDI(METRONOME_CHANNEL, 72, metronome_volume);
    metroSigCounter++;
    if (metroSigCounter > 2)
      metroSigCounter = 0;
  break;
  
  case 3: // 5/4
    if (metroSigCounter)
      sendMIDI(METRONOME_CHANNEL, 84, metronome_volume); // Note On 
    else
      sendMIDI(METRONOME_CHANNEL, 72, metronome_volume);
    metroSigCounter++;
    if (metroSigCounter > 4)
      metroSigCounter = 0;
  break;
  
  case 4: // 7/8
    if (metroSigCounter)
      sendMIDI(METRONOME_CHANNEL, 84, metronome_volume); // Note On 
    else
      sendMIDI(METRONOME_CHANNEL, 72, metronome_volume);
    metroSigCounter++;
    if (metroSigCounter > 6)
      metroSigCounter = 0;
  break;
  
  }
}

