package com.grandpace.mrhud;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.Looper;
import android.util.Log;

import static android.content.ContentValues.TAG;

public class MrHUDService extends Service {

    private Looper mServiceLooper;
   // private ServiceHandler mServiceHandler;

    private static final String TAG = MrHUDService.class.getSimpleName();
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d( TAG, "onCreate'd" );
    }
    @Override
    public int onStartCommand( Intent intent, int flags, int StartId )
    {
        //super.onStartCommand( intent, 0, StartId );
        Log.d( TAG, "onStart'd" );
        return START_STICKY; //super.onStartCommand(intent,flags,StartId);
    }
    @Override
    public void onDestroy() {
        super.onDestroy();

        Log.d( TAG, "onDestroy'd" );
    }


}
