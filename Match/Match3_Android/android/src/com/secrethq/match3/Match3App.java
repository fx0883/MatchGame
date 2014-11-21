package com.secrethq.match3;

import android.app.Application;
import android.content.Context;

public class Match3App extends Application{

	    private static Context context;

	    @Override
	    public void onCreate() {
	        super.onCreate();
	        context = getApplicationContext();
	    }

	    public static Context getAppContext() {
	        return context;
	    } 
	    
}
