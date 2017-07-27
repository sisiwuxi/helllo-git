package com.example.Mdtvdemo;
import android.util.Log;


public class MUtils {
    private final static String TAG = "MUtil";
    private final static int MAIN = 0;
    private final static int SUB = 1;

    private static boolean mchdbflg = false;

    static {
        try {
            Log.i(TAG, "!!Trying to load libMctit_jni.so");
            System.loadLibrary("Mctit_jni");

        } catch (UnsatisfiedLinkError ule) {
            Log.e(TAG, "WARNING:Could not load libMctit_jni.so");
        }
    }

    public MUtils() {

    }

    public native static String ChinaNet_android_tm_Native_getSN();
    public native static String ChinaNet_android_tm_Native_Factory_IN_Certify(String pSN, String pMAC, String pIN, String pchipid, String pMD5);
    public native static String ChinaNet_android_tm_Native_TimeStamp();
    public native static String ChinaNet_android_tm_Native_Random();
    public native static String ChinaNet_android_tm_Native_TMtoken_Certify(String pAuthenticator, String pSN, String pChipID, String pIN);
    public native static int ChinaNet_android_tm_Native_OTP_SetChipID(String pChipID);
    public native static String ChinaNet_android_tm_Native_OTP_GetChipID();
    public native static int ChinaNet_android_tm_Native_OTP_Write_SN_Data(String pSN);
    public native static int ChinaNet_android_tm_Native_OTP_Write_IN_Data(String pIN);
    public native static int ChinaNet_android_tm_Native_OTP_Write_RSA_N_Data();
    public native static int ChinaNet_android_tm_Native_OTP_Write_RSA_E_Data();
    public native static int ChinaNet_android_tm_Native_OTP_Write_M_Data();
    public native static String ChinaNet_android_tm_Native_OTP_Read_Data();
    public native static int ChinaNet_android_tm_Native_OTP_MaxReservedSize();
    public native static String ChinaNet_android_tm_Native_OTP_Read_RSA_N_Data();
    public native static String ChinaNet_android_tm_Native_OTP_Read_RSA_E_Data();
    public native static int ChinaNet_android_tm_Native_OTP_Write_M_SN_IN_Data(String jpMSNIN);
    public native static int ChinaNet_android_tm_Native_OTP_Lock();
}
