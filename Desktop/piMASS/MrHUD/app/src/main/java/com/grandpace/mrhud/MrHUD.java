package com.grandpace.mrhud;

import android.content.Intent;
import android.content.res.Resources;
import android.content.Context;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.os.Handler;


import android.util.LruCache;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;



public class MrHUD extends AppCompatActivity implements View.OnClickListener {


    private ImageView mIV;
    private TextView mTVReplyFromServer;
    private EditText mETSendMessage;
    private EditText mETIPadr;
    private EditText mETIPport;

    public MrBuff jpgBUF;
    public SocketChannel socCLNT;

    public Context myContext;

    private LruCache<String,ByteBuffer> mBBCache;
    public void addBBCache(String key, ByteBuffer bb) {
        if( nBBadd > nBBget )
            return;
        nBBadd++;
        mBBCache.put(key, bb);
       /* if( getBMCache(key) == null )
            mBBCache.put(key, bb); */
    }
    public ByteBuffer getBBCache(String key) {
        if( nBBget == nBBadd )
            return null;
        nBBget = nBBadd;
        return mBBCache.get(key);
    }
    public ByteBuffer killBBCache(String key) {
        if( nBBget == nBBadd )
            return null;
        nBBget = nBBadd;
        return mBBCache.remove(key);
    }
    private LruCache<String, Bitmap> mMemoryCache;
    public void addBMCache(String key, Bitmap bitmap) {
        if( nBMadd > nBMget )
            return;
        nBMadd++;
        mMemoryCache.put(key, bitmap);
        /* if( getBMCache(key) == null )
            mMemoryCache.put(key, bitmap);*/
    }


    public Bitmap getBMCache(String key) {
        if( nBMget == nBMadd )
            return null;
        nBMget = nBMadd;
        return mMemoryCache.get(key);
    }

    public Bitmap killBMCache(String key) {
        if( nBMget == nBMadd )
            return null;
        nBMget = nBMadd;
        return mMemoryCache.remove(key);
    }

    public static int calculateInSampleSize(
            BitmapFactory.Options options, int reqWidth, int reqHeight) {
        // Raw height and width of image
        final int height = options.outHeight;
        final int width = options.outWidth;
        int inSampleSize = 1;

        if (height > reqHeight || width > reqWidth) {

            final int halfHeight = height / 2;
            final int halfWidth = width / 2;

            // Calculate the largest inSampleSize value that is a power of 2 and keeps both
            // height and width larger than the requested height and width.
            while ((halfHeight / inSampleSize) >= reqHeight
                    && (halfWidth / inSampleSize) >= reqWidth) {
                inSampleSize *= 2;
            }

            // This offers some additional logic in case the image has a strange
            // aspect ratio. For example, a panorama may have a much larger
            // width than height. In these cases the total pixels might still
            // end up being too large to fit comfortably in memory, so we should
            // be more aggressive with sample down the image (=larger inSampleSize).

            long totalPixels = width * height / inSampleSize;

            // Anything more than 2x the requested pixels we'll sample down further
            final long totalReqPixelsCap = reqWidth * reqHeight * 2;

            while (totalPixels > totalReqPixelsCap) {
                inSampleSize *= 2;
                totalPixels /= 2;
            }

        }

        return inSampleSize;
    }

    public static Bitmap decodeSampledBitmapFromResource(Resources res, int resId,
                                                         int reqWidth, int reqHeight) {

        // First decode with inJustDecodeBounds=true to check dimensions
        final BitmapFactory.Options options = new BitmapFactory.Options();
        options.inJustDecodeBounds = true;
        BitmapFactory.decodeResource(res, resId, options);

        // Calculate inSampleSize
        options.inSampleSize = calculateInSampleSize(options, reqWidth, reqHeight);

        // Decode bitmap with inSampleSize set
        options.inJustDecodeBounds = false;
        Bitmap bm = BitmapFactory.decodeResource(res, resId, options);


        return bm;
    }

