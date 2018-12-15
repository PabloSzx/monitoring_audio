#include "audio.hpp"
#include <bits/stdc++.h>

using namespace std;

Audio::Audio() {
  Pa_Initialize();
  printf("PortAudio version number = %d\nPortAudio version text = '%s'\n",
         Pa_GetVersion(), Pa_GetVersionText());
  numDevices = Pa_GetDeviceCount();
}

void Audio::close(int i = 0) {
  cout << "CLOSE CALLED " << i << endl;
  // Pa_Terminate();
  // if (data.recordedSamples) /* Sure it is NULL or valid. */
  //   free(data.recordedSamples);
  // if (err != paNoError) {
  //   fprintf(stderr, "An error occured while using the portaudio stream\n");
  //   fprintf(stderr, "Error number: %d\n", err);
  //   fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
  //   err = 1; /* Always return 0 or 1, but no other return codes. */
  // }
  return;
}

void Audio::play() {
  /* Playback recorded data.  -------------------------------------------- */
  data.frameIndex = 0;

  outputParameters.device =
      Pa_GetDefaultOutputDevice(); /* default output device */
  if (outputParameters.device == paNoDevice) {
    fprintf(stderr, "Error: No default output device.\n");
    close();
    return;
  }
  outputParameters.channelCount = 2; /* stereo output */
  outputParameters.sampleFormat = PA_SAMPLE_TYPE;
  outputParameters.suggestedLatency =
      Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  printf("\n=== Now playing back. ===\n");
  fflush(stdout);
  err = Pa_OpenStream(&stream, NULL, /* no input */
                      &outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER,
                      paClipOff, /* we won't output out of range samples so
                                    don't bother clipping them */
                      playCallback, &data);
  if (err != paNoError) {
    close(52);
    return;
  }

  if (stream) {
    err = Pa_StartStream(stream);
    if (err != paNoError) {
      close(59);
      return;
    }

    printf("Waiting for playback to finish.\n");
    fflush(stdout);

    while ((err = Pa_IsStreamActive(stream)) == 1)
      Pa_Sleep(100);
    cout << err << endl;
    if (err < 0) {
      close(69);
      return;
    }

    // err = Pa_CloseStream(stream);

    if (err != paNoError) {
      close(75);
      return;
    }

    printf("Done.\n");
    fflush(stdout);
  }

  close();
}

void Audio::record(int n) {
  cout << "RECORDING CALLED" << endl;
  /* RECORDING */
  totalFrames = n * SAMPLE_RATE * NUM_CHANNELS;
  data.maxFrameIndex = totalFrames =
      n * SAMPLE_RATE; /* Record for a few seconds. */
  data.frameIndex = 0;
  numSamples = totalFrames * NUM_CHANNELS;
  numBytes = numSamples * sizeof(SAMPLE);
  // data.recordedSamples = (SAMPLE *)malloc(
  //     numBytes); /* From now on, recordedSamples is initialised. */
  // if (data.recordedSamples == NULL) {
  //   printf("Could not allocate record array.\n");
  //   close(95);
  //   return;
  // }
  // for (int i = 0; i < numSamples; i++)
  //   data.recordedSamples[i] = 0;
  data.recordedSamples.resize(data.recordedSamples.capacity() + numSamples, 0);

  err = Pa_Initialize();
  if (err != paNoError) {
    close(103);
    return;
  }

  inputParameters.device =
      Pa_GetDefaultInputDevice(); /* default input device */
  if (inputParameters.device == paNoDevice) {
    fprintf(stderr, "Error: No default input device.\n");
    close(111);
    return;
  }
  inputParameters.channelCount = 2; /* stereo input */
  inputParameters.sampleFormat = PA_SAMPLE_TYPE;
  inputParameters.suggestedLatency =
      Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  /* Record some audio. -------------------------------------------- */
  err = Pa_OpenStream(&stream, &inputParameters, NULL, /* &outputParameters, */
                      SAMPLE_RATE, FRAMES_PER_BUFFER,
                      paClipOff, /* we won't output out of range samples so
                                    don't bother clipping them */
                      recordCallback, &data);
  if (err != paNoError) {
    close(127);
    return;
  }

  err = Pa_StartStream(stream);
  if (err != paNoError) {
    close(133);
    return;
  }
  printf("\n=== Now recording!! Please speak into the microphone. ===\n");
  fflush(stdout);

  while ((err = Pa_IsStreamActive(stream)) == 1) {
    Pa_Sleep(1000);
    printf("index = %d\n", data.frameIndex);
    fflush(stdout);
  }
  if (err < 0) {
    close(145);
    return;
  }

  err = Pa_CloseStream(stream);
  if (err != paNoError) {
    close(151);
    return;
  }

  /* Measure maximum peak amplitude. */
  max = 0;
  average = 0.0;
  for (auto val : data.recordedSamples) {
    // for (int i = 0; i < numSamples; i++) {
    // val = data.recordedSamples[i];
    if (val < 0)
      val = -val; /* ABS */
    if (val > max) {
      max = val;
    }
    average += val;
  }

  average = average / (double)numSamples;

  printf("sample max amplitude = " PRINTF_S_FORMAT "\n", max);
  printf("sample average = %lf\n", average);
}

