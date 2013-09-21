package com.avalon.utils;

import java.util.concurrent.ConcurrentLinkedQueue;
import java.lang.Runnable;
import android.app.Activity;
import org.cocos2dx.lib.Cocos2dxHelper;

public abstract class AndroidHelper
{
    private static ConcurrentLinkedQueue<Runnable> jobs = new ConcurrentLinkedQueue<Runnable>();

    public static Activity getActivity()
    {
        return Cocos2dxHelper.getActivity();
    }

    public static void runOnUiThread(Runnable r)
    {
        getActivity().runOnUiThread(r);
	}

	public static void runOnMainThread(final Runnable r)
    {
        jobs.add(r);
	}

    public static void executeRunnables()
    {
        Runnable job = jobs.poll();
        if (job != null) {
            job.run();
        }
    }
}
