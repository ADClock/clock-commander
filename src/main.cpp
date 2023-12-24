#include <Arduino.h>
#include "Config.h"
#include "FastGPIO.h"

char line[1024];
size_t lineIdx;

void sendInstruction(bool hourBackward, bool hourForward, bool minuteBackward, bool minuteForward)
{
  FastGPIO::Pin<DATA1_OUT>::setOutputValue(hourBackward);
  FastGPIO::Pin<DATA2_OUT>::setOutputValue(hourForward);
  FastGPIO::Pin<DATA3_OUT>::setOutputValue(minuteBackward);
  FastGPIO::Pin<DATA4_OUT>::setOutputValue(minuteForward);
  FastGPIO::Pin<CLOCK_OUT>::setOutputHigh();
  delayMicroseconds(MIN_OUT_CLOCK_HIGH);
  FastGPIO::Pin<CLOCK_OUT>::setOutputLow();
  delayMicroseconds(MIN_OUT_CLOCK_LOW);
}

void sendInstruction(char instruction)
{
  switch (instruction)
  {
  case 'h':
    sendInstruction(true, false, false, false);
    break;
  case 'H':
    sendInstruction(false, true, false, false);
    break;
  case 'm':
    sendInstruction(false, false, true, false);
    break;
  case 'M':
    sendInstruction(false, false, false, true);
    break;
  case 'b':
    sendInstruction(true, false, true, false);
    break;
  case 'B':
    sendInstruction(false, true, false, true);
    break;
  case 'o':
    sendInstruction(true, false, false, true);
    break;
  case 'O':
    sendInstruction(false, true, true, false);
    break;
  case 's':
    sendInstruction(false, false, false, false);
    break;
  case 'c':
    sendInstruction(true, true, true, true);
    break;
  default:
    Serial.println("Unknown instruction: " + String(instruction));
    break;
  }
}

void processInstructionLine(char *line)
{
  size_t repeatSingle = 1;
  size_t repeatBatchTimes = 0;
  size_t repeatBatchIndex = 0;
  size_t delay = MIN_STEP_DELAY;
  size_t clockCount;
  bool printSending = true; // Should the instruction send be printed?

  // Create array of all words in the string
  char *instructions[64];
  size_t instructionCount = 0;
  char *instruction = strtok(line, " ");
  while (instruction != NULL)
  {
    instructions[instructionCount] = instruction;
    instructionCount++;
    instruction = strtok(NULL, " ");
  }

  for (size_t i = 0; i < instructionCount; i++)
  {
    instruction = instructions[i];

    if (*instruction == 'x')
    {
      instruction++;
      repeatSingle = atoi(instruction);
      Serial.println("Repeat single instruction " + String(repeatSingle) + " times");
      continue;
    }

    if (*instruction == 'r')
    {
      if (repeatBatchTimes > 0)
      {
        repeatBatchTimes--;
        i = repeatBatchIndex;
        if ((repeatBatchTimes > 1000 && repeatBatchTimes % 100 == 0) || (repeatBatchTimes < 1000 && repeatBatchTimes % 50 == 0) || (repeatBatchTimes < 100 && repeatBatchTimes % 10 == 0) || repeatBatchTimes < 5)
          Serial.println("Repeat batch " + String(repeatBatchTimes) + " times left");
        if (printSending)
          printSending = false;
        continue;
      }
      instruction++;
      // set instruction multiplier to x times
      repeatBatchTimes = atoi(instruction);
      repeatBatchIndex = i;
      Serial.println("Repeat batch " + String(repeatBatchTimes) + " times");
      printSending = true;
      continue;
    }

    if (*instruction == 'd')
    {
      instruction++;
      delay = atoi(instruction);
      Serial.println("Set delay to " + String(delay) + " us");
      continue;
    }

    // Send instruction
    if (printSending)
      Serial.println("Sending instruction: " + String(instructions[i]) + " " + String(repeatSingle) + " times");
    for (size_t j = 0; j < repeatSingle; j++)
    {
      clockCount = 0;
      
      while (*instruction != '\0' && clockCount < CLOCKS_IN_ROW)
      {
        sendInstruction(*instruction);
        instruction++;
        clockCount++;

        if (*instruction == '.')
        {
          instruction = instructions[i];
        }
      }

      // Delay if it wasn't the last instruction
      if (i < instructionCount - 1 || j < repeatSingle - 1)
      {
        delayMicroseconds(delay - APPROX_SENDING_TIME_PER_INSTRUCTION * clockCount);
      }
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("ADClock starting ... For more information visit https://github.com/ADClock/clock ");
  Serial.flush();
  pinMode(CLOCK_OUT, OUTPUT);
  pinMode(DATA1_OUT, OUTPUT);
  pinMode(DATA2_OUT, OUTPUT);
  pinMode(DATA3_OUT, OUTPUT);
  pinMode(DATA4_OUT, OUTPUT);

  lineIdx = 0;
}

void loop()
{
  // Read line from Serial
  while (Serial.available() > 0)
  {
    line[lineIdx] = Serial.read();
    if (line[lineIdx] == '\n')
    {
      line[lineIdx] = '\0'; // Nullzeichen am Ende des Strings hinzufügen
      Serial.println("Processing line: " + String(line));
      processInstructionLine(line);
      Serial.println("Done");
      Serial.flush();
      lineIdx = 0;
    }
    else
      lineIdx++;
  }
}