    class BitmapWorkerTask extends AsyncTask<Integer, Void, Bitmap> {
        // Decode image in background.
        @Override
        protected Bitmap doInBackground( Integer... params) {

            int w = params[1], h = params[2];
            final Bitmap my_bitmap = Bitmap.createBitmap( params[1], params[1]/2, Bitmap.Config.ARGB_8888 );
            for( int i = 0, wh = w*h, x, y, c; i < wh; i++ )
            {
                x = i%w;
                y = i/w;
                c = y | ((x>y ? x : y)<<16) | (x<<16) | (128<<24); //B0 G8 R16 A24
                my_bitmap.setPixel( x, y, c );
            }
            addBMCache(String.valueOf(params[0]), my_bitmap);
            return my_bitmap;

        }
    }

    public void loadBitmap(int resId, ImageView mIV ) {
        final String imageKey = String.valueOf(resId);

        final Bitmap bitmap = getBMCache(imageKey);
        if (bitmap != null) {
            mIV.setImageBitmap(bitmap);
        } else {
            //mIV.setImageResource(R.drawable.grandpacelogo);
            BitmapWorkerTask task = new BitmapWorkerTask(); // mIV );
            task.execute(resId, 256, 128 );
        }
    }
    class BitmapWorkerTaskBB extends AsyncTask<Integer, Void, Bitmap> {
        // Decode image in background.
        @Override
        protected Bitmap doInBackground( Integer... params) {
            if( true )
            {
                ByteBuffer bb = killBBCache( String.valueOf(params[0]) );
                int nDONE = bb.position();
                //byte [] aB = bb.array();
                //bb.flip();
                byte [] aByte = bb.array();

                BitmapFactory.Options opt = new BitmapFactory.Options();

                opt.inJustDecodeBounds = true;
                BitmapFactory.decodeByteArray( aByte, 0, nDONE, opt );

                int Height = opt.outHeight;
                int Width = opt.outWidth;
                String Type = opt.outMimeType;



                opt.inSampleSize = 1; //opt.outWidth*opt.outHeight;
                //opt.inScaled = false;
                opt.inPreferredConfig = Bitmap.Config.ARGB_8888;
                opt.inJustDecodeBounds = false;
                opt.inMutable = false; //true;
                final Bitmap my_bitmap = BitmapFactory.decodeByteArray( aByte, 0, nDONE, opt );
                //my_bitmap = my_bitmap.copy(Bitmap.Config.ARGB_8888, true);
                /*int w = params[1], h = params[2];
                final Bitmap my_bitmap = Bitmap.createBitmap( params[1], params[1]/2, Bitmap.Config.ARGB_8888 );
                for( int i = 0, wh = w*h, x, y, c; i < wh; i++ )
                {
                    x = i%w;
                    y = i/w;
                    c = y | ((x>y ? x : y)<<16) | (x<<16) | (128<<24); //B0 G8 R16 A24
                    my_bitmap.setPixel( x, y, c );
                }*/
                //my_bitmap.recycle();
                addBMCache(String.valueOf(params[0]), my_bitmap);
                return my_bitmap;
            }
            final Bitmap bitmap = decodeSampledBitmapFromResource(
                    getResources(), params[0], params[1], params[1]);

            addBMCache(String.valueOf(params[0]), bitmap);
            return bitmap;
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        myContext = getApplicationContext();

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mr_hud);

        Button buttonSend = (Button) findViewById(R.id.btn_send);

        final int maxMemory = (int) (Runtime.getRuntime().maxMemory() / 1024);
        final int cacheSize = maxMemory / 8;
        mMemoryCache = new LruCache<String, Bitmap>(cacheSize) {
            @Override
            protected int sizeOf(String key, Bitmap bitmap) {
                // The cache size will be measured in kilobytes rather than
                // number of items.
                return bitmap.getByteCount() / 1024;
            }
        };

