package com.invento.mitra.modimitra;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Build;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.bumptech.glide.Glide;

public class MainActivity extends AppCompatActivity {

    ImageView flag1;
    ImageView flag2;
    private int currentApiVersion;
    LinearLayout ll;
    TextView tv;
    //LinearLayout gesLL;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        immersiveUI();

        TTS.init(getApplicationContext());

        flag1 = findViewById(R.id.flag1);
        flag2 = findViewById(R.id.flag2);
        ll = findViewById(R.id.ll);
        tv = findViewById(R.id.tv);


        Glide.with(this).load(this.getResources().getIdentifier("bengaluru", "mipmap", this.getPackageName())).into(flag1);
        Glide.with(this).load(this.getResources().getIdentifier("ribbon", "mipmap", this.getPackageName())).into(flag2);
        //Glide.with(this).load(this.getResources().getIdentifier("bengaluru", "mipmap", this.getPackageName())).into(ll);



        /*gesLL.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                TTS.speak("Welcome Narendra Modi");
                flag1.startAnimation(AnimationUtils.loadAnimation(getApplicationContext(), R.anim.slide));
            }
        });*/


        flag1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                TTS.speak("namaskara. Namma bengaluru hubbakke svagataa");
                ll.setVisibility(View.GONE);
                tv.setVisibility(View.VISIBLE);
                //flag1.startAnimation(AnimationUtils.loadAnimation(getApplicationContext(), R.anim.slide));
            }
        });

        /*flag2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //TTS.speak("Welcome Ivanka Trump");
                flag2.startAnimation(AnimationUtils.loadAnimation(getApplicationContext(), R.anim.slide));
                Handler ha = new Handler();
                ha.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        ll.setVisibility(View.GONE);
                        tv.setVisibility(View.VISIBLE);
                    }
                }, 2000);
            }
        });*/

        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ll.setVisibility(View.VISIBLE);
                tv.setVisibility(View.GONE);
            }
        });

    }

    private void immersiveUI() {
        currentApiVersion = android.os.Build.VERSION.SDK_INT;

        final int flags = View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;

        // This work only for android 4.4+
        if(currentApiVersion >= Build.VERSION_CODES.KITKAT)
        {
            getWindow().getDecorView().setSystemUiVisibility(flags);

            // Code below is to handle presses of Volume up or Volume down.
            // Without this, after pressing volume buttons, the navigation bar will
            // show up and won't hide
            final View decorView = getWindow().getDecorView();
            decorView
                    .setOnSystemUiVisibilityChangeListener(new View.OnSystemUiVisibilityChangeListener()
                    {

                        @Override
                        public void onSystemUiVisibilityChange(int visibility)
                        {
                            if((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0)
                            {
                                decorView.setSystemUiVisibility(flags);
                            }
                        }
                    });
        }
    }

    @SuppressLint("NewApi")
    @Override
    public void onWindowFocusChanged(boolean hasFocus)
    {
        super.onWindowFocusChanged(hasFocus);
        if(currentApiVersion >= Build.VERSION_CODES.KITKAT && hasFocus)
        {
            getWindow().getDecorView().setSystemUiVisibility(
                    View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
        }
    }

}
