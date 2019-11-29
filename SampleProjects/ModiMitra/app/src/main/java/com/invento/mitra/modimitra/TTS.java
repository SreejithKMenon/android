package com.invento.mitra.modimitra;

import android.content.Context;
import android.speech.tts.TextToSpeech;
import android.speech.tts.UtteranceProgressListener;
import android.util.Log;

import java.util.Locale;

/**
 * Created by sreejithkmenon on 25/11/17.
 */

public class TTS  {

    public static TextToSpeech textToSpeech;
    public static String TAG = "TTS ";

    public static void init(final Context context) {
        if (textToSpeech == null) {
            textToSpeech = new TextToSpeech(context, new TextToSpeech.OnInitListener() {
                @Override
                public void onInit(int i) {


                    if (i == TextToSpeech.SUCCESS) {
                        textToSpeech.setSpeechRate((float) 0.87);
                        textToSpeech.setLanguage(new Locale("en"));

                        textToSpeech.setOnUtteranceProgressListener(new UtteranceProgressListener() {
                            @Override
                            public void onDone(String utteranceId) {
                                Log.d(TAG, "speech completed");
                            }

                            @Override
                            public void onError(String utteranceId) {
                            }

                            @Override
                            public void onStart(String utteranceId) {
                                Log.d(TAG, "speech started");
                            }
                        });
                    } else {
                        Log.e(TAG, "Initilization Failed!");
                    }
                }
            });
        }
    }

    public static void speak(final String text) {
        textToSpeech.speak(text, TextToSpeech.QUEUE_ADD, null,text);
    }

    public static void stopSpeech() {
        textToSpeech.stop();
    }

    public static void shutdownSpeech(){
        textToSpeech.shutdown();
    }

}


