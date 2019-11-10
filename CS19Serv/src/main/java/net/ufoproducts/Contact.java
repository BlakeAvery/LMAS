package net.ufoproducts;

import com.twilio.type.PhoneNumber;
/**
 * Contact: The object that stores a single emergency contact. Is an object because hurr durr object oriented programming
 */
public class Contact {
    public String name;
    public PhoneNumber number;
    public Relationship relationship;
    public Contact(String na, PhoneNumber num, Relationship rel) {
        name = na;
        number = num;
        relationship = rel;
    }
}
