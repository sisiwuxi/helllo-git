//<M Software>

package com.example.Mdtvdemo;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.os.Bundle;  
import android.os.Handler;  
import android.os.Message;  
import android.view.WindowManager;
import android.util.Log;
import android.os.SystemProperties;
import android.view.WindowManager;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.widget.TextView;
import android.view.accessibility.AccessibilityEvent;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapShader;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.ComposeShader;
import android.graphics.LinearGradient;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.RadialGradient;
import android.graphics.RectF;
import android.graphics.Shader;
import android.graphics.SweepGradient;
import android.graphics.drawable.BitmapDrawable;
import android.view.View;
import android.content.res.TypedArray;  
import android.graphics.Rect;  
import android.graphics.Paint.Style;  
import android.util.AttributeSet;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;
import java.util.HashMap;
import java.util.Map;
import org.apache.http.HttpResponse;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.InetSocketAddress;
import java.net.Proxy;
import java.net.URL;
import java.net.URLConnection;
import java.util.Iterator;
import java.util.Map;
import java.util.HashMap;


import android.app.Activity;  
import android.content.Intent;  
import android.os.Bundle;  
import android.view.View;  
import android.view.View.OnClickListener;  
import android.view.ViewGroup.LayoutParams;  
import android.widget.LinearLayout;  
import android.widget.TextView;  


import android.os.Bundle;  
import android.text.Editable;  
import android.text.Selection;  
import android.view.KeyEvent;  
import android.view.View;  
import android.view.View.OnClickListener;  
import android.widget.Button;  
import android.widget.EditText;  
import android.widget.TextView;  
import android.widget.Toast;  
import android.widget.TextView.OnEditorActionListener; 
import android.view.inputmethod.EditorInfo;



public class MainActivity extends Activity {
    private final static String TAG = "Certification";

    private Thread mHttpTread;
    private Thread mHttpTreadText;
    private Thread mHttpTreadNote;
    private TextView mCurNote;
    private Handler mTextHandler;
    private boolean mStopThread;

    private final static int HANDLER_SHOW_0 = 1 << 0;
    private final static int HANDLER_SHOW_1 = 1 << 1;
    private final static int HANDLER_SHOW_2 = 1 << 2;
    private final static int HANDLER_SHOW_3 = 1 << 3;
    private final static int HANDLER_SHOW_4 = 1 << 4;
    private final static int HANDLER_SHOW_5 = 1 << 5;
    private final static int HANDLER_SHOW_6 = 1 << 6;
    private final static int HANDLER_SHOW_7 = 1 << 7;
    private final static int HANDLER_SHOW_8 = 1 << 8;
	

   private long t1,t2;
   private String BaseURL = "http://:8088/BurnApp/servlet/GetSN?";
   //private String baseURL = "http://116.228.215.9:8080/BurnApp/servlet/GetSN?";
   private String BaseURL1 = "http://:8088/BurnApp/servlet/GetChallengeCode?";
   private String BaseURL2= "http://:8088/BurnApp/servlet/GetTMtoken?";
   private String URLIPStr = "";

   StringBuffer baseURL = new StringBuffer(BaseURL);
   StringBuffer baseURL1 = new StringBuffer(BaseURL1);
   StringBuffer baseURL2 = new StringBuffer(BaseURL2);

   private boolean  URLIPFlag = false;



    public String Trim(String IP){
        while(IP.startsWith(" ")){  
               IP= IP.substring(1,IP.length()).trim();  
            }  
        while(IP.endsWith(" ")){  
               IP= IP.substring(0,IP.length()-1).trim();  
            }  
        return IP;  
    }  
      
