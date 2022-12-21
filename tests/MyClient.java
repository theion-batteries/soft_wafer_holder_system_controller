import java.io.*;
import java.net.*;

// Java program to demonstrate BufferedReader
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class MyClient {
    /**
     * @param args
     */
    public static void main(String[] args) {
        while(true)
        {
            try {
                System.out.println("opening socket");
                Socket s = new Socket("192.168.0.209", 8882);
                DataOutputStream dout = new DataOutputStream(s.getOutputStream());
                DataInputStream din = new DataInputStream(s.getInputStream());
                // Enter data using BufferReader
                BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
                // Reading data using readLine
                String input = reader.readLine();
                // Printing the read line
                dout.writeUTF(input);
                InputStreamReader streamReader= new InputStreamReader(s.getInputStream());
                BufferedReader leser= new BufferedReader(streamReader);
                
                String value= leser.readLine();
                System.out.println(value);
                
                leser.close();
                dout.flush();
                dout.close();
                s.close();
            } catch (Exception e) {
                System.out.println(e);
            }
        }
  
    }
}