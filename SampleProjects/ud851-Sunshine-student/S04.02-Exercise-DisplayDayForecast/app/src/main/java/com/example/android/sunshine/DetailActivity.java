package com.example.android.sunshine;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

public class DetailActivity extends AppCompatActivity {

    private static final String FORECAST_SHARE_HASHTAG = " #SunshineApp";

    TextView weather;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_detail);

        // TODO (2) Display the weather forecast that was passed from MainActivity

        Bundle bundle = getIntent().getExtras();
        String weatherForDay = bundle.getString("weatherForDay");

        weather = (TextView) findViewById(R.id.weatherForDay);
        weather.setText(weatherForDay);

    }
}