package com.sbstrm.appirater;

import android.app.Dialog;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.Uri;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

import org.cocos2dx.lib.Cocos2dxHelper;

public abstract class Appirater
{
    private static final String PREF_SIGNIFICANT_EVENT_COUNT = "significant_event";
    private static final String PREF_RATE_CLICKED = "rateclicked";
    private static final String PREF_DONT_SHOW = "dontshow";
    private static final String PREF_DATE_REMINDER_PRESSED = "date_reminder_pressed";
    private static final String PREF_DATE_FIRST_LAUNCHED = "date_firstlaunch";
    private static final Activity activity = Cocos2dxHelper.getActivity();
    private static boolean testMode = false;
    private static int initialDaysUntilPrompt = 0;
    private static int reminderDaysUntilPrompt = 0;
    private static int significantEventsUntilPrompt = 0;
    private static String marketUrl;

    private static SharedPreferences getSharedPreferences()
    {
        return activity.getSharedPreferences(activity.getPackageName() + ".appirater", 0);
    }

    private static int getResourceId(String idType, String idName)
    {
        return activity.getResources().getIdentifier(idName, idType, activity.getPackageName());
    }

    private static String getString(String idName)
    {
        int rId = getResourceId("string", idName);
        String text = activity.getResources().getString(rId);

        if (idName == "appirator_app_title") {
            return text;
        }

        final String appName = getString("appirator_app_title");
        return String.format(text, appName);
    }

    private static void showRateDialog()
    {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run()
            {
                final SharedPreferences.Editor editor = getSharedPreferences().edit();
                final Dialog dialog = new Dialog(activity);

                final int densityDpi = activity.getResources().getDisplayMetrics().densityDpi;
                final String release = android.os.Build.VERSION.RELEASE;

                if (release.startsWith("1.") || release.startsWith("2.0") || release.startsWith("2.1")) {
                    //No dialog title on pre-froyo devices
                    dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
                } else if (densityDpi == DisplayMetrics.DENSITY_LOW || densityDpi == DisplayMetrics.DENSITY_MEDIUM) {
                    Display display = ((WindowManager) activity.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
                    int rotation = display.getRotation();
                    if (rotation == 90 || rotation == 270) {
                        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
                    } else {
                        dialog.setTitle(getString("rate_title"));
                    }
                }else{
                    dialog.setTitle(getString("rate_title"));
                }

                LinearLayout layout = (LinearLayout) LayoutInflater.from(activity).inflate(getResourceId("layout", "appirater"), null);

                TextView tv = (TextView) layout.findViewById(getResourceId("id", "message"));
                tv.setText(getString("rate_message"));

                Button rateButton = (Button) layout.findViewById(getResourceId("id", "rate"));
                rateButton.setText(getString("rate"));
                rateButton.setOnClickListener(new OnClickListener() {
                    @Override
                    public void onClick(View v)
                    {
                        activity.startActivity(new Intent(
                            Intent.ACTION_VIEW,
                            Uri.parse(String.format(marketUrl, activity.getPackageName()))
                        ));
                        editor.putBoolean(PREF_RATE_CLICKED, true);
                        editor.commit();
                        dialog.dismiss();
                    }
                });

                Button rateLaterButton = (Button) layout.findViewById(getResourceId("id", "rateLater"));
                rateLaterButton.setText(getString("rate_later"));
                rateLaterButton.setOnClickListener(new OnClickListener() {
                    @Override
                    public void onClick(View v)
                    {
                        editor.putLong(PREF_DATE_REMINDER_PRESSED, System.currentTimeMillis());
                        editor.commit();
                        dialog.dismiss();
                    }
                });

                Button cancelButton = (Button) layout.findViewById(getResourceId("id", "cancel"));
                cancelButton.setText(getString("rate_cancel"));
                cancelButton.setOnClickListener(new OnClickListener() {
                    @Override
                    public void onClick(View v)
                    {
                        editor.putBoolean(PREF_DONT_SHOW, true);
                        editor.commit();
                        dialog.dismiss();
                    }
                });

                dialog.setContentView(layout);
                dialog.show();
            }
        });
    }

    public static void appLaunched() {
        SharedPreferences prefs = getSharedPreferences();
        if (prefs.getLong(PREF_DATE_FIRST_LAUNCHED, 0) > 0) {
            return;
        }

        SharedPreferences.Editor editor = prefs.edit();
        editor.putLong(PREF_DATE_FIRST_LAUNCHED, System.currentTimeMillis());
        editor.commit();
    }

    public static void userDidSignificantEvent()
    {
        SharedPreferences prefs = getSharedPreferences();
        SharedPreferences.Editor editor = prefs.edit();

        long significantEventCount = prefs.getLong(PREF_SIGNIFICANT_EVENT_COUNT, 0);
        significantEventCount++;

        editor.putLong(PREF_SIGNIFICANT_EVENT_COUNT, significantEventCount);
        editor.commit();
    }

    public static void showIfNeeded()
    {
        SharedPreferences prefs = getSharedPreferences();
        if (!testMode && (prefs.getBoolean(PREF_DONT_SHOW, false) || prefs.getBoolean(PREF_RATE_CLICKED, false))) {
            return;
        }

        if (testMode) {
            showRateDialog();
            return;
        }

        long date_firstLaunch = prefs.getLong(PREF_DATE_FIRST_LAUNCHED, 0);
        long date_reminder_pressed = prefs.getLong(PREF_DATE_REMINDER_PRESSED, 0);
        long millisecondsToWait = initialDaysUntilPrompt * 24 * 60 * 60 * 1000L;

        if (System.currentTimeMillis() >= (date_firstLaunch + millisecondsToWait)) {
            if (date_reminder_pressed != 0) {
                long remindMillisecondsToWait = reminderDaysUntilPrompt * 24 * 60 * 60 * 1000L;
                if (System.currentTimeMillis() >= (remindMillisecondsToWait + date_reminder_pressed)) {
                    showRateDialog();
                }
            } else {
                long significantEventCount = prefs.getLong(PREF_SIGNIFICANT_EVENT_COUNT, 0);
                if (significantEventsUntilPrompt <= significantEventCount) {
                    showRateDialog();
                }
            }
        }
    }

    public static void setDebug(boolean flag)
    {
        testMode = flag;
    }

    public static void setInitialDaysUntilPrompt(int days)
    {
        initialDaysUntilPrompt = days;
    }

    public static void setReminderDaysUntilPrompt(int days)
    {
        reminderDaysUntilPrompt = days;
    }

    public static void setSignificantEventsUntilPrompt(int events)
    {
        significantEventsUntilPrompt = events;
    }

    public static void setMarketUrl(String url)
    {
        marketUrl = url;
    }
}