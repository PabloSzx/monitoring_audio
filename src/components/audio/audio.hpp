#include "portaudio.h"
#include <bits/stdc++.h>

/* #define SAMPLE_RATE  (17932) // Test failure to open with this value. */
#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_CHANNELS (2)
/* #define DITHER_FLAG     (paDitherOff) */
#define DITHER_FLAG (0) /**/
/** Set to 1 if you want to capture the recording to a file. */
#define WRITE_TO_FILE (0)

/* Select sample format. */
#if 1
#define PA_SAMPLE_TYPE paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 1
#define PA_SAMPLE_TYPE paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE (0)
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE (128)
#define PRINTF_S_FORMAT "%d"
#endif

typedef struct {
  int frameIndex; /* Index into sample array. */
  int maxFrameIndex;
  std::vector<SAMPLE> recordedSamples;
  // SAMPLE *recordedSamples;
} paTestData;

class Audio {
public:
  Audio();
  void showRecordingDevices();
  void record(int n);
  void play();
  void close(int i);

private:
  int numDevices, defaultDisplayed;
  const PaDeviceInfo *deviceInfo;
  PaStreamParameters inputParameters, outputParameters;
  PaError err = paNoError;

  PaStream *stream;
  paTestData data;
  int totalFrames;
  int numSamples;
  int numBytes;
  SAMPLE max, val;
  double average;

  static void
  PrintSupportedStandardSampleRates(const PaStreamParameters *inputParameters,
                                    const PaStreamParameters *outputParameters);
  static int recordCallback(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags, void *userData);
  static int playCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags, void *userData);
};