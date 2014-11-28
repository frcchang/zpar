import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;

public class ZPar
{
  static {
    System.loadLibrary("zpar");
    System.loadLibrary("zpar-jni");
  }     

  private native long doOpen(String model, String task, String lang);
  private native String doProcess(long zpar, String sent);
  private native int doClose(long zpar);
  private native int doBatch(long zpar, String inf, String outf);
  private long zpar;

  public ZPar(String model, String task, String lang)
  {
    zpar = doOpen(model, task, lang);
  }
  public void close()
  {
    doClose(zpar);
  }
  public String process( String sent)
  {
    return doProcess(zpar, sent);
  }
  public int batch(String inf, String outf)
  {
    return doBatch(zpar, inf, outf);
  }
  public static void main( String [] args)
  {
    ZPar zpar = new ZPar(args[0], args[1], args[2]);
    if ( args.length > 3 ) {
      zpar.batch(args[3], args[4]);
    } else {
      BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
      String s;
      try {
        while ((s = in.readLine()) != null) {
          System.out.println(zpar.process(s));
        }
      } catch ( IOException e ) {
        System.err.println(e);
      }
    }
    zpar.close();
  }
}
