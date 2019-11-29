package android.mitra.invento.extcamapplication.usbcameratest;

import android.app.Fragment;
import android.content.pm.ActivityInfo;
import android.mitra.invento.extcamapplication.R;
import android.os.Bundle;
import android.util.Log;
import android.view.Window;

import com.serenegiant.common.BaseActivity;

public class MainActivity extends BaseActivity {
    private static final boolean DEBUG = false;
    private static final String TAG = "MainActivity";

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);
        if (savedInstanceState == null) {
            if (DEBUG) Log.i(TAG, "onCreate:new");
            final Fragment fragment = new CameraFragment();
            getFragmentManager().beginTransaction()
                    .add(R.id.container, fragment).commit();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (DEBUG) Log.v(TAG, "onResume:");
//		updateScreenRotation();
    }

    @Override
    protected void onPause() {
        if (DEBUG) Log.v(TAG, "onPause:isFinishing=" + isFinishing());
        super.onPause();
    }

    @Override
    protected void onDestroy() {
        if (DEBUG) Log.v(TAG, "onDestroy:");
        super.onDestroy();
    }

    protected final void updateScreenRotation() {
        final int screenRotation = 2;
        switch (screenRotation) {
            case 1:
                setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT);
                break;
            case 2:
                setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
                break;
            default:
                setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED);
                break;
        }
    }

}