        mBBCache = new LruCache<String, ByteBuffer>(cacheSize) {
            //@Override
            protected int sizeOf(String key, ByteBuffer BBuff) {
                // The cache size will be measured in kilobytes rather than
                // number of items.
                return BBuff.capacity() / 1024;
            }
        };

        mIV = (ImageView) findViewById(R.id.idIV);
        mETSendMessage  = (EditText) findViewById(R.id.edt_send_message);
        mTVReplyFromServer = (TextView) findViewById(R.id.tv_reply_from_server);
        mETIPadr = (EditText) findViewById( R.id.inIPadr );
        mETIPport = (EditText) findViewById( R.id.inIPport );

        buttonSend.setOnClickListener(this);
    }



    @Override
    public void onClick(View v) {
        switch (v.getId())
        {
            case R.id.btn_send:
                bContinue = true; //!bContinue;
                sendMessage(
                                mETSendMessage.getText().toString(),
                                mETIPadr.getText().toString(),
                                mETIPport.getText().toString()
                            );

                //startService( new Intent(this, MrHUDService.class ));
                break;
        }
    }



    public int  bufINoff = 0,
                bufINload = 0,
                bufINalloc = 0x100,
                nREAD, //nRSkip = 0, nRSdone = 0,
                isPORT = 0;

    public MrBuff inBIN;
    public ByteBuffer   //bufIN = ByteBuffer.allocate(bufINalloc),
                        bufTMP,
                        bufREAD = ByteBuffer.allocate(0x1000);

    public String isADR = "";
    public InetSocketAddress isa = null;


    public int  bufOUToff = 0,
                bufOUTload = 0,
                bufOUTalloc = 0x100,
                nWRITE = 0;
    public ByteBuffer   bufOUT = ByteBuffer.allocate( bufOUTalloc ),
                        bufWRITE;

    public Handler RecvHandle = new Handler();
    public Thread RecvThread;
    public volatile boolean bRecvReqStop = true, bRecvDoneStop = true, bContinue = false,
                            bRecvReqWrite = false, bRecvReqPost = true, bRecvReqNew = true;
    public int nSleep = 0;

    private void joinTV( byte[] aAR, int ix, int ix2 )
    {
        if( (ix2-ix) < 1 )
            return;
        try {
            String strINbuff = new String( aAR, ix, ix2-ix, "UTF8");
            String strTV = mTVReplyFromServer.getText().toString();
            String[]    INline = strINbuff.split("\r\n|\r|\n"),
                    TVline = strTV.split("\r\n|\r|\n");
            int nINL = INline.length,
                    nTVL = TVline.length,
                    nTV = (mTVReplyFromServer.getHeight()/mTVReplyFromServer.getLineHeight())-1,
                    idx = 0, n, dif;
            if( nINL > nTV )
            {
                dif = nINL - nTV;
                for (int i = 0; i < dif; i++) {
                    n = strINbuff.indexOf('\n', idx);
                    if( n < 0 )
                        break;
                    idx = n+1;
                }
                if( idx > strINbuff.length() )
                    mTVReplyFromServer.setText( "" );
                else
                    mTVReplyFromServer.setText( strINbuff.substring( idx ) );
            } else {
                if (nINL + nTVL > nTV) {
                    dif = (nINL + nTVL) - nTV;
                    for (int i = 0; i < dif; i++) {
                        n = strTV.indexOf('\n', idx);
                        if( n < 0 )
                            break;
                        idx = n + 1;
                    }
                }

                if( idx > strTV.length() )
                    mTVReplyFromServer.setText( "" );
                else
                    mTVReplyFromServer.setText(strTV.substring(idx) + strINbuff);
            }
        } catch (Exception e) {

        }
    }
    private int van( byte [] aARR, int ix, int n, byte [] aVAN, int sn )
    {
        if( n == 0 )
            return 0;

        boolean [] bVAN = new boolean[0x110];
        bVAN[0] = true;
        for( int i = 1; i < 0x110; i++ )
        {
            bVAN[i] = false;
        }
        if( aVAN != null )
        for( int i = 0; i < sn; i++ )
        {
            bVAN[aVAN[i]] = true;
        }

        for( int e = ix+n; ix < e; ix++ )
        {
            if( !bVAN[aARR[ix]] )
                continue;
            return ix;
        }

        return ix;
    }
    public boolean isExternalStorageWritable() {
        String state = Environment.getExternalStorageState();
        if (Environment.MEDIA_MOUNTED.equals(state)) {
            return true;
        }
        return false;
    }
    public File getPublicAlbumStorageDir(String albumName) {
        // Get the directory for the user's public pictures directory.
        File file = new File(Environment.getExternalStoragePublicDirectory(
                Environment.DIRECTORY_DOWNLOADS), albumName);
        if (!file.mkdirs()) {
            //Log.e(LOG_TAG, "Directory not created");
        }
        return file;
    }
    public File getPrivateAlbumStorageDir(Context context, String albumName) {
        // Get the directory for the app's private pictures directory.
        File file = new File(context.getExternalFilesDir(
                Environment.DIRECTORY_PICTURES), albumName);
        if (!file.mkdirs()) {
            //Log.e(LOG_TAG, "Directory not created");
        }
        return file;
    }