    public boolean isIp(String IP){
        boolean b = false;  
        IP = this.Trim(IP);  
        if(IP.matches("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}")){  
            String s[] = IP.split("\\.");  
            if(Integer.parseInt(s[0])<255)  
                if(Integer.parseInt(s[1])<255)  
                    if(Integer.parseInt(s[2])<255)  
                        if(Integer.parseInt(s[3])<255)  
                            b = true;  
        }  
        return b;  
    }  
  
  
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main); // set layout view 

       final EditText editText=(EditText)findViewById(R.id.edit_text);  
       editText.setOnEditorActionListener(new TextView.OnEditorActionListener() {  
       @Override  
       public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {  
       if (actionId == EditorInfo.IME_ACTION_SEND  
                               || actionId == EditorInfo.IME_ACTION_DONE  
                               || (event != null && KeyEvent.KEYCODE_ENTER == event.getKeyCode() && KeyEvent.ACTION_DOWN == event.getAction())) {  
                            URLIPStr = editText.getText().toString(); // listen onkey enter

                            if(isIp(URLIPStr) && URLIPStr.length()>=7) // check ip length & pattern
                         	{   		
    				            System.out.println("====>URLIPStr:");
                                System.out.println(URLIPStr);
    				            baseURL.insert(7,URLIPStr); 
                         	    baseURL1.insert(7,URLIPStr); 
                         	    baseURL2.insert(7,URLIPStr); //insert IP adress 
                         	    System.out.println("====>baseURL:");
                                System.out.println(baseURL.toString());
                         	    System.out.println("====>baseURL1:");
                                System.out.println(baseURL1.toString());
                         	    System.out.println("====>baseURL2:");
                                System.out.println(baseURL2.toString()); 
                                URLIPFlag = true;
                            }
							else
							{
							    Log.i(TAG, "InPut IP Error!!!!!!!!!!\n");
							    URLIPFlag = false;
							}
                       }  
                       return false;  
                   }  
       }); 


	    Log.i(TAG, "MainActivity extends Activity");
        mCurNote = (TextView) this.findViewById(R.id.ID_Note_Certification_wait); //hadle various messages
        mTextHandler = new Handler() {
            public void handleMessage(Message msg) {
                int value = msg.what;
		        mCurNote.setText(" "); 
                if ((value & HANDLER_SHOW_0) != 0) {
			 Log.i(TAG, "@@@@@@HANDLER_SHOW_0@@@@@");
			 mCurNote.setText("安全数据校验认证中..."); 
                }
                if ((value & HANDLER_SHOW_1) != 0) {
			 Log.i(TAG, "@@@@@@HANDLER_SHOW_1@@@@@");
			 mCurNote.setText("OTP 烧录空间不足"); 
                }
                if ((value & HANDLER_SHOW_2) != 0) {
			 Log.i(TAG, "@@@@@@HANDLER_SHOW_2@@@@@");					
			 mCurNote.setText("认证SN为空"); 
                }
                if ((value & HANDLER_SHOW_3) != 0) {
			  Log.i(TAG, "@@@@@@HANDLER_SHOW_3@@@@@");						
			  mCurNote.setText("获取安全数据失败"); 
                }
                if ((value & HANDLER_SHOW_4) != 0) {
			 Log.i(TAG, "@@@@@@HANDLER_SHOW_4@@@@@");						
			  mCurNote.setText("OTP烧录成功"); 
                }
                if ((value & HANDLER_SHOW_5) != 0) {
			  Log.i(TAG, "@@@@@@HANDLER_SHOW_5@@@@@");						
			  mCurNote.setText("认证成功，烧写OTP，请勿断电"); 
                }
                if ((value & HANDLER_SHOW_6) != 0) {
			   Log.i(TAG, "@@@@@@HANDLER_SHOW_6@@@@@");						
			   mCurNote.setText("认证异常，请重启apk"); 
                }
                if ((value & HANDLER_SHOW_7) != 0) {
			 Log.i(TAG, "@@@@@@HANDLER_SHOW_7@@@@@");						
  	              mCurNote.setText("请输入服务器IP地址开始云端认证"); 
                }
                if ((value & HANDLER_SHOW_8) != 0) {
			 Log.i(TAG, "@@@@@@HANDLER_SHOW_8@@@@@");						
  	              mCurNote.setText("网络异常,请检查"); 
                }
	
		          mCurNote.setVisibility(View.VISIBLE);
            }
        };

