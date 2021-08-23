package com.grandpace.mrhud;

import android.content.Context;
import android.os.Environment;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;

public class MrBuff {
    public ByteBuffer   buff;
    private int ixSub,
                ixDone,  // ami már le lett tudva
                ixLoad,  // meddig van feltöltve
                ixNeed;  // amennyinek kéne lennie
    public int id;


    MrBuff( int i )
    {
        id = i;
        if( buff == null )
            buff = ByteBuffer.allocate(0x28000);
        else
            buff.allocate(0x28000);

        ixSub = ixDone = ixLoad = ixNeed = 0;
    }
    void reset()
    {
        buff.clear();
        ixSub = ixDone = ixLoad = ixNeed = 0;
    }
    int nLOAD()
    {
        return ixLoad-ixDone;
    }
    int nNEED()
    {
        if( ixNeed <= ixDone )
        {
            if( ixDone +8 <= ixLoad)
                ixNeed = ixDone + 8 + Integer.reverseBytes(buff.getInt( ixDone +4 ));
            else
                return ixLoad +1;
        }

        return ixNeed;
    }

    int nRemain()
    {
        int dif = nNEED()- ixLoad;
        return dif;
    }

    public ByteBuffer pDone()
    {
        if( nNEED() < 0 )
            return null; // nem lett elég adat hozzáadva a ixNeed kiszámításához
        if( ixDone == ixNeed)
            return null;
        if( ixNeed > ixLoad)
           return null;

        int nCap = ixNeed - (ixDone +8);
        ByteBuffer pOut = ByteBuffer.allocate( nCap );
        pOut.put( buff.array(), ixDone +8, nCap );
        ixSub = ixDone = ixNeed;
        return pOut;
    }
    MrBuff flush()
    {
        if( ixSub < 8 )
            return this;

        buff.position(0);
        if( ixLoad > ixSub)
            buff.put( buff.array(), ixDone, ixLoad - ixSub);

        ixNeed -= ixSub;
        ixLoad -= ixSub;
        if( ixLoad > 0 )
            buff.position(ixLoad);
        ixDone -= ixSub;
        ixSub = 0;
        return this;
    }
    MrBuff put( ByteBuffer bb, int ix, int n )
    {
        if( n == 0 )
            return this;
        int p = buff.position();
        try{
            if( buff.capacity() < n+p )
            {
                int nn = n*2 + p;
                nn += 0x10-(nn%0x10);
                if( p > 0 ) {
                    ByteBuffer tmp = ByteBuffer.allocate(p);
                    buff.flip();
                    tmp.put( buff ); //.array(), 0, buff.limit());
                    tmp.flip();
                    buff = ByteBuffer.allocate( nn );
                    buff.put( tmp );
                    tmp.clear();
                }
            }

            buff.put( bb.array(), ix, n );
        }
        catch(Exception e)
        {
        }
        ixLoad = buff.position();
        if( ixDone >= ixLoad)
            ixDone = ixNeed = 0;
        return this;
    }


    int pos()
    {
        return buff.position();
    }
    public boolean isExternalStorageWritable() {
        String state = Environment.getExternalStorageState();
        if (Environment.MEDIA_MOUNTED.equals(state)) {
            return true;
        }
        return false;
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

    private boolean save( Context myContext, byte [] aARR, int x, int n, String sAlbum, String sName ) {
        if( !isExternalStorageWritable() )
            return false;

        File dir = getPrivateAlbumStorageDir( myContext, sAlbum ); // getPublicAlbumStorageDir( album );
        File file = new File( dir + sName );
        FileOutputStream f;
        try{
            dir.mkdirs();
            f = new FileOutputStream( file );
            f.write( aARR, x, n );
            f.close();
            return true;
        } catch ( IOException e ){
            /*final String StrError = e.toString() + "\n" + mTVReplyFromServer.getText().toString();
            if( StrError.trim().length() != 0 )
                mTVReplyFromServer.setText( StrError );*/
        }

        return false;
    }
    private int van( byte [] aARR, int n, byte [] aVAN, int sn )
    {
        if( n == 0 )
            return 0;

        boolean [] bVAN = new boolean[0x100];
        bVAN[0] = true;
        for( int i = 1; i < 0x100; i++ )
        {
            bVAN[i] = false;
        }

        if( aVAN != null )
        for( int i = 0; i < sn; i++ )
        {
            bVAN[aVAN[i]] = true;
        }

        for( int i = 0; i < n; i++ )
        {
            if( bVAN[aARR[i]] );
            return i;
        }

        return n;
    }
    private int van( byte [] aVAN, int sn )
    {
        return van( buff.array(), buff.limit(), aVAN, sn );
    }
}