    private int nBMadd = 0,
                nBMget = 0,
                nBBadd = 0,
                nBBget = 0;

    private void sndOUT( String sWrite )
    {
        try {
            bufOUT.clear();
            bufOUT.put(sWrite.getBytes());
            bufOUT.flip();
            if( bRecvReqWrite == true )
            {
                joinTV(bufOUT.array(), 0, nWRITE);
                joinTV(("\n").getBytes(), 0, 1);
            }
            while (bufOUT.hasRemaining()) {
                bufOUToff += socCLNT.write(bufOUT);
            }
            bufOUT.clear();
        } catch(Exception e) {
            final String StrError = e.toString() + "\n" + mTVReplyFromServer.getText().toString();
            if( StrError.trim().length() != 0 )
                mTVReplyFromServer.setText( StrError );
        }
    }
    private void sendMessage(final String msg, final String adr, final String port )
    {
        int b = isADR.compareToIgnoreCase(adr),
            p = Integer.parseInt(port, 10);
        if( bRecvDoneStop )
            isPORT = p-1;
        if( isPORT != p | b != 0 ) {
            if( RecvThread != null )
            {
                RecvThread.interrupt();
                try{
                    if( socCLNT.isConnected() )
                        socCLNT.close();
                } catch ( IOException e ) {

                }
                RecvThread = null;
            }



            isADR = adr;
            isPORT = p;
            bRecvReqStop = bRecvDoneStop = false;


            RecvThread = new Thread(
                    new Runnable() {
                        @Override
                        public void run() {
                            try {
                                if (socCLNT != null)
                                    if (socCLNT.isOpen()) {
                                        if (socCLNT.isConnected())
                                            socCLNT.close();
                                    }

                                if (socCLNT == null)
                                    socCLNT = SocketChannel.open();
                                else if (!socCLNT.isOpen())
                                    socCLNT = SocketChannel.open();
                                if (socCLNT == null)
                                    return;

                                if (!socCLNT.isConnected()) {
                                    isa = new InetSocketAddress(isADR, isPORT);
                                    socCLNT.connect(isa);
                                    socCLNT.configureBlocking(false);
                                    if( inBIN != null )
                                        inBIN.reset();
                                    mTVReplyFromServer.setText(" connect:" + isADR + ":" + Integer.toString(isPORT) + "\n");
                                }

                                String sWrite = msg + "\r\n";
                                nWRITE = sWrite.trim().length();

                                if (nWRITE > 0)
                                if (socCLNT.isOpen())
                                if (socCLNT.isConnected()) {
                                    if (bufOUTalloc < bufOUTload + nWRITE) {
                                        bufOUTalloc += nWRITE;
                                        bufOUTalloc += 0x10 - (bufOUTalloc % 0x10);
                                        bufTMP = ByteBuffer.allocate(bufOUTload);
                                        bufTMP.put(bufOUT.array(), 0, bufOUTload);
                                        bufOUT.position(bufOUTload);
                                    }

                                    sndOUT( sWrite );
                                }

                                int ix = 0, ix2 = 0, nSkip = 0;
                                ByteBuffer pDone = null;
                                final String imageKey = String.valueOf(R.drawable.ize);
                                //Bitmap pBM = null;

                                while( !bRecvReqStop & !bRecvDoneStop ) {
                                   /* try
                                    {


                                        final Bitmap pBM = killBMCache(imageKey);
                                        if( pBM != null )
                                        {
                                            // van bitmap feltölti
                                            if( pDone != null )
                                                pDone = null;

                                            if( bRecvReqPost == false )
                                            {
                                                runOnUiThread( new Runnable() {
                                                    @Override
                                                    public void run() {
                                                        mIV.setImageBitmap(pBM);
                                                        mIV.invalidate();
                                                        //btn.setText("#" + i);
                                                    }
                                                });
                                            }

                                            if (bContinue)
                                            if (nWRITE > 0)
                                            if (socCLNT.isOpen())
                                            if (socCLNT.isConnected()) {
                                                if( bufOUTalloc < bufOUTload + nWRITE ) {
                                                    bufOUTalloc += nWRITE;
                                                    bufOUTalloc += 0x10 - (bufOUTalloc % 0x10);
                                                    bufTMP = ByteBuffer.allocate(bufOUTload);
                                                    bufTMP.put(bufOUT.array(), 0, bufOUTload);
                                                    bufOUT.position(bufOUTload);
                                                }

                                                sndOUT( sWrite );
                                            }

                                            if( bRecvReqPost == true )
                                            {
                                                runOnUiThread( new Runnable() {
                                                    @Override
                                                    public void run() {
                                                        mIV.setImageBitmap(pBM);
                                                        mIV.invalidate();
                                                        //btn.setText("#" + i);
                                                    }
                                                });
                                                Thread.sleep(10);
                                            }


                                        }
                                    } catch (Exception e) {
                                        final String StrError = e.toString() + "\n" + mTVReplyFromServer.getText().toString();
                                        if( StrError.trim().length() != 0 )
                                            mTVReplyFromServer.setText( StrError );
                                        bRecvDoneStop = true;
                                    }*/

                                    try {
                                        nREAD = 0;
                                        if( socCLNT.isConnected() )
                                        {
                                            bufREAD.clear();
                                            nREAD = socCLNT.read( bufREAD );
                                            bufREAD.flip();
                                            if( nREAD != 0 )
                                            {
                                                ix = 0;
                                                ix2 = nREAD;
                                                if( (inBIN != null) ? ( inBIN.nLOAD() > 0 ) : false )
                                                {
                                                    ix2 = inBIN.nRemain();
                                                    if( ix2 < 0 )
                                                        continue; // nem lett elég adat hozzáadva a ixNeed kiszámításához

                                                    if( ix2 > nREAD )
                                                        ix2 = nREAD;

                                                    inBIN.put( bufREAD, 0, ix2 );

                                                    ix = ix2;
                                                    ix2 = nREAD;
                                                    if( ix > 0 )
                                                    {
                                                        // a csomag végén kéne lenie másik cucunak
                                                        pDone = inBIN.pDone();
                                                        if( pDone == null )
                                                        {
                                                            // nem tudott belölle kiszedni ByteBuffert, na most van probléma

                                                        }
                                                        else if( bRecvReqNew )
                                                        {
                                                            byte[] aByte = pDone.array();
                                                            int nDONE = pDone.position();
                                                            final Bitmap my_bitmap = BitmapFactory.decodeByteArray(aByte, 0, nDONE, null);

                                                            runOnUiThread(new Runnable() {
                                                                @Override
                                                                public void run() {
                                                                    mIV.setImageBitmap(my_bitmap);
                                                                    mIV.invalidate();
                                                                }
                                                            });

                                                            if( pDone != null )
                                                                pDone = null;
                                                            inBIN.flush();

                                                            if (bContinue)
                                                            if (nWRITE > 0)
                                                            if (socCLNT.isOpen())
                                                            if (socCLNT.isConnected()) {
                                                                if( bufOUTalloc < bufOUTload + nWRITE ) {
                                                                    bufOUTalloc += nWRITE;
                                                                    bufOUTalloc += 0x10 - (bufOUTalloc % 0x10);
                                                                    bufTMP = ByteBuffer.allocate(bufOUTload);
                                                                    bufTMP.put(bufOUT.array(), 0, bufOUTload);
                                                                    bufOUT.position(bufOUTload);
                                                                }

                                                                sndOUT( sWrite );
                                                            }
                                                        } else {
                                                            addBBCache( imageKey, pDone );
                                                            BitmapWorkerTaskBB task = new BitmapWorkerTaskBB( );
                                                            task.execute( R.drawable.ize, 256, 128 );
                                                            inBIN.flush();
                                                        }
                                                    }
                                                }
                                                if( ix >= ix2 )
                                                    continue;

                                                ix2 = van( bufREAD.array(), ix, ix2-ix, null, 0 );
                                                if( bRecvReqWrite == true )
                                                    joinTV( bufREAD.array(), ix, ix2 );

                                                if( inBIN != null )
                                                    inBIN.reset();
                                                if( ix2 < nREAD )
                                                {
                                                    if( inBIN == null )
                                                        inBIN = new MrBuff( 0 );

                                                    inBIN.put( bufREAD, ix2, nREAD-ix2 );
                                                }
                                            }
                                        }
                                    } catch (Exception e) {
                                        final String StrError = e.toString() + "\n" + mTVReplyFromServer.getText().toString();
                                        if( StrError.trim().length() != 0 )
                                            mTVReplyFromServer.setText( StrError );
                                        bRecvDoneStop = true;
                                    }
                                }

                            } catch (Exception e) {
                                final String StrError = e.toString() + "\n" + mTVReplyFromServer.getText().toString();
                                if( StrError.trim().length() != 0 )
                                    mTVReplyFromServer.setText( StrError );
                               // bRecvDoneStop = true;
                            }
                            bRecvDoneStop = true;
                        }
                    }
            );
            RecvThread.start();
            return;
        }

        final Handler handler = new Handler();
        Thread thread = new Thread(
            new Runnable()
            {
                @Override
                public void run()
                {
                    try {
                        String sWrite = msg +"\r\n";
                        nWRITE = sWrite.trim().length();

                        if( nWRITE > 0 )
                        if( socCLNT.isOpen() )
                        if( socCLNT.isConnected() ) {

                            if( bufOUTalloc < bufOUTload + nWRITE )
                            {
                                bufOUTalloc += nWRITE;
                                bufOUTalloc += 0x10-(bufOUTalloc%0x10);
                                bufTMP = ByteBuffer.allocate( bufOUTload );
                                bufTMP.put( bufOUT.array(), 0, bufOUTload );
                                bufOUT.position(bufOUTload);
                            }

                            bufOUT.clear();
                            bufOUT.put(sWrite.getBytes());
                            bufOUT.flip();
                            joinTV( bufOUT.array(), 0, nWRITE );
                            joinTV( ("\n").getBytes(), 0, 1 );

                            while( bufOUT.hasRemaining() )
                            {
                                socCLNT.write(bufOUT);
                            }
                        }
                    } catch (IOException e) {
                        //e.printStackTrace();
                    }
                }
            }
        );

        thread.start();
    }


}


