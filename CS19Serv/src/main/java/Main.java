import java.net.*; import java.io.*;
import net.ufoproducts.*;

/**
 * Main: Of course, the main class of the program, where everything is kicked off. Starts the server and listens for
 * any button press alerts. Passes off the message string to the static getAlert method of the Alert class, then passes
 * off the newly created Alert object to the static send method of the Messenger class.
 */

public class Main {
    public static void main(String[] args) {
        System.out.println("Server init!");
        ServerSocket succ;
        try {
            succ = new ServerSocket(8051);
        } catch(Exception e) {
            succ = null;
        }
        while(true) {
            try {
                Socket system = succ.accept();
                BufferedReader input = new BufferedReader(new InputStreamReader(system.getInputStream()));
                //OutputStreamWriter out = new OutputStreamWriter(system.getOutputStream());
                String request = input.readLine();
                System.out.println(request);
                if(request != null) {
                    Alert a = Alert.getAlert(request);
                    System.out.println(a);
                }
            } catch(Exception e) {
                System.err.println("F: " + e);
            }
        }
    }
}