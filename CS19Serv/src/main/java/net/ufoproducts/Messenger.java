package net.ufoproducts;
import com.twilio.Twilio;
import com.twilio.rest.api.v2010.account.Message;
import com.twilio.type.PhoneNumber;
import java.util.ArrayList;
/**
 * Messenger: Where the heavy stuff happens. This function stores the contacts, inits Twilio, and sends the alert message
 * to all contacts. this is done in two methods: send and sans. There is also a relationParser method, that converts the
 * enums of a Contact to a string.
 */
public class Messenger {
    public static String alertEncoder(byte alertmeme) {
        switch(alertmeme) {
            case 0:
                return "nothing is wrong.";
            case 1:
                return "geofence alert";
            case 2:
                return "fall alert";
            case 3:
                return "geofence and potential fall alert";
            case 4:
                return "temperature alert";
            case 5:
                return "geofence and temperature alert";
            case 6:
                return "temperature and potential fall alert";
            case 7:
                return "geofence, temperature and potential fall alert";
            default:
                return "something absolutely terrible alert"; //we really shouldn't reach this level
        }
    }
    public static boolean send(Alert alert) {
        ArrayList<Contact> contacts = new ArrayList<>();
        contacts.add(new Contact("Jeff Galindo", new PhoneNumber("+13473892365"), Relationship.RELATIVE));
        contacts.add(new Contact("Marie Carter", new PhoneNumber("+13473561439"), Relationship.CARETAKER));
        contacts.add(new Contact("William Galindo", new PhoneNumber("+19086279115"), Relationship.CHILD));
        String messageTemplate = "This is an automated alert from QUACK. Your $relationship has pressed the alert button on their QUACK bracelet and needs help. " +
                "They are currently located at these GPS coordinates: $lat, $lon and initiated this alert at $timedate. Please proceed to this location as soon as possible.";
        sans(contacts, alert, Env.user, Env.pass); //store API keys in env class and don't add to git
        return true;
    }
    private static void sans(ArrayList<Contact> c, Alert a, String user, String pass) {
        Twilio.init(user, pass);
        for(int x = 0; x < c.size(); x++) {
            String messageMeme = "Alert from LMAS: Your " + relationParser(c.get(x).relationship) + "'s LMAS unit has sent a ";
            messageMeme += alertEncoder(a.alertcode);
            messageMeme += ". Detailed information: ";
            messageMeme += "Current location: " + a.lat + a.dir1 + ", " + a.lon + a.dir2 + ". Temperature: " + a.temp + ".";
            messageMeme += " Please check on them as soon as possible.";
            Message.creator(c.get(x).number, new PhoneNumber("+14012508762"), messageMeme).create();
            try {
                Thread.sleep(15000); //hide for 15 seconds, prevents overloads of alerts
            } catch(Exception e) {
                //ignore it
            }
        }
    }
    private static String relationParser(Relationship r) {
        switch(r) {
            case RELATIVE:
                return "relative";
            case CHILD:
                return "parent";
            case GRANDCHILD:
                return "grand-parent";
            case CARETAKER:
                return "dependent";
            default:
                return "sans";
        }
    }
}