void Audio::showRecordingDevices() {
  if (numDevices < 0) {
    printf("ERROR: Pa_GetDeviceCount returned 0x%x\n", numDevices);
    close();
    return;
  }
  printf("Number of devices = %d\n", numDevices);

  for (int i = 0; i < numDevices; i++) {
    deviceInfo = Pa_GetDeviceInfo(i);
    if (deviceInfo->maxInputChannels > 0) {
      printf("--------------------------------------- device #%d\n", i);

      /* Mark global and API specific default devices */
      defaultDisplayed = 0;
      if (i == Pa_GetDefaultInputDevice()) {
        printf("[ Default Input");
        defaultDisplayed = 1;
      } else if (i ==
                 Pa_GetHostApiInfo(deviceInfo->hostApi)->defaultInputDevice) {
        const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
        printf("[ Default %s Input", hostInfo->name);
        defaultDisplayed = 1;
      }

      if (i == Pa_GetDefaultOutputDevice()) {
        printf((defaultDisplayed ? "," : "["));
        printf(" Default Output");
        defaultDisplayed = 1;
      } else if (i ==
                 Pa_GetHostApiInfo(deviceInfo->hostApi)->defaultOutputDevice) {
        const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
        printf((defaultDisplayed ? "," : "["));
        printf(" Default %s Output", hostInfo->name);
        defaultDisplayed = 1;
      }

      if (defaultDisplayed)
        printf(" ]\n");

        /* print device info fields */
#ifdef WIN32
      { /* Use wide char on windows, so we can show UTF-8 encoded device names
         */
        wchar_t wideName[MAX_PATH];
        MultiByteToWideChar(CP_UTF8, 0, deviceInfo->name, -1, wideName,
                            MAX_PATH - 1);
        wprintf(L"Name                        = %s\n", wideName);
      }
#else
      printf("Name                        = %s\n", deviceInfo->name);
#endif
      printf("Host API                    = %s\n",
             Pa_GetHostApiInfo(deviceInfo->hostApi)->name);
      printf("Max inputs = %d", deviceInfo->maxInputChannels);
      printf(", Max outputs = %d\n", deviceInfo->maxOutputChannels);

      printf("Default low input latency   = %8.4f\n",
             deviceInfo->defaultLowInputLatency);
      printf("Default low output latency  = %8.4f\n",
             deviceInfo->defaultLowOutputLatency);
      printf("Default high input latency  = %8.4f\n",
             deviceInfo->defaultHighInputLatency);
      printf("Default high output latency = %8.4f\n",
             deviceInfo->defaultHighOutputLatency);

#ifdef WIN32
#if PA_USE_ASIO
      /* ASIO specific latency information */
      if (Pa_GetHostApiInfo(deviceInfo->hostApi)->type == paASIO) {
        long minLatency, maxLatency, preferredLatency, granularity;

        err = PaAsio_GetAvailableLatencyValues(i, &minLatency, &maxLatency,
                                               &preferredLatency, &granularity);

        printf("ASIO minimum buffer size    = %ld\n", minLatency);
        printf("ASIO maximum buffer size    = %ld\n", maxLatency);
        printf("ASIO preferred buffer size  = %ld\n", preferredLatency);

        if (granularity == -1)
          printf("ASIO buffer granularity     = power of 2\n");
        else
          printf("ASIO buffer granularity     = %ld\n", granularity);
      }
#endif /* PA_USE_ASIO */
#endif /* WIN32 */

      printf("Default sample rate         = %8.2f\n",
             deviceInfo->defaultSampleRate);

      /* poll for standard sample rates */
      inputParameters.device = i;
      inputParameters.channelCount = deviceInfo->maxInputChannels;
      inputParameters.sampleFormat = paInt16;
      inputParameters.suggestedLatency =
          0; /* ignored by Pa_IsFormatSupported() */
      inputParameters.hostApiSpecificStreamInfo = NULL;

      outputParameters.device = i;
      outputParameters.channelCount = deviceInfo->maxOutputChannels;
      outputParameters.sampleFormat = paInt16;
      outputParameters.suggestedLatency =
          0; /* ignored by Pa_IsFormatSupported() */
      outputParameters.hostApiSpecificStreamInfo = NULL;

      if (inputParameters.channelCount > 0) {
        printf("Supported standard sample rates\n for half-duplex 16 bit %d "
               "channel input = \n",
               inputParameters.channelCount);
        PrintSupportedStandardSampleRates(&inputParameters, NULL);
      }

      if (outputParameters.channelCount > 0) {
        printf("Supported standard sample rates\n for half-duplex 16 bit %d "
               "channel output = \n",
               outputParameters.channelCount);
        PrintSupportedStandardSampleRates(NULL, &outputParameters);
      }

      if (inputParameters.channelCount > 0 &&
          outputParameters.channelCount > 0) {
        printf("Supported standard sample rates\n for full-duplex 16 bit %d "
               "channel input, %d channel output = \n",
               inputParameters.channelCount, outputParameters.channelCount);
        PrintSupportedStandardSampleRates(&inputParameters, &outputParameters);
      }
    }
  }
}

