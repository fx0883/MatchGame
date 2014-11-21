
package com.secrethq.match3;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxEditText;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.lib.Cocos2dxRenderer;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.KeyEvent;
import android.view.ViewGroup;


public class Match3Activity extends Cocos2dxActivity{
	 
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate( savedInstanceState );
        
		setContentView(R.layout.main);

        this.mGLSurfaceView = (Cocos2dxGLSurfaceView) findViewById(R.id.game_gl_surfaceview);
        this.mGLSurfaceView.setEGLContextClientVersion(2);
        this.mGLSurfaceView.setCocos2dxRenderer(new Cocos2dxRenderer());
        
		this.mGLSurfaceView.setCocos2dxEditText( (Cocos2dxEditText)findViewById(R.id.textField) );
        
       
        
	}
	
    static {
        System.loadLibrary("game");
    }
    
	@Override 
	protected void onResume()
	{
		super.onResume();
        
	}
	@Override
    public boolean onKeyDown(int keyCode, KeyEvent event) //added by Usman (for exit game alert on back button pressed)//
	{
		
		if (keyCode != KeyEvent.KEYCODE_BACK)  return super.onKeyDown(keyCode, event);
//		postRequest();
        DialogInterface.OnClickListener dialogClickListener = new DialogInterface.OnClickListener() {

        public void onClick(DialogInterface dialog, int which) {
            switch (which){
            case DialogInterface.BUTTON_POSITIVE:
                //Yes button clicked
                break;

            case DialogInterface.BUTTON_NEGATIVE:
                //No button clicked
                break;
            }
        }
    };

	    AlertDialog.Builder builder = new AlertDialog.Builder(this);
	    builder.setTitle("Quit").setMessage("Are you sure?").setPositiveButton("Yes", dialogClickListener)
	        .setNegativeButton("No", dialogClickListener).show();

	    return super.onKeyDown(keyCode, event);
	}
}
