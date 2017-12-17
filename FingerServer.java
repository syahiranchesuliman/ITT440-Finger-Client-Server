import java.io.*;
import java.net.*;
import java.util.Random;

public class FingerServer extends Thread {
    private ServerSocket sock;

    public FingerServer() {
        super("Finger Server");
        try {
            sock = new ServerSocket(79);
            System.out.println("Finger Server up and running ...");
        } catch (IOException e) {
            System.err.println("Error: couldn't create socket.");
            System.exit(1);
        }
    }

    public void run() {
        Socket client = null;

        // Look for clients
        while (true) {
            // Wait for a client
            if (sock == null)
                return;
            try {
                client = sock.accept();
            } catch (IOException e) {
                System.err.println("Error: couldn't connect to client.");
                System.exit(1);
            }

            // Process finger requests
            try {
                InputStreamReader isr = new InputStreamReader(
                    client.getInputStream());
                BufferedReader is = new BufferedReader(isr);
                PrintWriter os = new PrintWriter(new
                   BufferedOutputStream(client.getOutputStream()), false);
                String outLine = null;

                // Output server greeting
                os.println("*** Finger Server");
                os.flush();

                // Process and output user input
                String inLine = is.readLine();
                if (inLine.length() > 0)
                    outLine = inLine;
                readPlan(outLine, os);
                os.flush();

                // Clean up
                os.close();
                is.close();
                client.close();
            } catch (Exception e) {
                System.err.println("Error: " + e);
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] arguments) {
        FingerServer server = new FingerServer();
        server.start();
    }

    void readPlan(String userName, PrintWriter os) {
        try {
            FileReader file = new
                FileReader(userName + ".plan");
            BufferedReader buff = new
                BufferedReader(file);
            boolean eof = false;
            os.println("\nUser name: " + userName + "\n");
            while (!eof) {
                String line = buff.readLine();
                if (line == null)
                   eof = true;
                else
                   os.println(line);
            }
            buff.close();
        } catch (IOException e) {
            os.println("User " + userName + " not found.");
        }
    }
}