void Audio::PrintSupportedStandardSampleRates(
    const PaStreamParameters *inputParameters,
    const PaStreamParameters *outputParameters) {
  static double standardSampleRates[] =
      {
          8000.0,  9600.0,  11025.0,  12000.0, 16000.0,
          22050.0, 24000.0, 32000.0,  44100.0, 48000.0,
          88200.0, 96000.0, 192000.0, -1 /* negative terminated  list */
      };
  int i, printCount;
  PaError err;

  printCount = 0;
  for (int i = 0; standardSampleRates[i] > 0; i++) {
    err = Pa_IsFormatSupported(inputParameters, outputParameters,
                               standardSampleRates[i]);
    if (err == paFormatIsSupported) {
      if (printCount == 0) {
        printf("\t%8.2f", standardSampleRates[i]);
        printCount = 1;
      } else if (printCount == 4) {
        printf(",\n\t%8.2f", standardSampleRates[i]);
        printCount = 1;
      } else {
        printf(", %8.2f", standardSampleRates[i]);
        ++printCount;
      }
    }
  }
  if (!printCount)
    printf("None\n");
  else
    printf("\n");
}

int Audio::recordCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags, void *userData) {
  paTestData *data = (paTestData *)userData;
  const SAMPLE *rptr = (const SAMPLE *)inputBuffer;
  SAMPLE *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
  long framesToCalc;
  long i;
  int finished;
  unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

  (void)outputBuffer; /* Prevent unused variable warnings. */
  (void)timeInfo;
  (void)statusFlags;
  (void)userData;

  if (framesLeft < framesPerBuffer) {
    framesToCalc = framesLeft;
    finished = paComplete;
  } else {
    framesToCalc = framesPerBuffer;
    finished = paContinue;
  }

  if (inputBuffer == NULL) {
    for (int i = 0; i < framesToCalc; i++) {
      *wptr++ = SAMPLE_SILENCE; /* left */
      if (NUM_CHANNELS == 2)
        *wptr++ = SAMPLE_SILENCE; /* right */
    }
  } else {
    for (int i = 0; i < framesToCalc; i++) {
      *wptr++ = *rptr++; /* left */
      if (NUM_CHANNELS == 2)
        *wptr++ = *rptr++; /* right */
    }
  }
  data->frameIndex += framesToCalc;
  return finished;
}

int Audio::playCallback(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags statusFlags, void *userData) {
  paTestData *data = (paTestData *)userData;
  SAMPLE *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
  SAMPLE *wptr = (SAMPLE *)outputBuffer;
  unsigned int i;
  int finished;
  unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

  (void)inputBuffer; /* Prevent unused variable warnings. */
  (void)timeInfo;
  (void)statusFlags;
  (void)userData;

  if (framesLeft < framesPerBuffer) {
    /* final buffer... */
    for (int i = 0; i < framesLeft; i++) {
      *wptr++ = *rptr++; /* left */
      if (NUM_CHANNELS == 2)
        *wptr++ = *rptr++; /* right */
    }
    for (; i < framesPerBuffer; i++) {
      *wptr++ = 0; /* left */
      if (NUM_CHANNELS == 2)
        *wptr++ = 0; /* right */
    }
    data->frameIndex += framesLeft;
    finished = paComplete;
  } else {
    for (int i = 0; i < framesPerBuffer; i++) {
      *wptr++ = *rptr++; /* left */
      if (NUM_CHANNELS == 2)
        *wptr++ = *rptr++; /* right */
    }
    data->frameIndex += framesPerBuffer;
    finished = paContinue;
  }
  return finished;
}
