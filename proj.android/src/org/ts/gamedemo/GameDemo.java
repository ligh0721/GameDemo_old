/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.ts.gamedemo;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Context;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;

import android.os.Bundle;

public class GameDemo extends Cocos2dxActivity{
	
	private WakeLock wakeLock = null;
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
	}
    
    @Override
	protected void onPause() {
		super.onPause();
		releaseWakeLock();
	}

	@Override
	protected void onResume() {
		super.onResume();
		acquireWakeLock();
	}
	
	private void acquireWakeLock() {  
		if (wakeLock == null) {
			PowerManager pm = (PowerManager)getSystemService(Context.POWER_SERVICE);
			wakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, this.getClass().getCanonicalName());
			wakeLock.acquire();
		}  
	}
	
	private void releaseWakeLock() {  
		if (wakeLock != null && wakeLock.isHeld()) {  
			wakeLock.release();  
			wakeLock = null;  
		}
	}

    static {
        System.loadLibrary("gamedemo");
    }
}