mHttpTreadText = new Thread() 
{
            int PostNumber = -1;
            @Override
            public void run() {
           //     while (!mStopThread) {
                    synchronized (this) {
                        try {
                           sendMessage(HANDLER_SHOW_7);

		                   t1=System.currentTimeMillis();
						   
                           do
                           {
                           t2=System.currentTimeMillis();
                           }while(t2-t1<3000);					

                            while(PostNumber != 0)
                           {
                                          if(URLIPFlag)
                                         {
                                            PostNumber = HTTP_POST_Factory_IN_Certify(); // main logic 
					                        Log.i(TAG, "==>@@@@@@HTTP_POST_Factory_IN_Certify@@@@@<==");
						                    URLIPFlag = false;
                                         }
									     else
									     {
									        Log.i(TAG, "==>@@@@@@URL IP SET ERROR!!!!!!@@@@@<==");
					     					t1=System.currentTimeMillis();
	                                        do
	                                        {
	                                             t2=System.currentTimeMillis();
	                                        }while(t2-t1<2000);		
									     }
					     
                           }


                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
            }
        };

       mHttpTreadText.start();	
    }

    private void sendMessage(int what) {
        Message msg = mTextHandler.obtainMessage();
        msg.what = what;
        mTextHandler.sendMessage(msg);
    }
	
    private String charset = "utf-8";
    private Integer connectTimeout = null;
    private Integer socketTimeout = null;
    private String proxyHost = null;
    private Integer proxyPort = null;
    private String sn = null;
    private String mac = null;
    private String in = null;
    private String ha = null;
    private String Authenticator = null;
    private String M_SN_IN_RSAE = null;

    
    /**
     * Do GET request  http get
     * @param url
     * @return
     * @throws Exception
     * @throws IOException
     */
    private String doGet(String url) throws Exception {
        
        URL localURL = new URL(url);
        
        URLConnection connection = openConnection(localURL);
        HttpURLConnection httpURLConnection = (HttpURLConnection)connection;
        
        httpURLConnection.setRequestProperty("Accept-Charset", charset);
        httpURLConnection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
        
        InputStream inputStream = null;
        InputStreamReader inputStreamReader = null;
        BufferedReader reader = null;
        StringBuffer resultBuffer = new StringBuffer();
        String tempLine = null;
        
        if (httpURLConnection.getResponseCode() >= 300) {
            throw new Exception("HTTP Request is not success, Response code is " + httpURLConnection.getResponseCode());
        }
        
        try {
            inputStream = httpURLConnection.getInputStream();
            inputStreamReader = new InputStreamReader(inputStream);
            reader = new BufferedReader(inputStreamReader);
            
            while ((tempLine = reader.readLine()) != null) {
                resultBuffer.append(tempLine);
            }
            
        } finally {
            
            if (reader != null) {
                reader.close();
            }
            
            if (inputStreamReader != null) {
                inputStreamReader.close();
            }
            
            if (inputStream != null) {
                inputStream.close();
            }
            
        }

        return resultBuffer.toString();
    }
    
    /**
     * Do POST request http post
     * @param url
     * @param parameterMap
     * @return
     * @throws Exception 
     */
    private String doPost(String url, Map parameterMap) throws Exception {
        
        /* Translate parameter map to parameter date string */
	 int dopostnumber =0;
        StringBuffer parameterBuffer = new StringBuffer();
        if (parameterMap != null) {
            Iterator iterator = parameterMap.keySet().iterator();
            String key = null;
            String value = null;
            while (iterator.hasNext()) {
                key = (String)iterator.next();
                if (parameterMap.get(key) != null) {
                    value = (String)parameterMap.get(key);
                } else {
                    value = "";
                }
                
                parameterBuffer.append(key).append("=").append(value);
                if (iterator.hasNext()) {
                    parameterBuffer.append("&");
                }
            }
        }
        
        System.out.println("POST parameter : " + parameterBuffer.toString());
        
        URL localURL = new URL(url);
        
        URLConnection connection = openConnection(localURL);
        HttpURLConnection httpURLConnection = (HttpURLConnection)connection;
        
        httpURLConnection.setDoOutput(true);
        httpURLConnection.setRequestMethod("POST");
        httpURLConnection.setRequestProperty("Accept-Charset", charset);
        httpURLConnection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
        httpURLConnection.setRequestProperty("Content-Length", String.valueOf(parameterBuffer.length()));
        
        OutputStream outputStream = null;
        OutputStreamWriter outputStreamWriter = null;
        InputStream inputStream = null;
        InputStreamReader inputStreamReader = null;
        BufferedReader reader = null;
        StringBuffer resultBuffer = new StringBuffer();
        String tempLine = null;
        
        try {
            outputStream = httpURLConnection.getOutputStream();
            outputStreamWriter = new OutputStreamWriter(outputStream);
            
            outputStreamWriter.write(parameterBuffer.toString());
            outputStreamWriter.flush();
            
            if (httpURLConnection.getResponseCode() >= 300) {
                throw new Exception("HTTP Request is not success, Response code is " + httpURLConnection.getResponseCode());
            }
            
            inputStream = httpURLConnection.getInputStream();
            inputStreamReader = new InputStreamReader(inputStream);
            reader = new BufferedReader(inputStreamReader);
            
            while ((tempLine = reader.readLine()) != null) {
                resultBuffer.append(tempLine);
		  if(dopostnumber == 5000)
		  	break;
            }
            
        } finally {
            
            if (outputStreamWriter != null) {
                outputStreamWriter.close();
            }
            
            if (outputStream != null) {
                outputStream.close();
            }
            
            if (reader != null) {
                reader.close();
            }
            
            if (inputStreamReader != null) {
                inputStreamReader.close();
            }
            
            if (inputStream != null) {
                inputStream.close();
            }
            
        }

        return resultBuffer.toString();
    }

    private URLConnection openConnection(URL localURL) throws IOException {
        URLConnection connection;
        if (proxyHost != null && proxyPort != null) {
            Proxy proxy = new Proxy(Proxy.Type.HTTP, new InetSocketAddress(proxyHost, proxyPort));
            connection = localURL.openConnection(proxy);
        } else {
            connection = localURL.openConnection();
        }
        return connection;
    }
    
    /**
     * Render request according setting
     * @param request
     */
    private void renderRequest(URLConnection connection) {
        
        if (connectTimeout != null) {
            connection.setConnectTimeout(connectTimeout);
        }
        
        if (socketTimeout != null) {
            connection.setReadTimeout(socketTimeout);
        }
        
    }

    private int HTTP_POST_Factory_IN_Certify() 
    {
        int ret = -1;
        int postnumber = 0;
		
	    try
	    {
			/* Post Request */
			Log.i(TAG, "enter into HTTP_POST_Factory_IN_Certify  Http request!!!");
			
			String SN = MUtils.ChinaNet_android_tm_Native_getSN(); // SN is related to property
			String ChipID = "66666666"; //chip id is hardcode now but not need write to otp
			                            //server will check chip id is not null
      	 
			Map dataMap = new HashMap();
			dataMap.put("SN", SN);
			dataMap.put("ChipID", ChipID);
			String  OutKeyData= null;

			
		    //OutKeyData = doPost(baseURL.toString(), dataMap);  //every SN only can be modify for onetime 
            OutKeyData = "Result=000,ResultDesc=??,Code=018999902016040812345678|5A0B6CBDB7E9|09A1C38A2C2D0DE8A029EDA731A0676AA89332A279F43845F22082D2227DD65C7646DA2CEDB0D748B8FBA9ED2D951B459CAA3777D971367CD2E54E1C95F82C71EF326032248345DEAB34AC13072290B65260BA2E5BB1F24A34C1297C18B09DA1CB71F52183D8403EE079F76FC3CA37A5E8B1F6D45EBFEC6232F1AC22485ADA85|9AA8FB7837DF0110A864AD533A79AB06";

			while(OutKeyData == null)
			{
			      OutKeyData = doPost(baseURL.toString(), dataMap);
				  if(OutKeyData!=null)  // break to out or wait for 2000 * 6
				  	break;
				  
                   t1=System.currentTimeMillis();
                   do
                   {
                      t2=System.currentTimeMillis();
                   }while(t2-t1<2000);
				   
                   Log.i(TAG, "@@@@@@0==========doPost===========baseURL@@@@@");
				    postnumber++;
				    if(postnumber >= 6)
				    sendMessage(HANDLER_SHOW_8);
				 	
			}
			
	        System.out.println(TAG+OutKeyData);
		    String sn_mac_in_ha[] = OutKeyData.split("=");

		    System.out.println("OutKeyData_Result.substring(0, 3):");
		    System.out.println(sn_mac_in_ha[1].substring(0, 3));
		   
        if(sn_mac_in_ha[1].substring(0, 3).equals("000"))
        {
		     sendMessage(HANDLER_SHOW_0);
        }
		else if(sn_mac_in_ha[1].substring(0, 3).equals("007"))
        {
		     sendMessage(HANDLER_SHOW_2);
        }
		else if(sn_mac_in_ha[1].substring(0, 3).equals("008"))
        {
		     sendMessage(HANDLER_SHOW_3);
        }
		else if(sn_mac_in_ha[1].substring(0, 3).equals("099"))
        {
		     sendMessage(HANDLER_SHOW_3);
        }
		
        System.out.println(sn_mac_in_ha[3]);

		
        String split_sn_mac_in_ha[] = sn_mac_in_ha[3].split("\\|");
		sn = split_sn_mac_in_ha[0];
		mac = split_sn_mac_in_ha[1];
		in = split_sn_mac_in_ha[2];
		ha = split_sn_mac_in_ha[3];


		
		System.out.println("sn:");
              System.out.println(sn);
		System.out.println("mac:");
              System.out.println(mac);
		System.out.println("in:");
              System.out.println(in);			  
		System.out.println("ha:");
              System.out.println(ha);
			  
		String IN = MUtils.ChinaNet_android_tm_Native_Factory_IN_Certify(sn,mac,in,ChipID,ha);

		System.out.println("000-IN:");
        System.out.println(IN);

              //http://192.168.54.102:8080/BurnApp/servlet/GetChallengeCode?
		if(IN != null)
		{
             Log.i(TAG, "@@@@@@Certify1====================SUC@@@@@");
             String TimeStamp = MUtils.ChinaNet_android_tm_Native_TimeStamp();
		     String Random = MUtils.ChinaNet_android_tm_Native_Random();
		     Map dataMap1 = new HashMap();
             dataMap1.put("SN", SN);
		     dataMap1.put("TimeStamp", TimeStamp);	   
             dataMap1.put("ChipID", Random);
             String  OutKeyData1= null;
             postnumber = 0;
		     while(OutKeyData1 == null)
		     {
		         OutKeyData1 = doPost(baseURL1.toString(), dataMap1);
			  if(OutKeyData1!=null)
			  	break;
                       t1=System.currentTimeMillis();
                       do
                       {
                          t2=System.currentTimeMillis();
                        }while(t2-t1<2000);
                        Log.i(TAG, "@@@@@@1==========doPost===========baseURL1@@@@@");
			   postnumber++;
			   if(postnumber >= 6)
			        sendMessage(HANDLER_SHOW_8);
		      }
			 
		   System.out.println(OutKeyData1);
	       String CTIT_T1_R1_T2_R2[] = OutKeyData1.split("=");
	       System.out.println("CTIT_T1_R1_T2_R2:");
	       System.out.println(CTIT_T1_R1_T2_R2[3]);
	       Authenticator = CTIT_T1_R1_T2_R2[3];
	       //Result=000,ResultDesc=??,ChallengeCode=33C5742698AE750839CFCED7AB39A68CC46B5FA4208A701A4845C542035AC0E5EDB6744272A2A4EE988BC0F56015B1F95DED8D3EFC0C31AE10C3C4362B628A2C2948BDA3BACEE60A054C85005FC21A97D20A1E393D47D0113E3FEB615E1345DC333AC1A60C9F1404EFFAF0EF461879143544D67F5987A0348A4E887226B4642A
	       String SHA256_SN_ChipID_IN_T2_R2 =MUtils.ChinaNet_android_tm_Native_TMtoken_Certify(Authenticator,sn,ChipID,IN);
	       System.out.println("SHA256_SN_ChipID_IN_T2_R2:");
	       System.out.println(SHA256_SN_ChipID_IN_T2_R2);

		     if(SHA256_SN_ChipID_IN_T2_R2 != null)
		     {
					Map dataMap2 = new HashMap();
					dataMap2.put("SN", SN);
					dataMap2.put("Authenticator", SHA256_SN_ChipID_IN_T2_R2);	   
					String  OutKeyData2= null;

                   postnumber = 0;
		           while(OutKeyData2 == null)
		           {
      		                 OutKeyData2 = doPost(baseURL2.toString(), dataMap2);
				   if(OutKeyData!=null)
			  	        break;
                               t1=System.currentTimeMillis();
                               do
                               {
                                      t2=System.currentTimeMillis();
                                }while(t2-t1<2000);
                                Log.i(TAG, "@@@@@@2==========doPost===========baseURL2@@@@@");
				    postnumber++;
			           if(postnumber >= 6)
			                 sendMessage(HANDLER_SHOW_8);
		            }
					   
		        System.out.println(OutKeyData2);
			    String TM_TMtoken_Result[] = OutKeyData1.split("=");
			    
			    System.out.println("TM_TMtoken_Result[1].substring(0, 3):");
                System.out.println(TM_TMtoken_Result[1].substring(0, 3));

			    if(TM_TMtoken_Result[1].substring(0, 3).equals("000"))
			    {
				    sendMessage(HANDLER_SHOW_5);
			    }
			    else
			    {
				    sendMessage(HANDLER_SHOW_6);
			    }

          		mStopThread = true;	
			    postnumber = 0;

			    t1=System.currentTimeMillis();
                do
                {
                t2=System.currentTimeMillis();
                }while(t2-t1<3000);	

                 if(TM_TMtoken_Result[1].substring(0, 3).equals("000"))
                {
				        int MaxReservedSize = MUtils.ChinaNet_android_tm_Native_OTP_MaxReservedSize();
		                System.out.println("MaxReservedSize:");
                        System.out.println(MaxReservedSize);
				   if(MaxReservedSize >=128)
				   {
                      String M = "CTIT";
					  String RSA_E= "0001374B";
                      M_SN_IN_RSAE = M + SN+IN+RSA_E;
					  System.out.println("=====================================");				 
                      System.out.println("M_SN_IN_RSAE.length:");
					  System.out.println(M_SN_IN_RSAE.length());
					  System.out.println("M_SN_IN_RSAE:");		
					  System.out.println(M_SN_IN_RSAE);
					  System.out.println("=====================================");		
                      MUtils.ChinaNet_android_tm_Native_OTP_Write_M_SN_IN_Data(M_SN_IN_RSAE);

 					  MUtils.ChinaNet_android_tm_Native_OTP_SetChipID(ChipID);
                      MUtils.ChinaNet_android_tm_Native_OTP_Write_RSA_N_Data();

									 
                      String OTPData = MUtils.ChinaNet_android_tm_Native_OTP_Read_Data();   //M+SN+IN+RSA_E
 					  System.out.println("OTPData:");
                      System.out.println(OTPData);

					  
                      String OTP_ChipID = MUtils.ChinaNet_android_tm_Native_OTP_GetChipID(); //ChipID
 					  System.out.println("OTP_ChipID:");
                      System.out.println(OTP_ChipID);	

					  
 					  String OTP_RSA_N = MUtils.ChinaNet_android_tm_Native_OTP_Read_RSA_N_Data();  //RSA_N
					  MUtils.ChinaNet_android_tm_Native_OTP_Lock(); //Lock OTP
 											
 					  sendMessage(HANDLER_SHOW_4);
				   }
				   else
				   {
				       Log.i(TAG, "Fail $$$$$$$$$$$$$$$$$$$$$$$$$$$$OTP ReservedSize < 128!!!!!!!");
					 sendMessage(HANDLER_SHOW_1);
				   }
                         }
		     }
		     else
		     {
		         Log.i(TAG, "@@@@@@Certify2====================FAIL@@@@@");
			   t1=System.currentTimeMillis();
                        do
                        {
                              t2=System.currentTimeMillis();
                        }while(t2-t1<3000);	
		         return -1;
		     }
			 
		}
		else
		{
		    Log.i(TAG, "@@@@@@Certify1====================FAIL@@@@@");
		    t1=System.currentTimeMillis();
                  do
                  {
                        t2=System.currentTimeMillis();
                   }while(t2-t1<3000);	
		     return -1;
		}

		return 0;
        }
        catch (Exception  e)
        {
              Log.i(TAG, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
              e.printStackTrace();
              sendMessage(HANDLER_SHOW_8);
              t1=System.currentTimeMillis();
              do
              {
                    t2=System.currentTimeMillis();
              }while(t2-t1<3000);	
		  
	          return -1;
        }
	 
    }
	
}
