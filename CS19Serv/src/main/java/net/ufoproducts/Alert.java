package net.ufoproducts;

import java.util.ArrayList;
import java.util.Date;
/**
 * Alert: This object holds the representation of an alert sent by QUACK. It has two methods, other than the constructor:
 * getAlert, which is a static message that returns an alert, and the standard toString method.
 */
public class Alert {
    public String id;
    public String lat;
    public String dir1;
    public String lon;
    public String dir2;
    public double accel;
    public double temp;
    public byte alertcode;
    public String request;
    public Alert(String i, String la, String d1, String lo, String d2, double acc, double t, byte alerter) {
        id = i;
        lat = la;
        dir1 = d1;
        lon = lo;
        dir2 = d2;
        accel = acc;
        temp = t;
        alertcode = alerter;
    }
    public static Alert getAlert(String alert) {
        String[] fields = alert.split(",");
        for(int x = 0; x < fields.length; x++) {
            System.out.println(fields[x]);
        }
        if(fields.length != 8) {
            System.err.println("Invalid request length: Either old version or not a Redux");
            return new Alert("00000", "0", "N", "0", "E", -9.81, 72.0, (byte)0b000);
        }
        Date date = new Date();
        return new Alert(fields[0], fields[1], fields[2], fields[3], fields[4], Double.parseDouble(fields[5]), Double.parseDouble(fields[6]), Byte.parseByte(fields[8]));
    }
    public String toString() {
        return "ID: " + id + ", lat: " + lat + dir1 + ", long: " + lon + dir2 + ", accel: " + accel + ", temp: " + temp + ", alertcode: " + alertcode;
    }
}
